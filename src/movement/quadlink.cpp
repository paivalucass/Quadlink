#include "quadlink.h"

namespace quadlink{

UAV::UAV(Vehicle vehicle_type) : vehicle_type(vehicle_type)  {
    
    switch (vehicle_type)
    {
    case quadlink::Vehicle::QuadCopter:
        vehicle_name = "QUADCOPTER";
        // More vehicle specific variables should be initialized here
        break;
    // TODO: Add more vehicle support in the future
    default:
        break;
    }
}

FlightStatus UAV::connect(std::string& connection_url)
{
    /*
        Connection URL only tested with UDP for now 
        e.g 127.0.0.1:14568 
    */

    //Connect via string
    std::cout << YELLOW_BOLD_TEXT << "[INFO] CONNECTING TO " << UAV::vehicle_name << RESET_TEXT << std::endl;

    quadlink::ConnectionStatus status = this->connect_udp(connection_url);
    
    switch (status)
    {
        case (quadlink::ConnectionStatus::Success):
            std::cout << GREEN_BOLD_TEXT << "[INFO] " << UAV::vehicle_name << " CONNECTED SUCCESSFULLY" << RESET_TEXT << std::endl;
            return quadlink::FlightStatus::FINISHED;
        case (quadlink::ConnectionStatus::Failed):
            std::cerr << "[INFO] " << UAV::vehicle_name << " FAILED CONNECTING" << RESET_TEXT << std::endl;
            return quadlink::FlightStatus::FAILED;
        case (quadlink::ConnectionStatus::Timeout):
            std::cout << RED_BOLD_TEXT << "[INFO] " << UAV::vehicle_name << " CONNECTION TIMEOUT" << RESET_TEXT << std::endl;
            return quadlink::FlightStatus::TIMEOUT;
        default:
            return quadlink::FlightStatus::FAILED;
    }
}

FlightStatus UAV::arm()
{
    // TODO: A message factory to abstract message creation
    // TODO: Verify vehicle integrity before arming via telemetry
    std::cout << YELLOW_BOLD_TEXT << "[INFO] ARMING " << UAV::vehicle_name << RESET_TEXT << std::endl;
    mavlink_command_long_t arm_command;
    
    arm_command.command = MAV_CMD_COMPONENT_ARM_DISARM;
    arm_command.confirmation = 0;
    arm_command.param1 = 1.0f;  // 1.0 for arm / 0.0 for disarm
    arm_command.param2 = 0.0f; 

    quadlink::ConnectionStatus status = this->send_mav_message(arm_command);
    switch (status)
    {
        case (quadlink::ConnectionStatus::Success):
            std::cout << GREEN_BOLD_TEXT << "[INFO] SUCCESS ARMING " << UAV::vehicle_name << RESET_TEXT << std::endl;
            return quadlink::FlightStatus::FINISHED;
        case (quadlink::ConnectionStatus::Failed):
            std::cout << RED_BOLD_TEXT << "[INFO] " << UAV::vehicle_name << " ARMED" << RESET_TEXT << std::endl;
            return quadlink::FlightStatus::FAILED;
        case (quadlink::ConnectionStatus::Timeout):
            std::cout << RED_BOLD_TEXT << "[INFO] ARMING TIMED OUT " << RESET_TEXT << std::endl;
            return quadlink::FlightStatus::TIMEOUT;
        default:
            return quadlink::FlightStatus::FAILED;
    }
}

FlightStatus UAV::takeoff(double target_height)
{
    /*
        Perform takeoff given a determined height
    */

    // Setting takeoff altitude 
    UAV::action->set_takeoff_altitude(target_height);

    if (UAV::arm() == FlightStatus::FINISHED)
    {
        std::cout << YELLOW_BOLD_TEXT << "[INFO] TAKING OFF..." << RESET_TEXT << std::endl;
        const mavsdk::Action::Result flying = UAV::action->takeoff();

        if (flying != mavsdk::Action::Result::Success)
        {
            std::cerr << "Takeoff Failed" << flying << std::endl;
            return FlightStatus::FAILED;
        }
        else
        {
            /*
                This guarantees that the takeoff will be finished before leaving the method
            */
            while (UAV::telemetry->position().relative_altitude_m < target_height)
            {
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            std::cout << GREEN_BOLD_TEXT << "[INFO]" << UAV::vehicle_name << " finished climbing" << RESET_TEXT << std::endl;
            return FlightStatus::FINISHED;
        }
    }
    else
    {
        return FlightStatus::FAILED;
    }
}

FlightStatus UAV::land(bool check)
{
    if (check)
    {
        if (UAV::telemetry->armed())
        {
            return FlightStatus::IN_PROGRESS;
        }
        else
        {
            std::cout << GREEN_BOLD_TEXT << "[INFO]" << UAV::vehicle_name << " landed successfully" << RESET_TEXT << std::endl;
            return FlightStatus::FINISHED;
        }
    }

    std::cout << YELLOW_BOLD_TEXT << "[INFO] Landing..." << RESET_TEXT << std::endl;
    const mavsdk::Action::Result landing = UAV::action->land();

    if (landing == mavsdk::Action::Result::Success)
    {
        return FlightStatus::IN_PROGRESS;
    }
    else
    {
        std::cerr << "Landing failed" << std::endl;
        return FlightStatus::FAILED;
    }
}
}
