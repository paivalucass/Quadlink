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
             * @brief Sets position x y z in local ned.
             * @param x The x-coordinate of the position.
             * @param y The y-coordinate of the position.
             * @param z The z-coordinate of the position.
             * @param vx The velocity in x.
             * @param vy The velocity in y.
             * @param vz The velocity in z.
             * @param ax The acceleration in x.
             * @param ay The acceleration in y.
             * @param az The acceleration in z. 
             * @param yaw The target yaw.
             * @param yaw_rate The yaw rate.
             * @return The quadlink::ConnectionStatus representing the go to.
            */
            quadlink::ConnectionStatus action_set_position_target_local_ned(float x, float y, float z, float vx = 0, float vy = 0, float vz = 0, float ax = 0, float ay = 0, float az = 0, float yaw = 0, float yaw_rate = 0, uint16_t type_mask = 0b0000110111111000);
            
            quadlink::ConnectionStatus action_set_position_target_body(float x, float y, float z, float vx = 0, float vy = 0, float vz = 0, float ax = 0, float ay = 0, float az = 0, float yaw = 0, float yaw_rate = 0, uint16_t type_mask = 0b0000110111111000);


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