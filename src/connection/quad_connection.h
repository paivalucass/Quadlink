#pragma once

#include <string>
#include <memory>
#include <mavsdk/mavsdk.h>
#include <mavlink/common/mavlink.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cmath>


#define TIMEOUT_SECONDS 5;
#define TIMEOUT_MILISECONDS 0;

namespace quadlink {

    enum class ConnectionStatus{
        Success,
        Failed,
        Timeout
    };

    class QuadConnector
    {
    public:
        QuadConnector();

        quadlink::ConnectionStatus connect_udp(std::string& connection_url);

        quadlink::ConnectionStatus check_message(const uint8_t* buffer, ssize_t size, uint8_t target_ID);

        
        
    private:
        std::vector<std::string> connection_url;
        int sockfd;
        struct sockaddr_in server_addr;
        struct sockaddr_in drone_addr;
        uint8_t system_id;
        uint8_t component_id;
        uint8_t target_system_id;
        uint8_t target_component_id;
    };
}