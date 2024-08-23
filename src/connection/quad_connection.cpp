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

    if (QuadConnector::sockfd < 0)
    {
        cerr << "Failed creating socket" << endl;
    }

    QuadConnector::server_addr.sin_family = AF_INET;
    QuadConnector::server_addr.sin_port = htons(port);
    QuadConnector::server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Bind to all interfaces


}