#pragma once

#include <string>
#include <memory>
#include "quad_connection.h"
#include <mavsdk/mavsdk.h>
#include <mavlink/common/mavlink.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cmath>
#include "utils/strings.cpp"
#include "utils/mav_messages.cpp"
#include <iostream>

namespace quadlink{

quadlink::QuadConnector::QuadConnector() {}

quadlink::ConnectionStatus quadlink::QuadConnector::check_message(const uint8_t* buffer, ssize_t size, uint8_t target_ID)
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
                return quadlink::ConnectionStatus::Success;
            }
        }
    }

    return quadlink::ConnectionStatus::Failed;
}

quadlink::ConnectionStatus quadlink::QuadConnector::connect_udp(std::string& connection_url)
{
    /*
        Identifies the drone in the given IP/PORT and verifies the heartbeat.
    */

    quadlink::QuadConnector::connection_url = split_string(connection_url, ':');
    std::string ip_url = QuadConnector::connection_url[0];
    std::string port_url = QuadConnector::connection_url[1];

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

    uint8_t buffer[2048];
    sockaddr_in drone_addr;
    socklen_t addr_len = sizeof(drone_addr);

    /*
        Receive a message.
    */

    ssize_t len = recvfrom(quadlink::QuadConnector::sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&drone_addr, &addr_len);

    if (len < 0) {
        close(quadlink::QuadConnector::sockfd);
        return quadlink::ConnectionStatus::Timeout;
    } else {
        // TODO: Parametrize buffer size somehow
        return quadlink::QuadConnector::check_message(buffer, 2048, MAVLINK_MSG_ID_HEARTBEAT);
    }

    close(quadlink::QuadConnector::sockfd);
    return quadlink::ConnectionStatus::Failed;
}
}