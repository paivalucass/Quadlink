// quadlink.h
#pragma once

#include <string>
#include <memory>
#include "quadlink.h"
#include "../connection/quad_connection.h"
#include "../telemetry/quad_telemetry.h"
#include "../action/quad_action.h"
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

namespace quadlink {

    /**
     * @brief Enum class for different types of vehicles.
     */
    enum class Vehicle {
        QuadCopter
    };

    /**
     * @brief Enum class for different flight statuses.
     */
    enum class FlightStatus {
        IN_PROGRESS,
        FAILED,
        FINISHED,
        TIMEOUT
    };

    /**
     * @brief A class representing a UAV, adaptable to all vehicles.
     */
    class UAV : public QuadTelemetry, public QuadAction{
    public:
        /**
         * @brief Constructor of class UAV.
         * @param vehicle_type type of the UAV being used quadlink::Vehicle e.g Quadcopter.
         */
        UAV(Vehicle vehicle_type);

        /**
         * @brief Destructor of class UAV.
         */
        ~UAV() = default;

        /**
         * @brief Connect to the drone via UDP, TCP or serial.
         * @param connection_url The URL for the connection e.g 127.0.0.1:14568.
         * @return The quadlink::FlightStatus representing the connection. 
         */
        FlightStatus connect(std::string& connection_url);

        /**
         * @brief Perform pre-flight check and arm the drone.
         * @return The quadlink::FlightStatus representing the arming.
         */
        FlightStatus arm();

        /**
         * @brief Perform takeoff to a target height.
         * @param target_height The target height for the takeoff.
         * @return The quadlink::FlightStatus representing the takeoff.
         */
        FlightStatus takeoff(double target_height);

        /**
         * @brief Perform landing.
         * @param check A boolean to check if landing is possible.
         * @return The quadlink::FlightStatus representing the landing.
         */
        FlightStatus land(bool check);

        /**
         * @brief Go to a pre-defined position.
         * @param x The x-coordinate of the position.
         * @param y The y-coordinate of the position.
         * @param z The z-coordinate of the position.
         * @return The quadlink::FlightStatus representing the go to.
         */
        FlightStatus go_to_relative(double x, double y, double z);

        /**
         * @brief Obtain the battery status of the drone.
         */
        void get_battery_status();

    private:
        Vehicle vehicle_type;
        std::string vehicle_name;
        std::string connection_url;
        std::shared_ptr<mavsdk::Action> action;
        std::shared_ptr<mavsdk::Telemetry> telemetry;
    };
}