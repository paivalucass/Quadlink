#include "quad_telemetry.h"

namespace quadlink{

quadlink::QuadTelemetry::QuadTelemetry() : msg_factory(std::make_shared<quadlink::MessageFactory>()){   
}

quadlink::SensorStatus quadlink::QuadTelemetry::sensor_health(mavlink_sys_status_t sys, uint8_t sensor){
    /*
        Perfoms a bitmasp AND operation to verify sensor status
    */
    quadlink::SensorStatus status;
    status = quadlink::SensorStatus::Absent;
    if (sys.onboard_control_sensors_present & sensor) {
        status = quadlink::SensorStatus::Disabled;
        if (sys.onboard_control_sensors_enabled & sensor){
            if (sys.onboard_control_sensors_health & sensor) {
                status = quadlink::SensorStatus::Healthy;
            } else {
                status = quadlink::SensorStatus::Unhealthy;
            }
        }
    }
    return status;
}

std::string quadlink::QuadTelemetry::parse_sensor_status(quadlink::SensorStatus status){
    if (status == quadlink::SensorStatus::Absent){
        return "ABSENT";
    }
    if (status == quadlink::SensorStatus::Disabled){
        return "DISABLED";
    }
    if (status == quadlink::SensorStatus::Healthy){
        return "HEALTHY";
    }
    if (status == quadlink::SensorStatus::Unhealthy){
        return "UNHEALTHY";
    }
}

quadlink::SensorStatus quadlink::QuadTelemetry::check_sensors_status(){
    /*
        SHOULD THIS METHOD BE SPECIFIC? 
        FUTURE CHANGES SHOULD HAPPEN HERE.
    */
    // TODO: BEING ABLE TO PICK WHICH SENSORS ARE USED FOR CHECK. 
    std::string status_name;
    quadlink::MessageStatus status = quadlink::QuadConnector::wait_message(MAVLINK_MSG_ID_SYS_STATUS, 5);
    quadlink::QuadTelemetry::sensors.gyro = quadlink::QuadTelemetry::sensor_health(status.sys, MAV_SYS_STATUS_SENSOR_3D_GYRO);
    quadlink::QuadTelemetry::sensors.accel = quadlink::QuadTelemetry::sensor_health(status.sys, MAV_SYS_STATUS_SENSOR_3D_ACCEL);
    quadlink::QuadTelemetry::sensors.mag = quadlink::QuadTelemetry::sensor_health(status.sys, MAV_SYS_STATUS_SENSOR_3D_MAG);
    quadlink::QuadTelemetry::sensors.gps = quadlink::QuadTelemetry::sensor_health(status.sys, MAV_SYS_STATUS_SENSOR_GPS);

    if (quadlink::QuadTelemetry::sensors.gyro != quadlink::SensorStatus::Healthy)
    {
        std::cout << RED_BOLD_TEXT << "[INFO] GYRO " << quadlink::QuadTelemetry::parse_sensor_status(quadlink::QuadTelemetry::sensors.gyro) << RESET_TEXT << std::endl;
        return quadlink::SensorStatus::Unhealthy;
    }
    if (quadlink::QuadTelemetry::sensors.accel != quadlink::SensorStatus::Healthy)
    {
        std::cout << RED_BOLD_TEXT << "[INFO] ACCELEROMETER " << quadlink::QuadTelemetry::parse_sensor_status(quadlink::QuadTelemetry::sensors.accel) << RESET_TEXT << std::endl;
        return quadlink::SensorStatus::Unhealthy;
    }
    if (quadlink::QuadTelemetry::sensors.mag != quadlink::SensorStatus::Healthy)
    {
        std::cout << RED_BOLD_TEXT << "[INFO] MAGNETOMETER " << quadlink::QuadTelemetry::parse_sensor_status(quadlink::QuadTelemetry::sensors.mag) << RESET_TEXT << std::endl;
        return quadlink::SensorStatus::Unhealthy;
    }
    if (quadlink::QuadTelemetry::sensors.gps != quadlink::SensorStatus::Healthy)
    {
        std::cout << RED_BOLD_TEXT << "[INFO] GPS " << quadlink::QuadTelemetry::parse_sensor_status(quadlink::QuadTelemetry::sensors.gps) << RESET_TEXT << std::endl;
        return quadlink::SensorStatus::Unhealthy;
    }

    return quadlink::SensorStatus::Healthy;
}


quadlink::VehicleStatus quadlink::QuadTelemetry::check_is_armed(){
    quadlink::MessageStatus status = quadlink::QuadConnector::wait_message(MAVLINK_MSG_ID_HEARTBEAT, 5);
    /*
        AND operation to verify the armed flag
    */
    if (status.heartbeat.base_mode & MAV_MODE_FLAG_SAFETY_ARMED){
        return quadlink::VehicleStatus::Armed;
    }
    return quadlink::VehicleStatus::Disarmed;
}

mavlink_local_position_ned_t quadlink::QuadTelemetry::check_local_position(){
    quadlink::MessageStatus status = quadlink::QuadConnector::wait_message(MAVLINK_MSG_ID_LOCAL_POSITION_NED, 2);
    
    return status.local_position;
}
}