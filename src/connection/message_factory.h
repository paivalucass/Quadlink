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

    /*
        Currently only supporting Ardupilot modes. PX4 should be added in the near future
    */
    enum class ArdupilotFlightMode{
        STABILIZE = 0,
        GUIDED = 4,
        LOITER = 5,
        LAND = 9
    };

    class MessageFactory
    {
        public:
            MessageFactory();

            ~MessageFactory() = default;

            mavlink_command_long_t create_arm_command();

            mavlink_command_long_t create_mode_command(ArdupilotFlightMode mode);

            mavlink_command_long_t create_takeoff_command(float height);

            mavlink_command_long_t create_set_home_command();

            mavlink_command_long_t create_land_command();

            mavlink_command_long_t create_set_local_ned();


        private:

    };
}