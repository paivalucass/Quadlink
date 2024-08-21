#include <quadlink.h>
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

bool QuadCopter::connect(const std::string& connection_url){
    /*
        Connection URL only tested with UDP for now 
        e.g udpin://127.0.0.1:14562 
    */

    //Connect via string
    cout << "[INFO] CONNECTING TO QUADCOPTER" << endl;

    mavsdk::ConnectionResult connection_result = mavsdk.add_any_connection(connection_url);

    if (connection_result != mavsdk::ConnectionResult::Success) {
        cerr << "Connection Failed" << endl;
        return false;
    }

    //Find Quadcopter in system array
    cout << "[INFO] WAITING FOR SYSTEM..." << endl;

    while (mavsdk.systems().size() == 0){
        this_thread::sleep_for(chrono::seconds(1));
    }

    auto system = mavsdk.systems().at(0);
    
    //Declare pluguin variables if connection succeeded
    if (!system->is_connected()){
        cerr << "Connection to QuadCopter Failed" << endl;
        return false;
    }

    else{
        cout << "[INFO] CONNECTED TO QUADCOPTER" << endl;
        QuadCopter::action = std::make_shared<mavsdk::Action>(system);
        QuadCopter::telemetry = std::make_shared<mavsdk::Telemetry>(system);
        return true;
    }
}

void QuadCopter::takeoff(double height){

}
