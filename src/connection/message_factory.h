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
#include <cstring>

namespace quadlink{

    /**
     * @brief Enum class for Ardupilot flight modes.
     * Currently only supporting Ardupilot modes. PX4 should be added in the near future.
     */
    enum class ArdupilotFlightMode{
        STABILIZE = 0,
        GUIDED = 4,
        LOITER = 5,
        LAND = 9
    };

    /**
     * @brief A class for creating mavlink messages.
     */
    class MessageFactory
    {
        public:
            /**
             * @brief Constructor for MessageFactory.
             */
            MessageFactory();

            /**
             * @brief Destructor for MessageFactory.
             */
            ~MessageFactory() = default;

            /**
             * @brief Creates an arm command message.
             * @return The created message.
             */
            mavlink_command_long_t create_arm_command();

            /**
             * @brief Creates a mode command message.
             * @param mode The desired flight mode.
             * @return The created message.
             */
            mavlink_command_long_t create_mode_command(ArdupilotFlightMode mode);

            /**
             * @brief Creates a takeoff command message.
             * @param height The desired takeoff height.
             * @return The created message.
             */
            mavlink_command_long_t create_takeoff_command(float height);

            /**
             * @brief Creates a set home command message.
             * @return The created message.
             */
            mavlink_command_long_t create_set_home_command();

            /**
             * @brief Creates a land command message.
             * @return The created message.
             */
            mavlink_command_long_t create_land_command();

            /**
             * @brief Creates a set local NED command message.
             * @return The created message.
             */
            mavlink_set_position_target_local_ned_t create_set_local_ned_command(uint16_t type_mask, float x, float y, float z, float vx, float vy, float vz, float ax, float ay, float az, float yaw, float yaw_rate);
            
            /**
             * 
            */
            mavlink_set_position_target_local_ned_t create_set_body_command(uint16_t type_mask, float x, float y, float z, float vx, float vy, float vz, float ax, float ay, float az, float yaw, float yaw_rate);


        private:

    };
}