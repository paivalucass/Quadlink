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

quadlink::ConnectionStatus UAV::connect(std::string& connection_url)
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
        case (quadlink::ConnectionStatus::Finished):
            std::cout << GREEN_BOLD_TEXT << "[INFO] " << UAV::vehicle_name << " CONNECTED SUCCESSFULLY" << RESET_TEXT << std::endl;
            return quadlink::ConnectionStatus::Finished;
        case (quadlink::ConnectionStatus::Failed):
            std::cerr << "[ERROR] " << UAV::vehicle_name << " FAILED CONNECTING" << RESET_TEXT << std::endl;
            return quadlink::ConnectionStatus::Failed;
        case (quadlink::ConnectionStatus::Timeout):
            std::cerr << RED_BOLD_TEXT << "[TIMEOUT] " << UAV::vehicle_name << " CONNECTION TIMEOUT" << RESET_TEXT << std::endl;
            return quadlink::ConnectionStatus::Timeout;
        default:
            return quadlink::ConnectionStatus::Failed;
    }
}

quadlink::ConnectionStatus UAV::arm(ArdupilotFlightMode flight_mode)
{
    std::cout << YELLOW_BOLD_TEXT << "[INFO] ARMING " << UAV::vehicle_name << RESET_TEXT << std::endl;

    /*
        Is vehicle armed?
    */
    if (quadlink::QuadTelemetry::check_is_armed() == quadlink::VehicleStatus::Armed)
    {
        return quadlink::ConnectionStatus::Finished;
    }

    /*
        Perform pre-flight check.
    */
    if (quadlink::QuadTelemetry::check_sensors_status() != quadlink::SensorStatus::Healthy)
    {
        return quadlink::ConnectionStatus::Failed;
    } 

    if (quadlink::QuadAction::action_set_home_position() == quadlink::ConnectionStatus::Failed){
        std::cerr << YELLOW_BOLD_TEXT << "[ERROR] WARNING! " << UAV::vehicle_name << " FAILED SETTING HOME POSITION" << RESET_TEXT << std::endl;
    }

    if (quadlink::QuadAction::action_change_mode(flight_mode) == quadlink::ConnectionStatus::Failed){
        std::cerr << RED_BOLD_TEXT << "[ERROR] FAILED CHANGING " << UAV::vehicle_name << " FLIGHT MODE, UNSAFE TO ARM" << RESET_TEXT << std::endl;
        return quadlink::ConnectionStatus::Failed;
    }

    /*
        Arm UAV.
    */
    switch (quadlink::QuadAction::action_arm())
    {
        case (quadlink::ConnectionStatus::Finished):
            std::cout << GREEN_BOLD_TEXT << "[INFO] SUCCESS ARMING " << UAV::vehicle_name << RESET_TEXT << std::endl;
            return quadlink::ConnectionStatus::Finished;
        case (quadlink::ConnectionStatus::Failed):
            std::cerr << RED_BOLD_TEXT << "[ERROR] " << UAV::vehicle_name << " FAILED ARMING" << RESET_TEXT << std::endl;
            return quadlink::ConnectionStatus::Failed;
        case (quadlink::ConnectionStatus::Timeout):
            std::cerr << RED_BOLD_TEXT << "[TIMEOUT] ARMING TIMED OUT " << RESET_TEXT << std::endl;
            return quadlink::ConnectionStatus::Timeout;
        default:
            return quadlink::ConnectionStatus::Failed;
    }
}

quadlink::ConnectionStatus UAV::takeoff(float target_height, bool blocking)
{
    /*
        Perform takeoff given a determined height
    */

    std::cout << YELLOW_BOLD_TEXT << "[INFO] ATTEMPTING TAKEOFF" << RESET_TEXT << std::endl;

    // Setting takeoff altitude 
    quadlink::QuadAction::action_set_takeoff_height(target_height);

    if (quadlink::QuadTelemetry::check_is_armed() == quadlink::VehicleStatus::Armed)
    {
        std::cout << YELLOW_BOLD_TEXT << "[INFO] TAKING OFF..." << RESET_TEXT << std::endl;
        
        quadlink::ConnectionStatus takeoff_status = quadlink::QuadAction::action_takeoff();

        switch (takeoff_status){
            case (quadlink::ConnectionStatus::Finished):
                if (blocking){
                    /*
                        TODO: Method only stops after reaching the altitude 
                    */
                }
                else{
                    std::cout << GREEN_BOLD_TEXT << "[INFO] TAKEOFF SUCCESSFULL" << RESET_TEXT << std::endl;
                    return quadlink::ConnectionStatus::Finished;
                }
            case (quadlink::ConnectionStatus::Failed):
                std::cerr << RED_BOLD_TEXT << "[INFO] TAKEOFF FAILED" << RESET_TEXT << std::endl;
                return quadlink::ConnectionStatus::Failed;
            case (quadlink::ConnectionStatus::Timeout):
                std::cerr << RED_BOLD_TEXT << "[TIMEOUT] TAKEOFF TIMED OUT" << RESET_TEXT << std::endl;    
                return quadlink::ConnectionStatus::Timeout;
        }
    }
    else
    {
        std::cout << RED_BOLD_TEXT << "[ERROR] " << quadlink::UAV::vehicle_name << " NOT ARMED" << RESET_TEXT << std::endl;
        return quadlink::ConnectionStatus::Failed;
    }
}

quadlink::ConnectionStatus UAV::land(bool check)
{
    // if (check)
    // {
    //     if (UAV::telemetry->armed())
    //     {
    //         return quadlink::ConnectionStatus::IN_PROGRESS;
    //     }
    //     else
    //     {
    //         std::cout << GREEN_BOLD_TEXT << "[INFO]" << UAV::vehicle_name << " landed successfully" << RESET_TEXT << std::endl;
    //         return quadlink::ConnectionStatus::FINISHED;
    //     }
    // }

    // std::cout << YELLOW_BOLD_TEXT << "[INFO] Landing..." << RESET_TEXT << std::endl;
    // const mavsdk::Action::Result landing = UAV::action->land();

    // if (landing == mavsdk::Action::Result::Success)
    // {
    //     return quadlink::ConnectionStatus::IN_PROGRESS;
    // }
    // else
    // {
    //     std::cerr << "Landing failed" << std::endl;
    //     return quadlink::ConnectionStatus::FAILED;
    // }
}
}
