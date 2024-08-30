#pragma once

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

#define TIMEOUT_SECONDS 5;
#define TIMEOUT_MILISECONDS 0;

namespace quadlink {

    enum class ConnectionStatus{
        Success,
        Failed,
        Timeout
    };

    typedef struct{
        ConnectionStatus connection;
        mavlink_command_ack_t ack;
        mavlink_sys_status_t sys;
    } MessageStatus;

    class QuadConnector
    {
    public:
        QuadConnector();

        ~QuadConnector();

        /*
        Identifies the drone in the given IP/PORT and verifies the heartbeat.
        Waits 5 seconds for the heartbet.
        If heartbeat not found, returns a quadlink::ConnectionStatus::Timeout.
        */
        quadlink::ConnectionStatus connect_udp(std::string& connection_url);

        /*
        Create a socket binded to the given URL info.
        */
        quadlink::ConnectionStatus create_socket(std::string& connection_url);

        /*
        Checks if a message is from the given mavlink ID.
        */
        quadlink::MessageStatus check_message(uint16_t target_ID);

        /*
        Wait for a given mavlink message to be received.
        */
        quadlink::MessageStatus wait_message(uint16_t target_ID, double time_waiting);

        /*
        Sends a mavlink message to the last drone url connected.
        */
        quadlink::ConnectionStatus send_mav_message(mavlink_command_long_t &msg);

        /*
        Appends the system ID and component ID, creating the message after.
        */
        mavlink_message_t build_message(mavlink_command_long_t &msg);

        
    private:
        quadlink::Clock clock;
        std::vector<std::string> connection_url;
        int sockfd;
        struct sockaddr_in server_addr;
        struct sockaddr_in drone_addr;
        uint8_t* buffer;
        size_t buffer_size;
        uint8_t system_id;
        uint8_t component_id;
        uint8_t target_system_id;
        uint8_t target_component_id;
    };
};