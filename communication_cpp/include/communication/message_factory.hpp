#pragma once

#include <string>
#include <memory>
#include "connection.hpp"
#include <mavlink/v2.0/common/mavlink.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cmath>
#include "communication/strings.hpp"
#include "communication/clock.hpp"
#include <iostream>
#include <chrono>
#include <cstring>

namespace rcpilot {

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
     * @brief Creates a set local NED command message.
     * @param position The array of positions, velocity, and acceleration.
     * @return The created message.
     */
    mavlink_set_position_target_local_ned_t create_set_local_ned_command(float* position);

    /**
     * @brief Creates a set yaw command message.
     * @param position The yaw position.
     * @return The created message.
     */
    mavlink_set_position_target_local_ned_t create_set_yaw(float* position);

    /**
     * @brief Creates a set yaw rate command message.
     * @param position The yaw rate.
     * @return The created message.
     */
    mavlink_set_position_target_local_ned_t create_set_yaw_rate(float* position);

    /**
     * @brief Creates a set body command message for velocity.
     * @param position The array of positions, velocity, and acceleration.
     * @return The created message.
     */
    mavlink_set_position_target_local_ned_t create_set_body_command_velocity(float* position);

    /**
     * @brief Creates a set body command message for position.
     * @param position The array of positions, velocity, and acceleration.
     * @return The created message.
     */
    mavlink_set_position_target_local_ned_t create_set_body_command_position(float* position);

    mavlink_set_position_target_local_ned_t create_standard_position_command(float* position, uint16_t type_mask, uint8_t frame_type);

}