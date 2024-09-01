#pragma once

#include <string>
#include <memory>
#include "quad_connection.h"
#include <mavlink/common/mavlink.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cmath>
#include "utils/strings.h"
#include "utils/mav_messages.h"
#include "utils/clock.h"
#include <iostream>
#include <chrono>
#include <cstring>

namespace quadlink{
    class MessageFactory
    {
        public:
            MessageFactory();

            ~MessageFactory() = default;

            mavlink_command_long_t create_arm_command();

            mavlink_command_long_t create_mode_command(uint8_t mode);

            mavlink_command_long_t create_takeoff_command();

        private:

    };
}