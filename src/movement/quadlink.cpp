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

using namespace quadlink;
using namespace std; 

UAV::UAV(const mavsdk::Mavsdk::Configuration& config, Vehicle Vehicle_type) : mavsdk(config), vehicle_type(vehicle_type)  {
    
    switch (vehicle_type)
    {
    case Vehicle::QuadCopter:
        vehicle_name = "QUADCOPTER";
        // More vehicle specific variables should be initialized here
        break;
    // TODO: Add more vehicle support in the future
    default:
        break;
    }

}

FlightStatus UAV::connect(const std::string& connection_url)
{
    /*
        Connection URL only tested with UDP for now 
        e.g udpin://127.0.0.1:14568 
    */

    //Connect via string
    cout << YELLOW_BOLD_TEXT << "[INFO] CONNECTING TO " << UAV::vehicle_name << RESET_TEXT << endl;

    mavsdk::ConnectionResult connection_result = mavsdk.add_any_connection(connection_url);

    if (connection_result != mavsdk::ConnectionResult::Success) 
    {
        cerr << "Connection Failed" << endl;
        return FlightStatus::FAILED;
    }

    //Find UAV in system array
    cout << YELLOW_BOLD_TEXT << "[INFO] WAITING FOR SYSTEM..." << RESET_TEXT << endl;

    while (mavsdk.systems().size() == 0)
    {
        this_thread::sleep_for(chrono::seconds(1));
    }

    auto system = mavsdk.systems().at(0);
    
    //Declare pluguin variables if connection succeeded
    if (!system->is_connected())
    {
        cerr << "Connection to " << UAV::vehicle_name << " Failed" << endl;
        return FlightStatus::FAILED;
    }

    else
    {
        // UAV::system_id = system->get_system_id();
        // Component ID is different depending on the message, a way to handle this will be added soon.
        // UAV::target_system_id = system->get_system_id();
        // Target component ID is different depending on the message, a way to handle this will be added soon.
        cout << GREEN_BOLD_TEXT << "[INFO] CONNECTED TO "<< UAV::vehicle_name << RESET_TEXT << endl;
        UAV::action = std::make_shared<mavsdk::Action>(system);
        UAV::telemetry = std::make_shared<mavsdk::Telemetry>(system);
        return FlightStatus::FINISHED;
    }
}

FlightStatus UAV::arm()
{
    // TODO: Verify calibrations before performing arm

    cout << YELLOW_BOLD_TEXT << "[INFO] PERFORMING PRE FLIGHT CHECK" << RESET_TEXT << endl;

    while (!UAV::telemetry->health_all_ok())
    {
        cout << RED_BOLD_TEXT << "Vehicle not ready to arm. Waiting on:" << RESET_TEXT << '\n';

        if (!UAV::telemetry->health().is_global_position_ok) {
            cerr << "-> GPS health." << endl;
        }
        if (!UAV::telemetry->health().is_home_position_ok){
            cerr << "-> Home position to be set" << endl;
        }
        this_thread::sleep_for(std::chrono::seconds(1));
    }

    cout << YELLOW_BOLD_TEXT << "[INFO] ARMING..." << RESET_TEXT << endl;

    const mavsdk::Action::Result armed = UAV::action->arm();

    this_thread::sleep_for(chrono::seconds(3));

    if (armed != mavsdk::Action::Result::Success)
    {
        cerr << "Failed Arming" << armed << endl;
        return FlightStatus::FAILED;
    }

    else
    {
        cout << GREEN_BOLD_TEXT << "[INFO] " << UAV::vehicle_name << " ARMED SUCCESSFULLY" << RESET_TEXT << endl;
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
        cout << YELLOW_BOLD_TEXT << "[INFO] TAKING OFF..." << RESET_TEXT << endl;
        const mavsdk::Action::Result flying = UAV::action->takeoff();

        if (flying != mavsdk::Action::Result::Success)
        {
            cerr << "Takeoff Failed" << flying << endl;
            return FlightStatus::FAILED;
        }
        else
        {
            /*
                This guarantees that the takeoff will be finished before leaving the method
            */
            while (UAV::telemetry->position().relative_altitude_m < target_height)
            {
                this_thread::sleep_for(chrono::seconds(1));
            }
            cout << GREEN_BOLD_TEXT << "[INFO]" << UAV::vehicle_name << " finished climbing" << RESET_TEXT << endl;
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
            cout << GREEN_BOLD_TEXT << "[INFO]" << UAV::vehicle_name << " landed successfully" << RESET_TEXT << endl;
            return FlightStatus::FINISHED;
        }
    }

    cout << YELLOW_BOLD_TEXT << "[INFO] Landing..." << RESET_TEXT << endl;
    const mavsdk::Action::Result landing = UAV::action->land();

    if (landing == mavsdk::Action::Result::Success)
    {
        return FlightStatus::IN_PROGRESS;
    }
    else
    {
        cerr << "Landing failed" << endl;
        return FlightStatus::FAILED;
    }
}
