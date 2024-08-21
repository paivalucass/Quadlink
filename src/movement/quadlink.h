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

    class QuadCopter {
    public:
        // Constructor
        QuadCopter(const mavsdk::Mavsdk::Configuration& config);

        // Connect the drone via UDP, TCP, Serial
        bool connect(const std::string& connection_url);

        // Do pre flight check and perform arm
        bool arm();

        // Perform takeoff
        bool takeoff(double target_height);

        // Perform land
        void land();

        // Obtain battery level
        void get_battery_status();

    private:
        std::string vehicle_type = "QUADCOPTER";
        mavsdk::Mavsdk mavsdk;
        std::shared_ptr<mavsdk::Action> action;
        std::shared_ptr<mavsdk::Telemetry> telemetry;
    };
}

