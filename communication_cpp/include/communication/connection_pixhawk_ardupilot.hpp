#ifndef COMMUNICATION_CONNECTION_PIXHAWK_ARDUPILOT_HPP
#define COMMUNICATION_CONNECTION_PIXHAWK_ARDUPILOT_HPP

#include <string>
#include <memory>
#include <mavlink/v2.0/common/mavlink.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cmath>
#include <iostream>
#include <chrono>
#include <cstring>
#include "connection.hpp"
#include "message_factory.hpp"

namespace rcpilot {
    /**
     * @brief A class for connecting with a drone. SPECS: Pixhawk FC / Ardupilot Firmware
     */
    class ConnectionPixHawkArdupilot : public virtual rcpilot::Connector {
    public:
        /**
         * @brief Constructor for ConnectionPixHawkArdupilot.
         */
        ConnectionPixHawkArdupilot();

        /**
         * @brief Destructor for ConnectionPixHawkArdupilot.
         */
        ~ConnectionPixHawkArdupilot() = default;

        /**
         * @brief Connects to the drone in the given connection string.
         * @param connection_url The connection URL (e.g., IP and port).
         * @return The status of the connection.
         */
        rcpilot::ConnectionStatus action_connect(std::string& connection_url);

        /**
         * @brief Arms the drone.
         * @return The status of the connection.
         */
        rcpilot::ConnectionStatus action_arm();

        /**
         * @brief Changes the flight mode of the drone.
         * @param flight_mode The desired flight mode.
         * @return The status of the connection.
         */
        rcpilot::ConnectionStatus action_change_mode(ArdupilotFlightMode flight_mode);

        /**
         * @brief Sets the home position of the drone.
         * @return The status of the connection.
         */
        rcpilot::ConnectionStatus action_set_home_position();

        /**
         * @brief Initiates takeoff of the drone.
         * @return The status of the connection.
         */
        rcpilot::ConnectionStatus action_takeoff();

        /**
         * @brief Initiates landing of the drone.
         * @return The status of the connection.
         */
        rcpilot::ConnectionStatus action_land();

        /**
         * @brief Sets the target position in local NED coordinates.
         * @param position The array of positions, velocity, and acceleration.
         * @return The status of the connection.
         */
        rcpilot::ConnectionStatus set_position_target_local_ned(float* position);

        /**
         * @brief Sets the target position in body frame coordinates (velocity).
         * @param position The array of positions, velocity, and acceleration.
         * @return The status of the connection.
         */
        rcpilot::ConnectionStatus set_position_target_body_velocity(float* position);

        /**
         * @brief Sets the target position in body frame coordinates (position).
         * @param position The array of positions, velocity, and acceleration.
         * @return The status of the connection.
         */
        rcpilot::ConnectionStatus set_position_target_body_position(float* position);

        /**
         * @brief Sets the yaw of the drone.
         * @param position The yaw position.
         * @return The status of the connection.
         */
        rcpilot::ConnectionStatus set_yaw(float* position);

        /**
         * @brief Sets the yaw rate of the drone.
         * @param position The yaw rate.
         * @return The status of the connection.
         */
        rcpilot::ConnectionStatus set_yaw_rate(float* position);

        /**
         * @brief Checks if the drone is armed.
         * @return The status of the connection.
         */
        rcpilot::ConnectionStatus arm_check();

        /**
         * @brief Sets the takeoff height of the drone.
         * @param height The desired takeoff height.
         */
        void action_set_takeoff_height(float height);

    private:
        float _takeoff_height; ///< The takeoff height of the drone.
    };
}

#endif // COMMUNICATION_CONNECTION_PIXHAWK_ARDUPILOT_HPP