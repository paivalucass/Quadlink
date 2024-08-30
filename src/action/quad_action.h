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


namespace quadlink{
    class QuadAction : public virtual quadlink::QuadConnector{
        public:
            QuadAction();

            ~QuadAction() = default;

        private:
    };
}