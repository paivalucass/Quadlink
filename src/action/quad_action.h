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
    class QuadAction : public virtual quadlink::QuadConnector{
        public:
            QuadAction();

            ~QuadAction() = default;

            quadlink::ConnectionStatus action_arm();

            quadlink::ConnectionStatus action_change_mode(ArdupilotFlightMode flight_mode);

            quadlink::ConnectionStatus action_set_home_position();

            quadlink::ConnectionStatus action_takeoff();

            quadlink::ConnectionStatus action_land();
            
            void action_set_takeoff_height(float height);

        private:
            std::shared_ptr<quadlink::MessageFactory> msg_factory;
            float _takeoff_height;
    };
}