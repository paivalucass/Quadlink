#include <string>
#include <memory>
#include <mavlink/common/mavlink.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cmath>
#include <iostream>
#include <chrono>
#include <cstring>
#include "../connection/quad_connection.h"
#include "../connection/message_factory.h"


namespace quadlink{
    /**
     * @brief A class for performing actions on a quadcopter.
     */
    class QuadAction : public virtual quadlink::QuadConnector{
        public:
            /**
             * @brief Constructor for QuadAction.
             */
            QuadAction();

            /**
             * @brief Destructor for QuadAction.
             */
            ~QuadAction() = default;

            /**
             * @brief Arms the quadcopter.
             * @return The status of the connection.
             */
            quadlink::ConnectionStatus action_arm();

            /**
             * @brief Changes the flight mode of the quadcopter.
             * @param flight_mode The desired flight mode.
             * @return The status of the connection.
             */
            quadlink::ConnectionStatus action_change_mode(ArdupilotFlightMode flight_mode);

            /**
             * @brief Sets the home position of the quadcopter.
             * @return The status of the connection.
             */
            quadlink::ConnectionStatus action_set_home_position();

            /**
             * @brief Initiates takeoff of the quadcopter.
             * @return The status of the connection.
             */
            quadlink::ConnectionStatus action_takeoff();

            /**
             * @brief Initiates landing of the quadcopter.
             * @return The status of the connection.
             */
            quadlink::ConnectionStatus action_land();

            /**
             * @brief Sets the target position in local NED coordinates.
             * @param position The array of positions, velocity and acceleration.
             * @return The status of the connection.
             */
            quadlink::ConnectionStatus action_set_position_target_local_ned(float* position);

            /**
             * @brief Sets the target position in body frame coordinates.
             * @param position The array of positions, velocity and acceleration.
             * @return The status of the connection.
             */
            quadlink::ConnectionStatus action_set_position_target_body(float* position);


            /**
             * @brief Sets the takeoff height of the quadcopter.
             * @param height The desired takeoff height.
             */
            void action_set_takeoff_height(float height);

        private:
            std::shared_ptr<quadlink::MessageFactory> msg_factory;
            float _takeoff_height;
    };
}