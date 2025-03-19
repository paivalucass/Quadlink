#pragma once

#include <string>
#include <memory>
#include <mavlink/v2.0/common/mavlink.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cmath>
#include "communication/strings.hpp"
#include "communication/clock.hpp"
#include <iostream>
#include <chrono>
#include <fcntl.h>
#include <termios.h>
#include <thread>

#define TIMEOUT_SECONDS 5;
#define TIMEOUT_MILISECONDS 0;

namespace rcpilot {

    /**
     * @brief Enum class for connection statuses.
     */
    // this should replace FlightStatus
    enum class ConnectionStatus{
        Finished,
        Failed,
        Timeout,
        In_progress
    };

    enum class EncodeType{
        COMMAND_LONG,
        POSITION_TARGET_LOCAL_NED,
        POSITION_TARGET_BODY
    };

    enum class ArdupilotFlightMode{
        STABILIZE = 0,
        GUIDED = 4,
        LOITER = 5,
        LAND = 9
    };

    /**
     * @brief Struct for the status of a message.
     */
    typedef struct{
        ConnectionStatus connection;
        mavlink_command_ack_t ack;
        mavlink_sys_status_t sys;
        mavlink_heartbeat_t heartbeat;
        mavlink_local_position_ned_t local_position;
    } MessageStatus;

    /**
     * @brief A class for connecting to an UAV.
     */
    class Connector
    {
    public:
        /**
         * @brief Constructor for Connector.
         */
        Connector();

        /**
         * @brief Destructor for Connector.
         */
        ~Connector();

        /**
         * @brief Identifies the drone in the given IP/PORT and verifies the heartbeat.
         * Waits 5 seconds for the heartbeat.
         * If heartbeat not found, returns a rcpilot::ConnectionStatus::Timeout.
         * @param connection_url The URL for the connection.
         * @return The status of the connection.
         */
        rcpilot::ConnectionStatus connect_udp(std::string& connection_url);

        /**
         * @brief Wait for a given mavlink message to be received.
         * @param target_ID The ID of the target message.
         * @param time_waiting The time to wait for the message in seconds.
         * @return The status of the message.
         */
        rcpilot::MessageStatus wait_message(uint16_t target_ID, double time_waiting);

        /**
         * @brief Sends a mavlink message to the last drone url connected.
         * @param msg The mavlink message.
         * @return The status of the connection.
         */
        rcpilot::ConnectionStatus send_mav_message(mavlink_message_t msg, EncodeType encode);

    protected:
        /**
         * @brief Appends the system ID and component ID, completing the message scope.
         * @param msg The command long to build.
         * @return The built message.
         */
        mavlink_message_t build_command(mavlink_command_long_t &command);

        /**
         * @brief Appends the system ID and component ID, completing the message scope.
         * @param msg The set_position_target_local_ned command to build.
         * @return The built message.
        */
        mavlink_message_t build_command(mavlink_set_position_target_local_ned_t &command);

    private:

        /**
         * @brief Create a socket binded to the given URL info.
         * @param connection_url The URL for the connection.
         * @return The status of the connection.
         */
        rcpilot::ConnectionStatus create_socket(std::string& connection_url);

        /**
         * @brief Open a serial port. This option is a WIP and may or may not be used in the future
         * @param device The serial port for the connection.
         * @return The status of the connection.
         */
        rcpilot::ConnectionStatus open_serial_port(std::string& device);

        /**
         * @brief Checks if a message is from the given mavlink ID.
         * @param target_ID The ID of the target message.
         * @return The status of the message.
         */
        rcpilot::MessageStatus check_message(uint16_t target_ID);

        rcpilot::Clock clock;
        std::vector<std::string> connection_url;
        static int sockfd;
        static int serialfd;
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