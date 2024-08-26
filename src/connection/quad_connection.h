#pragma once

#include <string>
#include <memory>
#include <mavlink/common/mavlink.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cmath>
#include <utils/clock.h>


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

        /*
        Identifies the drone in the given IP/PORT and verifies the heartbeat.
        Waits 5 seconds for the heartbet.
        If heartbeat not found, returns a quadlink::ConnectionStatus::Timeout.
        */
        quadlink::ConnectionStatus connect_udp(std::string& connection_url);

        /*
        Checks if a message is from the given mavlink ID.
        */
        quadlink::ConnectionStatus check_message(const uint8_t* buffer, ssize_t size, uint8_t target_ID);
        
    private:
        quadlink::Clock Clock;
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