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
#include <cstring>

namespace quadlink{

quadlink::QuadConnector::QuadConnector() : clock() , buffer_size(2048)
{
    buffer = new uint8_t[buffer_size];
    std::fill(buffer, buffer + buffer_size, 0);
}

quadlink::QuadConnector::~QuadConnector(){
    delete[] buffer;
}

quadlink::MessageStatus quadlink::QuadConnector::check_message(uint16_t target_ID)
{
    mavlink_message_t msg;
    mavlink_status_t status;
    quadlink::MessageStatus return_status;

    for (ssize_t i = 0; i < quadlink::QuadConnector::buffer_size; ++i) {
        if (mavlink_parse_char(MAVLINK_COMM_0, quadlink::QuadConnector::buffer[i], &msg, &status)) {
            if (msg.msgid == target_ID) {
                if (target_ID == MAVLINK_MSG_ID_HEARTBEAT){
                    // CURRENTLY HARD CODED BUT CAN BE THE CAUSE OF FUTURE PROBLEMS WITH CONNECTION, THIS SHOULD BE CHANGED ASAP TO A MORE DYNAMIC APPROACH
                    quadlink::QuadConnector::target_system_id = msg.sysid;
                    quadlink::QuadConnector::system_id = 1; // Considering both system id and component id from sneder are always 1 (should be working for now)
                    quadlink::QuadConnector::component_id = 1;
                    quadlink::QuadConnector::target_component_id = 1; // 1 is usually the autopilot (theres only one)
                }
                else if (target_ID == MAVLINK_MSG_ID_COMMAND_ACK){
                    mavlink_msg_command_ack_decode(&msg, &return_status.ack);
                }
                return_status.connection = quadlink::ConnectionStatus::Success;
                return return_status;
            }
        }
    }
    return_status.connection = quadlink::ConnectionStatus::Failed;
    return return_status;
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

quadlink::MessageStatus quadlink::QuadConnector::wait_message(uint16_t target_ID, double time_waiting)
{
    ssize_t recv_len;
    quadlink::MessageStatus status;
    status.connection = quadlink::ConnectionStatus::Failed;
    //  TODO: Change this?
    socklen_t addr_len = sizeof(quadlink::QuadConnector::drone_addr);

    /*
        Wait for given message
    */

    //  Reset clock choronometer
    quadlink::QuadConnector::clock.reset();

    while (quadlink::QuadConnector::clock.watch() < time_waiting && status.connection != quadlink::ConnectionStatus::Success)
    {
        std::fill(quadlink::QuadConnector::buffer, quadlink::QuadConnector::buffer + quadlink::QuadConnector::buffer_size, 0);
        /*
            recvfrom stores the address info into drone_addr
        */
        recv_len = recvfrom(quadlink::QuadConnector::sockfd, quadlink::QuadConnector::buffer, quadlink::QuadConnector::buffer_size, 0, (struct sockaddr *)&drone_addr, &addr_len);

        if (recv_len < 0) {
            status.connection = quadlink::ConnectionStatus::Timeout;
            return status;
        } else {
            status = quadlink::QuadConnector::check_message(target_ID);
        }
    }
    
    if (status.connection == quadlink::ConnectionStatus::Success)
    {
        return status;
    }
    status.connection = quadlink::ConnectionStatus::Timeout;
    return status;
}

quadlink::ConnectionStatus quadlink::QuadConnector::connect_udp(std::string& connection_url)
{
    /*
        Identifies the drone in the given IP/PORT and verifies the heartbeat.
        Waits 5 seconds for the heartbet.
        If heartbeat not found, returns a timeout.
    */

    quadlink::ConnectionStatus socket_check = quadlink::QuadConnector::create_socket(connection_url);

    /*
        Wait for heartbet mmessage (5 seconds)
    */

    quadlink::MessageStatus message_status = wait_message(MAVLINK_MSG_ID_HEARTBEAT, 5.0);

    if (message_status.connection == quadlink::ConnectionStatus::Success)
    {
        return quadlink::ConnectionStatus::Success;
    }
    else if (message_status.connection == quadlink::ConnectionStatus::Timeout)
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

quadlink::ConnectionStatus quadlink::QuadConnector::send_mav_message(mavlink_command_long_t &command)
{   
    mavlink_message_t msg = quadlink::QuadConnector::build_message(command);
    int len = mavlink_msg_to_send_buffer(quadlink::QuadConnector::buffer, &msg);
    quadlink::MessageStatus ack_status;

    // Try three times to receive ACK
    for (int i = 0; i < 3; i ++)
    {
        sendto(quadlink::QuadConnector::sockfd, quadlink::QuadConnector::buffer, len, 0, (struct sockaddr*)&drone_addr, sizeof(drone_addr));  

        ack_status = wait_message(MAVLINK_MSG_ID_COMMAND_ACK, 2.0);
        
        if (ack_status.connection == quadlink::ConnectionStatus::Success && ack_status.ack.result == MAV_RESULT_ACCEPTED)
        {   
            return quadlink::ConnectionStatus::Success;
        }
        else if (ack_status.connection == quadlink::ConnectionStatus::Timeout)
        {
            continue;
        }
        else{
            return quadlink::ConnectionStatus::Failed;
        }
    }
    return quadlink::ConnectionStatus::Timeout;
}

mavlink_message_t quadlink::QuadConnector::build_message(mavlink_command_long_t &command)
{
    mavlink_message_t msg;

    command.target_system = quadlink::QuadConnector::target_system_id;
    command.target_component = quadlink::QuadConnector::target_component_id;

    mavlink_msg_command_long_encode(quadlink::QuadConnector::system_id, quadlink::QuadConnector::component_id, &msg, &command);

    return msg;
}
}