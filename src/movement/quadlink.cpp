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

    quadlink::ConnectionStatus status = quadlink::QuadConnector::connect_udp(connection_url);
    
    switch (status)
    {
        case (quadlink::ConnectionStatus::Success):
            std::cout << GREEN_BOLD_TEXT << "[INFO] " << UAV::vehicle_name << " CONNECTED SUCCESSFULLY" << RESET_TEXT << std::endl;
            return quadlink::FlightStatus::FINISHED;
        case (quadlink::ConnectionStatus::Failed):
            std::cerr << "[ERROR] " << UAV::vehicle_name << " FAILED CONNECTING" << RESET_TEXT << std::endl;
            return quadlink::FlightStatus::FAILED;
        case (quadlink::ConnectionStatus::Timeout):
            std::cerr << RED_BOLD_TEXT << "[TIMEOUT] " << UAV::vehicle_name << " CONNECTION TIMEOUT" << RESET_TEXT << std::endl;
            return quadlink::FlightStatus::TIMEOUT;
        default:
            return quadlink::FlightStatus::FAILED;
    }
}

FlightStatus UAV::arm(ArdupilotFlightMode flight_mode)
{
    std::cout << YELLOW_BOLD_TEXT << "[INFO] ARMING " << UAV::vehicle_name << RESET_TEXT << std::endl;

    /*
        Perform pre-flight check.
    */
    if (quadlink::QuadTelemetry::check_sensors_status() != quadlink::SensorStatus::Healthy)
    {
        return quadlink::FlightStatus::FAILED;
    } 

    if (quadlink::QuadAction::action_set_home_position() == quadlink::ConnectionStatus::Failed){
        std::cerr << YELLOW_BOLD_TEXT << "[ERROR] WARNING! " << UAV::vehicle_name << " FAILED SETTING HOME POSITION" << RESET_TEXT << std::endl;
    }

    if (quadlink::QuadAction::action_change_mode(flight_mode) == quadlink::ConnectionStatus::Failed){
        std::cerr << RED_BOLD_TEXT << "[ERROR] FAILED CHANGING " << UAV::vehicle_name << " FLIGHT MODE, UNSAFE TO ARM" << RESET_TEXT << std::endl;
        return quadlink::FlightStatus::FAILED;
    }


    /*
        Arm UAV.
    */
    switch (quadlink::QuadAction::action_arm())
    {
        case (quadlink::ConnectionStatus::Success):
            std::cout << GREEN_BOLD_TEXT << "[INFO] SUCCESS ARMING " << UAV::vehicle_name << RESET_TEXT << std::endl;
            return quadlink::FlightStatus::FINISHED;
        case (quadlink::ConnectionStatus::Failed):
            std::cerr << RED_BOLD_TEXT << "[ERROR] " << UAV::vehicle_name << " FAILED ARMING" << RESET_TEXT << std::endl;
            return quadlink::FlightStatus::FAILED;
        case (quadlink::ConnectionStatus::Timeout):
            std::cerr << RED_BOLD_TEXT << "[TIMEOUT] ARMING TIMED OUT " << RESET_TEXT << std::endl;
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
    // UAV::action->set_takeoff_altitude(target_height);

    // if (UAV::arm() == FlightStatus::FINISHED)
    // {
    //     std::cout << YELLOW_BOLD_TEXT << "[INFO] TAKING OFF..." << RESET_TEXT << std::endl;
    //     const mavsdk::Action::Result flying = UAV::action->takeoff();

    //     if (flying != mavsdk::Action::Result::Success)
    //     {
    //         std::cerr << "Takeoff Failed" << flying << std::endl;
    //         return FlightStatus::FAILED;
    //     }
    //     else
    //     {
    //         /*
    //             This guarantees that the takeoff will be finished before leaving the method
    //         */
    //         while (UAV::telemetry->position().relative_altitude_m < target_height)
    //         {
    //             std::this_thread::sleep_for(std::chrono::seconds(1));
    //         }
    //         std::cout << GREEN_BOLD_TEXT << "[INFO]" << UAV::vehicle_name << " finished climbing" << RESET_TEXT << std::endl;
    //         return FlightStatus::FINISHED;
    //     }
    // }
    // else
    // {
    //     return FlightStatus::FAILED;
    // }
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
