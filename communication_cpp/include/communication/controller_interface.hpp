#ifndef CONTROLLER_INTERFACE_HPP
#define CONTROLLER_INTERFACE_HPP

#include "communication/connection.hpp"

namespace rcpilot{

    class FlightControllerInterface {
        public:
            virtual rcpilot::ConnectionStatus connect(std::string& connection_string) = 0;

            virtual rcpilot::ConnectionStatus arm() = 0;

            virtual rcpilot::ConnectionStatus change_mode(int mode) = 0;

            virtual rcpilot::ConnectionStatus set_home_position() = 0;

            virtual rcpilot::ConnectionStatus takeoff() = 0;

            virtual rcpilot::ConnectionStatus land() = 0;

            virtual rcpilot::ConnectionStatus goto_ned(float x, float y, float z) = 0;

            virtual rcpilot::ConnectionStatus goto_body(float x, float y, float z) = 0;

            virtual rcpilot::ConnectionStatus yaw(float yaw) = 0;

            virtual rcpilot::ConnectionStatus set_yaw_rate(float yaw_rate) = 0;

            virtual rcpilot::ConnectionStatus change_body_velocity(float x, float y, float z) = 0;

            virtual void set_takeoff_height(float height) = 0;

            virtual ~FlightControllerInterface() = default;
    };
}


#endif // CONTROLLER_INTERFACE_HPP