#include <string>
#include <memory>
#include <mavlink/common/mavlink.h>
#include "../connection/message_factory.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cmath>
#include <iostream>
#include <chrono>
#include <cstring>
#include "../connection/quad_connection.h"


namespace quadlink{
    /**
     * @brief Enum class for different sensor statuses.
     */
    enum class SensorStatus{
        Present,
        Absent,
        Enabled,
        Disabled,
        Healthy,
        Unhealthy
    };

    /**
     * @brief Struct for sensors to store their statuses.
     */
    typedef struct {
        SensorStatus gyro;
        SensorStatus accel;
        SensorStatus mag;
        SensorStatus gps;
    } Sensors;

    /**
     * @brief A class for quadcopter telemetry.
     */
    class QuadTelemetry : public virtual quadlink::QuadConnector{
        public:
            /**
             * @brief Constructor for QuadTelemetry.
             */
            QuadTelemetry();

            /**
             * @brief Destructor for QuadTelemetry.
             */
            ~QuadTelemetry() = default;

            /**
             * @brief Checks the health of the main sensors.
             * This method checks via bitmap the health of GYRO, ACCELEROMETER, MAGNETOMETER, GPS.
             * @return The quadlink::Sensors status of the sensors.
             */
            quadlink::Sensors check_sensors_health();

            /**
             * @brief Performs a bitmask operation AND to verify sensor status.
             * @param sys The system info.
             * @param sensor The sensor to check.
             * @return The quadlink::SensorStatus status of the sensor.
             */
            quadlink::SensorStatus sensor_health(mavlink_sys_status_t sys, uint8_t sensor);

        private:
            std::shared_ptr<quadlink::MessageFactory> msg_factory;
    };
}