// quadlink.h
#pragma once

#include <string>
#include <memory>
#include <mavsdk/mavsdk.h>
#include <mavlink/common/mavlink.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include "../connection/quad_connection.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cmath>

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

        ~UAV();

        // Connect the drone via UDP, TCP, Serial
        FlightStatus connect(std::string& connection_url);

        // Do pre flight check and perform arm
        FlightStatus arm();

        // Perform takeoff
        FlightStatus takeoff(double target_height);

        // Perform land
        FlightStatus land(bool check);

        // Go to a pre defined position
        FlightStatus go_to_relative(double x, double y, double z);

        // Obtain battery level
        void get_battery_status();

    private:
        quadlink::QuadConnector connection;
        Vehicle vehicle_type;
        std::string vehicle_name;
        std::string connection_url;
        mavsdk::Mavsdk mavsdk;
        std::shared_ptr<mavsdk::Action> action;
        std::shared_ptr<mavsdk::Telemetry> telemetry;
    };
}

