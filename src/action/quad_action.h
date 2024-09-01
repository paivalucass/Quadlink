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

            quadlink::ConnectionStatus action_change_mode(uint8_t mode);

            

        private:
            std::shared_ptr<quadlink::MessageFactory> msg_factory;
    };
}