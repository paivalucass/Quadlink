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


namespace quadlink {
    class QuadConnector
    {
    public:
        QuadConnector(mavsdk::System& system);


    private:
        mavsdk::System& _system;
        int sockfd;
        struct sockaddr_in drone_addr;
        uint8_t system_id;
        uint8_t component_id;
        uint8_t target_system_id;
        uint8_t target_component_id;
    };
}