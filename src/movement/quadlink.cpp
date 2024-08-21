#include "quadlink.h"
#include "format.h"
#include <iostream>
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <thread>
#include <chrono>

using namespace quadlink;
using namespace std; 

QuadCopter::QuadCopter(const mavsdk::Mavsdk::Configuration& config) : mavsdk(config) {
}

bool QuadCopter::connect(const std::string& connection_url)
{
    /*
        Connection URL only tested with UDP for now 
        e.g udpin://127.0.0.1:14562 
    */

    //Connect via string
    cout << YELLOW_BOLD_TEXT << "[INFO] CONNECTING TO " << QuadCopter::vehicle_type << RESET_TEXT << endl;

    mavsdk::ConnectionResult connection_result = mavsdk.add_any_connection(connection_url);

    if (connection_result != mavsdk::ConnectionResult::Success) 
    {
        cerr << "Connection Failed" << endl;
        return false;
    }

    //Find Quadcopter in system array
    cout << YELLOW_BOLD_TEXT << "[INFO] WAITING FOR SYSTEM..." << RESET_TEXT << endl;

    while (mavsdk.systems().size() == 0)
    {
        this_thread::sleep_for(chrono::seconds(1));
    }

    auto system = mavsdk.systems().at(0);
    
    //Declare pluguin variables if connection succeeded
    if (!system->is_connected())
    {
        cerr << "Connection to " << QuadCopter::vehicle_type << " Failed" << endl;
        return false;
    }

    else
    {
        cout << GREEN_BOLD_TEXT << "[INFO] CONNECTED TO "<< QuadCopter::vehicle_type << RESET_TEXT << endl;
        QuadCopter::action = std::make_shared<mavsdk::Action>(system);
        QuadCopter::telemetry = std::make_shared<mavsdk::Telemetry>(system);
        return true;
    }
}

bool QuadCopter::arm()
{
    // TODO: Verify calibrations before performing arm

    cout << YELLOW_BOLD_TEXT << "[INFO] PERFORMING PRE FLIGHT CHECK" << RESET_TEXT << endl;

    while (!telemetry->health_all_ok())
    {

        cout << RED_BOLD_TEXT << "Vehicle not ready to arm. Waiting on:" << RESET_TEXT << '\n';

        if (!QuadCopter::telemetry->health().is_global_position_ok) {
            cerr << "-> GPS health." << endl;
        }
        if (!QuadCopter::telemetry->health().is_home_position_ok){
            cerr << "-> Home position to be set" << endl;
        }
        this_thread::sleep_for(std::chrono::seconds(1));
    }

    cout << YELLOW_BOLD_TEXT << "[INFO] ARMING..." << RESET_TEXT << endl;

    const mavsdk::Action::Result armed = QuadCopter::action->arm();

    this_thread::sleep_for(chrono::seconds(3));

    if (armed != mavsdk::Action::Result::Success)
    {
        cerr << "Failed Arming" << armed << endl;
        return false;
    }

    else
    {
        cout << GREEN_BOLD_TEXT << "[INFO] " << QuadCopter::vehicle_type << " ARMED SUCCESSFULLY" << RESET_TEXT << endl;
        return true;
    }
}

bool QuadCopter::takeoff(double target_height)
{
    /*
        Perform takeoff given a determined height
    */

    // Setting takeoff altitude 
    QuadCopter::action->set_takeoff_altitude(target_height);

    if (QuadCopter::arm())
    {
        cout << GREEN_BOLD_TEXT << "[INFO] TAKING OFF!" << RESET_TEXT << '\n';
        const mavsdk::Action::Result flying = QuadCopter::action->takeoff();

        if (flying != mavsdk::Action::Result::Success)
        {
            cerr << "Takeoff Failed" << flying << endl;
            return false;
        }
        else
        {
            while (QuadCopter::telemetry->position().relative_altitude_m < target_height)
            {
                this_thread::sleep_for(chrono::seconds(1));
            }
        }
    }
    else
    {
        return false;
    }
    return false;
}
