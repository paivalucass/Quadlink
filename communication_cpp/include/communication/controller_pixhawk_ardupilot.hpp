#ifndef COMMUNICATION_CONTROLLER_PIXHAWK_ARDUPILOT_HPP
#define COMMUNICATION_CONTROLLER_PIXHAWK_ARDUPILOT_HPP

#include "communication/connection.hpp"
#include "communication/connection_pixhawk_ardupilot.hpp"
#include "communication/controller_interface.hpp"
#include "rclcpp/rclcpp.hpp"

namespace rcpilot {

    /**
     * @brief A class for controlling a Pixhawk flight controller with Ardupilot firmware.
     */
    class ControllerPixhawkArdupilot : public FlightControllerInterface {
    public:
        /**
         * @brief Constructor for ControllerPixhawkArdupilot.
         */
        ControllerPixhawkArdupilot();

        /**
         * @brief Destructor for ControllerPixhawkArdupilot.
         */
        ~ControllerPixhawkArdupilot() = default;

        /**
         * @brief Connects to the flight controller using the given connection string.
         * @param connection_string The connection string (e.g., IP and port).
         * @return The status of the connection.
         */
        rcpilot::ConnectionStatus connect(std::string& connection_string) override;

        /**
         * @brief Arms the flight controller.
         * @return The status of the connection.
         */
        rcpilot::ConnectionStatus arm() override;

        /**
         * @brief Changes the flight mode of the flight controller.
         * @param mode The desired flight mode.
         * @return The status of the connection.
         */
        rcpilot::ConnectionStatus change_mode(int mode) override;

        /**
         * @brief Sets the home position of the flight controller.
         * @return The status of the connection.
         */
        rcpilot::ConnectionStatus set_home_position() override;

        /**
         * @brief Initiates takeoff of the flight controller.
         * @return The status of the connection.
         */
        rcpilot::ConnectionStatus takeoff() override;

        /**
         * @brief Initiates landing of the flight controller.
         * @return The status of the connection.
         */
        rcpilot::ConnectionStatus land() override;

        /**
         * @brief Moves the flight controller to the specified NED coordinates.
         * @param x The x-coordinate.
         * @param y The y-coordinate.
         * @param z The z-coordinate.
         * @return The status of the connection.
         */
        rcpilot::ConnectionStatus goto_ned(float x, float y, float z) override;

        /**
         * @brief Moves the flight controller to the specified body frame coordinates.
         * @param x The x-coordinate.
         * @param y The y-coordinate.
         * @param z The z-coordinate.
         * @return The status of the connection.
         */
        rcpilot::ConnectionStatus goto_body(float x, float y, float z) override;

        /**
         * @brief Sets the yaw of the flight controller.
         * @param yaw The desired yaw.
         * @return The status of the connection.
         */
        rcpilot::ConnectionStatus yaw(float yaw) override;

        /**
         * @brief Sets the yaw rate of the flight controller.
         * @param yaw_rate The desired yaw rate.
         * @return The status of the connection.
         */
        rcpilot::ConnectionStatus set_yaw_rate(float yaw_rate) override;

        /**
         * @brief Changes the body velocity of the flight controller.
         * @param x The x-velocity.
         * @param y The y-velocity.
         * @param z The z-velocity.
         * @return The status of the connection.
         */
        rcpilot::ConnectionStatus change_body_velocity(float x, float y, float z) override;

        /**
         * @brief Sets the takeoff height of the flight controller.
         * @param height The desired takeoff height.
         */
        void set_takeoff_height(float height) override;

    private:
        rcpilot::ConnectionPixHawkArdupilot pixhawk_connector; ///< The connector for Pixhawk with Ardupilot firmware.
        float takeoff_height; ///< The takeoff height of the flight controller.

        /**
         * @brief Checks if the given flight mode is valid.
         * @param mode_int The flight mode as an integer.
         * @return True if the flight mode is valid, false otherwise.
         */
        bool isValidFlightMode(int mode_int);
    };

}

#endif // COMMUNICATION_CONTROLLER_PIXHAWK_ARDUPILOT_HPP