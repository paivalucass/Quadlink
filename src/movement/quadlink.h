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
        FINISHED,
        TIMEOUT
    };

    /*
        This class should be adaptable to all vehicles.
    */
    class UAV {
    public:
        // Constructor of the UAV class
        UAV(Vehicle vehicle_type);

        // Destructor (currently default but should be created futher on when dynamic allocation is used!)
        ~UAV() = default;

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
        std::shared_ptr<mavsdk::Action> action;
        std::shared_ptr<mavsdk::Telemetry> telemetry;
    };
}

