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
#include <iostream>


quadlink::QuadConnector::QuadConnector() 
{
}


quadlink::ConnectionStatus quadlink::QuadConnector::connect_udp(std::string& connection_url)
{
    /*
        Identifies the drone in the given IP/PORT and verifies the heartbet.
    */
    QuadConnector::connection_url = split_string(connection_url, ':');
    string ip_url = QuadConnector::connection_url[0];
    string port_url = QuadConnector::connection_url[1];

    u_int16_t port = string_to_int16(port_url);
    
    quadlink::QuadConnector::sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (quadlink::QuadConnector::sockfd < 0)
    {
        close(quadlink::QuadConnector::sockfd);
        cerr << "Failed creating socket" << endl;
        return quadlink::ConnectionStatus::Failed;
    }

    quadlink::QuadConnector::server_addr.sin_family = AF_INET;
    quadlink::QuadConnector::server_addr.sin_port = htons(port);
    quadlink::QuadConnector::server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Bind to all interfaces

    if (bind(quadlink::QuadConnector::sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        close(quadlink::QuadConnector::sockfd);
        cerr << "Failed binding socket" << endl;
        return quadlink::ConnectionStatus::Failed;
    }

    uint8_t buffer[2048];
    sockaddr_in drone_addr;
    socklen_t addr_len = sizeof(drone_addr);

    ssize_t len = recvfrom(quadlink::QuadConnector::sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&drone_addr, &addr_len);

    if (len < 0) {
        close(sockfd);
        return quadlink::ConnectionStatus::Failed;
    } else {
        cout << "Message received, length: " << len << endl;
        mavlink_message_t msg;
        mavlink_status_t status;
        // Decode the MAVLink message
        for (ssize_t i = 0; i < len; ++i) {
            if (mavlink_parse_char(MAVLINK_COMM_0, buffer[i], &msg, &status)) {
                if (msg.msgid == MAVLINK_MSG_ID_HEARTBEAT) {
                    close(sockfd);
                    return quadlink::ConnectionStatus::Success;
                }
            }
        }
    }

    close(sockfd);
    return quadlink::ConnectionStatus::Failed;
}