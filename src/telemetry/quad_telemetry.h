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
    class QuadTelemetry : public virtual quadlink::QuadConnector{
        public:
            QuadTelemetry();

            ~QuadTelemetry() = default;

        private:
    };
}