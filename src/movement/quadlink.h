// quadlink.h
#pragma once

#include <string>
#include <memory>
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>

namespace quadlink {

    enum class Vehicle {
        QuadCopter
    };

    enum class FlightStatus {
        IN_PROGRESS,
        FAILED,
        FINISHED
    };

    /*
        This class should be adaptable to all vehicles.
    */
    class UAV {
    public:
        // Constructor of the UAV class
        UAV(const mavsdk::Mavsdk::Configuration& config, Vehicle vehicle_type);

        // Connect the drone via UDP, TCP, Serial
        FlightStatus connect(const std::string& connection_url);

        // Do pre flight check and perform arm
        FlightStatus arm();

        // Perform takeoff
        FlightStatus takeoff(double target_height);

        // Perform land
        FlightStatus land();

        // Obtain battery level
        void get_battery_status();

    private:
        Vehicle vehicle_type;
        std::string vehicle_name;
        mavsdk::Mavsdk mavsdk;
        std::shared_ptr<mavsdk::Action> action;
        std::shared_ptr<mavsdk::Telemetry> telemetry;
    };
}

