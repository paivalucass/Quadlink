#include "quad_telemetry.h"

namespace quadlink{

quadlink::QuadTelemetry::QuadTelemetry() : msg_factory(std::make_shared<quadlink::MessageFactory>()){   
}

// TODO: Only one AND method for both sys_status and heartbeat
quadlink::SensorStatus quadlink::QuadTelemetry::sensor_health(mavlink_sys_status_t sys, uint8_t sensor){
    /*
        Perfoms a bitmasp AND operation to verify sensor status
    */
    quadlink::SensorStatus status;
    if (sys.onboard_control_sensors_present & sensor) {
        status = quadlink::SensorStatus::Present;
        if (sys.onboard_control_sensors_enabled & sensor){
            status = quadlink::SensorStatus::Enabled;
            if (sys.onboard_control_sensors_health & sensor) {
                status = quadlink::SensorStatus::Helthy;
            } else {
                status = quadlink::SensorStatus::Unhelthy;
            }
        }
    }
    return status;
}

quadlink::Sensors quadlink::QuadTelemetry::check_sensors_health(){
    quadlink::Sensors sensors_status;
    quadlink::MessageStatus status = this->wait_message(MAVLINK_MSG_ID_SYS_STATUS, 5);
    sensors_status.gyro = quadlink::QuadTelemetry::sensor_health(status.sys, MAV_SYS_STATUS_SENSOR_3D_GYRO);
}               

}