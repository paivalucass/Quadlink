#include "communication/controller_pixhawk_ardupilot.hpp"

namespace rcpilot{

rcpilot::ControllerPixhawkArdupilot::ControllerPixhawkArdupilot() : pixhawk_connector() {
}

rcpilot::ConnectionStatus rcpilot::ControllerPixhawkArdupilot::connect(std::string& connection_string){
    return pixhawk_connector.action_connect(connection_string);
}

rcpilot::ConnectionStatus rcpilot::ControllerPixhawkArdupilot::arm(){
    
    // if (pixhawk_connector.arm_check() == rcpilot::ConnectionStatus::Finished){
    //     RCLCPP_WARN(rclcpp::get_logger("communication_node"), "VEHICLE ALREADY ARMED");            
    //     return rcpilot::ConnectionStatus::Failed;
    // }

    // check sensors before arming

    this->set_home_position();

    return pixhawk_connector.action_arm();
}

rcpilot::ConnectionStatus rcpilot::ControllerPixhawkArdupilot::set_home_position(){
    return pixhawk_connector.action_set_home_position();
}

rcpilot::ConnectionStatus rcpilot::ControllerPixhawkArdupilot::change_mode(int mode){
    
    if (isValidFlightMode(mode)) {
        // Verify if flight mode is already the given one
        rcpilot::ArdupilotFlightMode flight_mode = static_cast<rcpilot::ArdupilotFlightMode>(mode);
        return pixhawk_connector.action_change_mode(flight_mode);
    }
    else{
        // TODO: handle this better
        return rcpilot::ConnectionStatus::Failed;
    }
}

rcpilot::ConnectionStatus rcpilot::ControllerPixhawkArdupilot::takeoff(){
    if (pixhawk_connector.arm_check() == rcpilot::ConnectionStatus::Failed){
        this->arm(); // Dangerous?
    }

    return pixhawk_connector.action_takeoff();
}

rcpilot::ConnectionStatus rcpilot::ControllerPixhawkArdupilot::land(){
    // If you want to know if it landed, maybe add an arm check

    return pixhawk_connector.action_land();
}

rcpilot::ConnectionStatus rcpilot::ControllerPixhawkArdupilot::change_body_velocity(float x, float y, float z){
    float* position = new float[12] {0,0,0,x,y,z,0,0,0,0,0,0};

    rcpilot::ConnectionStatus status = pixhawk_connector.set_position_target_body_velocity(position);

    delete[] position;

    return status;
}

rcpilot::ConnectionStatus rcpilot::ControllerPixhawkArdupilot::goto_body(float x, float y, float z){
    float* position = new float[12] {x,y,z,0,0,0,0,0,0,0,0,0};

    rcpilot::ConnectionStatus status = pixhawk_connector.set_position_target_body_position(position);

    delete[] position;

    return status;
}

rcpilot::ConnectionStatus rcpilot::ControllerPixhawkArdupilot::goto_ned(float x, float y, float z){
    float* position = new float[12] {x,y,z,0,0,0,0,0,0,0,0,0};

    rcpilot::ConnectionStatus status = pixhawk_connector.set_position_target_local_ned(position);

    delete[] position;

    return status;
}

rcpilot::ConnectionStatus rcpilot::ControllerPixhawkArdupilot::yaw(float yaw){

    float yaw_rad = (yaw * M_PI) / 180;

    float* position = new float[12] {0,0,0,0,0,0,0,0,0,0,yaw_rad,0};

    rcpilot::ConnectionStatus status = pixhawk_connector.set_yaw(position);

    delete[] position;

    return status;
}

rcpilot::ConnectionStatus rcpilot::ControllerPixhawkArdupilot::set_yaw_rate(float yaw_rate){

    float yaw_rate_rad = (yaw_rate * M_PI) / 180;

    float* position = new float[12] {0,0,0,0,0,0,0,0,0,0,0,yaw_rate_rad};

    rcpilot::ConnectionStatus status = pixhawk_connector.set_yaw(position);

    delete[] position;

    return status;
}

void rcpilot::ControllerPixhawkArdupilot::set_takeoff_height(float height){
    pixhawk_connector.action_set_takeoff_height(height);
}

bool rcpilot::ControllerPixhawkArdupilot::isValidFlightMode(int mode_int) {
    switch (static_cast<rcpilot::ArdupilotFlightMode>(mode_int)) {
        case rcpilot::ArdupilotFlightMode::STABILIZE:
        case rcpilot::ArdupilotFlightMode::GUIDED:
        case rcpilot::ArdupilotFlightMode::LOITER:
        case rcpilot::ArdupilotFlightMode::LAND:
            return true;
        default:
            return false;
    }
}

}

