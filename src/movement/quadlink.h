// quadlink.h
#pragma once

#include <string>
#include <memory>
#include "quadlink.h"
#include "../connection/quad_connection.h"
#include "../telemetry/quad_telemetry.h"
#include "../action/quad_action.h"
#include "format.h"
#include <iostream>
#include <mavlink/common/mavlink.h>
#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cmath>
#include "../connection/message_factory.h"

#define POSITION_THRESHOLD 0.1; // ideal value?

namespace quadlink {

    /**
     * @brief Enum class for different types of vehicles.
     */
    enum class Vehicle {
        QuadCopter
    };

    /**
     * @brief A class representing a UAV, adaptable to all vehicles.
     */
    class UAV : public QuadTelemetry, public QuadAction{
    public:
        /**
         * @brief Constructor of class UAV.
         * @param vehicle_type type of the UAV being used quadlink::Vehicle e.g Quadcopter.
         */
        UAV(Vehicle vehicle_type);

        /**
         * @brief Destructor of class UAV.
         */
        ~UAV() = default;

        /**
         * @brief Connect to the drone via UDP, TCP or serial.
         * @param connection_url The URL for the connection e.g 127.0.0.1:14568.
         * @return The quadlink::ConnectionStatus representing the connection. 
         */
        ConnectionStatus connect(std::string& connection_url);

        /**
         * @brief Perform pre-flight check and arm the drone in the determined flight mode.
         * @return The quadlink::ConnectionStatus representing the arming.
         */
        ConnectionStatus arm(ArdupilotFlightMode flight_mode);

        /**
         * @brief Perform takeoff to a target height.
         * @param target_height The target height for the takeoff.
         * @return The quadlink::ConnectionStatus representing the takeoff.
         */
        ConnectionStatus takeoff(float target_height, bool blocking);

        /**
         * @brief Perform landing.
         * @param check A boolean to check if landing is possible.
         * @return The quadlink::ConnectionStatus representing the landing.
         */
        ConnectionStatus land(bool check);

        /**
         * @brief Changes the position of the UAV using local NED coordinates.
         * @param x The x-coordinate of the position.
         * @param y The y-coordinate of the position.
         * @param z The z-coordinate of the position (reversed axis).
         * @return The status of the connection.
         */
        ConnectionStatus change_position_ned(float x = 0, float y = 0, float z = 0);

        /**
         * @brief Changes the position of the UAV using body frame coordinates.
         * @param x The x-coordinate of the position.
         * @param y The y-coordinate of the position.
         * @param z The z-coordinate of the position.
         * @return The status of the connection.
         */
        ConnectionStatus change_position_body(float x = 0, float y = 0, float z = 0);

        /**
         * @brief Changes the UAV position using a constant velocity.
         * @param vx The velocity in the x direction.
         * @param vy The velocity in the y direction.
         * @param vz The velocity in the z direction.
         * @return The status of the connection.
         */
        ConnectionStatus change_velocity(float vx = 0, float vy = 0, float vz = 0);

        /**
         * @brief Changes the UAV position using an acceleration (this might be dangerous, use carefully).
         * @param ax The acceleration in the x direction.
         * @param ay The acceleration in the y direction.
         * @param az The acceleration in the z direction.
         * @return The status of the connection.
         */
        ConnectionStatus change_acceleration(float ax = 0, float ay = 0, float az = 0);

        /**
         * @brief Changes the yaw of the quadcopter.
         * @return The status of the connection.
         */
        ConnectionStatus change_yaw();

        //TODO: add telemetry methods

    private:
        Vehicle vehicle_type;
        std::string vehicle_name;
        std::string connection_url;
    };
}