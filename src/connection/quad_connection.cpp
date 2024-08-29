#include <string>
#include <memory>
#include "quad_connection.h"
#include <mavlink/common/mavlink.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cmath>
#include "utils/strings.h"
#include "utils/mav_messages.h"
#include "utils/clock.h"
#include <iostream>
#include <chrono>

namespace quadlink{

quadlink::QuadConnector::QuadConnector() : clock()
{
}

quadlink::ConnectionStatus quadlink::QuadConnector::check_message(const uint8_t* buffer, ssize_t size, uint16_t target_ID)
{
    mavlink_message_t msg;
    mavlink_status_t status;

    for (ssize_t i = 0; i < size; ++i) {
        if (mavlink_parse_char(MAVLINK_COMM_0, buffer[i], &msg, &status)) {
            if (msg.msgid == target_ID) {
                if (target_ID == MAVLINK_MSG_ID_HEARTBEAT){
                    quadlink::QuadConnector::target_system_id = msg.sysid;
                    quadlink::QuadConnector::system_id = msg.sysid;
                }
                std::cout << "Message checked" << std::endl;
                return quadlink::ConnectionStatus::Success;
            }
        }
    }

    return quadlink::ConnectionStatus::Failed;
}

quadlink::ConnectionStatus quadlink::QuadConnector::create_socket(std::string& connection_url)
{
    quadlink::QuadConnector::connection_url = split_string(connection_url, ':');
    std::string ip_url = quadlink::QuadConnector::connection_url[0];
    std::string port_url = quadlink::QuadConnector::connection_url[1];

    u_int16_t port = string_to_int16(port_url);
    
    quadlink::QuadConnector::sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (quadlink::QuadConnector::sockfd < 0)
    {
        close(quadlink::QuadConnector::sockfd);
        std::cerr << "Failed creating socket" << std::endl;
        return quadlink::ConnectionStatus::Failed;
    }

    struct timeval timeout;
    timeout.tv_sec = TIMEOUT_SECONDS; 
    timeout.tv_usec = TIMEOUT_MILISECONDS;           

    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        perror("setsockopt");
        close(sockfd);
        return quadlink::ConnectionStatus::Failed;
    }

    /*
        Binding socket to the determined port.
    */

    quadlink::QuadConnector::server_addr.sin_family = AF_INET;
    quadlink::QuadConnector::server_addr.sin_port = htons(port);
    // TODO: BIND ONLY TO THE SPECIFIED IP IN URL (CURRENTLY BINDING TO ALL)
    quadlink::QuadConnector::server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Bind to all interfaces 

    if (bind(quadlink::QuadConnector::sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        close(quadlink::QuadConnector::sockfd);
        std::cerr << "Failed binding socket" << std::endl;
        return quadlink::ConnectionStatus::Failed;
    }

    return quadlink::ConnectionStatus::Success;
}

quadlink::ConnectionStatus quadlink::QuadConnector::wait_message(uint16_t target_ID, double time_waiting)
{
    uint8_t buffer[2048];
    ssize_t recv_len;
    // TODO: How to instanciate this?
    socklen_t addr_len = sizeof(quadlink::QuadConnector::drone_addr);

    /*
        Wait for given message
    */

    // Reset clock choronometer
    quadlink::QuadConnector::clock.reset();

    quadlink::ConnectionStatus check = quadlink::ConnectionStatus::Failed;

    while (quadlink::QuadConnector::clock.watch() < time_waiting && check != quadlink::ConnectionStatus::Success)
    {
        std::fill(std::begin(buffer), std::end(buffer), 0);
        /*
            recvfrom stores the address info into drone_addr
        */
        recv_len = recvfrom(quadlink::QuadConnector::sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&drone_addr, &addr_len);

        if (recv_len < 0) {
            return quadlink::ConnectionStatus::Timeout;
        } else {
            // TODO: Parametrize buffer size somehow
            check = quadlink::QuadConnector::check_message(buffer, 2048, target_ID);
        }
    }
    
    if (check == quadlink::ConnectionStatus::Success)
    {
        return quadlink::ConnectionStatus::Success;
    }
    return quadlink::ConnectionStatus::Timeout;
}

quadlink::ConnectionStatus quadlink::QuadConnector::connect_udp(std::string& connection_url)
{
    /*
        Identifies the drone in the given IP/PORT and verifies the heartbeat.
        Waits 5 seconds for the heartbet.
        If heartbeat not found, returns a timeout.
    */


    // TODO: a method for socket creation/binding should be implemented later
    quadlink::ConnectionStatus socket_check = quadlink::QuadConnector::create_socket(connection_url);

    /*
        Wait for heartbet mmessage (5 seconds)
    */

    quadlink::ConnectionStatus message_status = wait_message(MAVLINK_MSG_ID_HEARTBEAT, 5.0);

    if (message_status == quadlink::ConnectionStatus::Success)
    {
        return quadlink::ConnectionStatus::Success;
    }
    else if (message_status == quadlink::ConnectionStatus::Timeout)
    {
        close(quadlink::QuadConnector::sockfd);
        return quadlink::ConnectionStatus::Timeout;
    }
    else
    {  
        close(quadlink::QuadConnector::sockfd);
        return quadlink::ConnectionStatus::Failed;
    }
}

quadlink::ConnectionStatus quadlink::QuadConnector::send_mav_message(mavlink_message_t &msg)
{   
    uint8_t send_buffer[1024];
    int len = mavlink_msg_to_send_buffer(send_buffer, &msg);
    quadlink::ConnectionStatus message_status;
    // TODO: Parametrize buffer size
    for (int i = 0; i < 3; i ++)
    {
        sendto(quadlink::QuadConnector::sockfd, send_buffer, len, 0, (struct sockaddr*)&drone_addr, sizeof(drone_addr));  

        message_status = wait_message(MAVLINK_MSG_ID_COMMAND_ACK, 2.0);
        
        if (message_status == quadlink::ConnectionStatus::Success)
        {
            return quadlink::ConnectionStatus::Success;
        }
    }
    return message_status;
}
}