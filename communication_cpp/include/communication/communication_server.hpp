#include "rclcpp/rclcpp.hpp"
#include "library/srv/action.hpp"
#include "library/srv/movement.hpp"
#include "communication/connection.hpp"
#include "communication/connection_pixhawk_ardupilot.hpp"
#include "communication/message_factory.hpp"
#include "communication/controller_pixhawk_ardupilot.hpp"
#include "communication/controller_interface.hpp"
#include <csignal>
#include <iostream>

#include <string>

namespace LogColors {
    const std::string RESET  = "\033[0m";
    const std::string BLACK  = "\033[30m";
    const std::string RED    = "\033[31m";
    const std::string GREEN  = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string BLUE   = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN   = "\033[36m";
    const std::string WHITE  = "\033[37m";
}