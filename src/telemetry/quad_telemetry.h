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
    enum class SensorStatus{
        Present,
        Absent,
        Enabled,
        Disabled,
        Helthy,
        Unhelthy
    };

    typedef struct {
        SensorStatus gyro;
        SensorStatus accel;
        SensorStatus mag;
        SensorStatus gps;
    } Sensors;

    class QuadTelemetry : public virtual quadlink::QuadConnector{
        public:
            QuadTelemetry();

            ~QuadTelemetry() = default;

            quadlink::Sensors check_sensors_health();

            /*
            Perfoms a bitmask AND to verify sensor status
            */
            quadlink::SensorStatus sensor_health(mavlink_sys_status_t sys, uint8_t sensor);

        private:
            std::shared_ptr<quadlink::MessageFactory> msg_factory;
    };
}