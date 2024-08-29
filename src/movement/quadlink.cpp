#include "quadlink.h"
#include "../connection/quad_connection.h"
#include "format.h"
#include <iostream>
#include <mavsdk/mavsdk.h>
#include <mavlink/common/mavlink.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <thread>
#include <chrono>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cmath>

namespace quadlink{

UAV::UAV(Vehicle vehicle_type) : vehicle_type(vehicle_type), connection()  {
    
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

    quadlink::ConnectionStatus check = quadlink::UAV::connection.connect_udp(connection_url);
    
    if (check == quadlink::ConnectionStatus::Success)
    {
        std::cout << GREEN_BOLD_TEXT << "[INFO] " << UAV::vehicle_name << " CONNECTED SUCCESSFULLY" << RESET_TEXT << std::endl;
        return quadlink::FlightStatus::FINISHED;
    }
    else if (check == quadlink::ConnectionStatus::Failed)
    {
        std::cerr << "[INFO] " << UAV::vehicle_name << " FAILED CONNECTING" << RESET_TEXT << std::endl;
        return quadlink::FlightStatus::FAILED;
    }
    else
    {
        std::cout << RED_BOLD_TEXT << "[INFO] " << UAV::vehicle_name << " CONNECTION TIMEOUT" << RESET_TEXT << std::endl;
        return quadlink::FlightStatus::TIMEOUT;
    }
}

FlightStatus UAV::arm()
{
    // TODO: Verify calibrations before performing arm

    std::cout << YELLOW_BOLD_TEXT << "[INFO] PERFORMING PRE FLIGHT CHECK" << RESET_TEXT << std::endl;

    while (!UAV::telemetry->health_all_ok())
    {
        std::cout << RED_BOLD_TEXT << "Vehicle not ready to arm. Waiting on:" << RESET_TEXT << '\n';

        if (!UAV::telemetry->health().is_global_position_ok) {
            std::cerr << "-> GPS health." << std::endl;
        }
        if (!UAV::telemetry->health().is_home_position_ok){
            std::cerr << "-> Home position to be set" << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    std::cout << YELLOW_BOLD_TEXT << "[INFO] ARMING..." << RESET_TEXT << std::endl;

    const mavsdk::Action::Result armed = UAV::action->arm();

    std::this_thread::sleep_for(std::chrono::seconds(3));

    if (armed != mavsdk::Action::Result::Success)
    {
        std::cerr << "Failed Arming" << armed << std::endl;
        return FlightStatus::FAILED;
    }

    else
    {
        std::cout << GREEN_BOLD_TEXT << "[INFO] " << UAV::vehicle_name << " ARMED SUCCESSFULLY" << RESET_TEXT << std::endl;
        return FlightStatus::FINISHED;
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
