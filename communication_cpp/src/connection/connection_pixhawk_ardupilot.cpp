#include "communication/connection_pixhawk_ardupilot.hpp"


namespace rcpilot{

rcpilot::ConnectionPixHawkArdupilot::ConnectionPixHawkArdupilot(){
}

rcpilot::ConnectionStatus rcpilot::ConnectionPixHawkArdupilot::action_connect(std::string& connection_url){

    return rcpilot::Connector::connect_udp(connection_url);
}

rcpilot::ConnectionStatus rcpilot::ConnectionPixHawkArdupilot::action_arm(){

    mavlink_command_long_t arm_command = rcpilot::create_arm_command();

    mavlink_message_t msg = this->build_command(arm_command);

    return rcpilot::Connector::send_mav_message(msg, rcpilot::EncodeType::COMMAND_LONG);
}

rcpilot::ConnectionStatus rcpilot::ConnectionPixHawkArdupilot::action_change_mode(ArdupilotFlightMode flight_mode){

    mavlink_command_long_t mode_command = rcpilot::create_mode_command(flight_mode);

    mavlink_message_t msg = this->build_command(mode_command);

    return rcpilot::Connector::send_mav_message(msg, rcpilot::EncodeType::COMMAND_LONG);
}

rcpilot::ConnectionStatus rcpilot::ConnectionPixHawkArdupilot::action_set_home_position(){

    mavlink_command_long_t home_command = rcpilot::create_set_home_command();

    mavlink_message_t msg = this->build_command(home_command);

    return rcpilot::Connector::send_mav_message(msg, rcpilot::EncodeType::COMMAND_LONG);
}

rcpilot::ConnectionStatus rcpilot::ConnectionPixHawkArdupilot::action_takeoff(){
    
    mavlink_command_long_t takeoff_command = rcpilot::create_takeoff_command(rcpilot::ConnectionPixHawkArdupilot::_takeoff_height);

    mavlink_message_t msg = this->build_command(takeoff_command);

    return rcpilot::Connector::send_mav_message(msg, rcpilot::EncodeType::COMMAND_LONG);
}

rcpilot::ConnectionStatus rcpilot::ConnectionPixHawkArdupilot::action_land(){

    mavlink_command_long_t mode_command = rcpilot::create_mode_command(ArdupilotFlightMode::LAND);

    mavlink_message_t msg = this->build_command(mode_command);

    return rcpilot::Connector::send_mav_message(msg, rcpilot::EncodeType::COMMAND_LONG);
}

void rcpilot::ConnectionPixHawkArdupilot::action_set_takeoff_height(float height){

    rcpilot::ConnectionPixHawkArdupilot::_takeoff_height = height;
}

rcpilot::ConnectionStatus rcpilot::ConnectionPixHawkArdupilot::set_position_target_local_ned(float* position){

    mavlink_set_position_target_local_ned_t target_local_ned_command = rcpilot::create_set_local_ned_command(position);

    mavlink_message_t msg = this->build_command(target_local_ned_command);                                                                                  

    return rcpilot::Connector::send_mav_message(msg, rcpilot::EncodeType::POSITION_TARGET_LOCAL_NED);
}

rcpilot::ConnectionStatus rcpilot::ConnectionPixHawkArdupilot::set_position_target_body_velocity(float* position){

    mavlink_set_position_target_local_ned_t target_body_command = rcpilot::create_set_body_command_velocity(position);

    mavlink_message_t msg = this->build_command(target_body_command);                                                                                  

    return rcpilot::Connector::send_mav_message(msg, rcpilot::EncodeType::POSITION_TARGET_BODY);
}

rcpilot::ConnectionStatus rcpilot::ConnectionPixHawkArdupilot::set_position_target_body_position(float* position){

    mavlink_set_position_target_local_ned_t target_body_command = rcpilot::create_set_body_command_position(position);

    mavlink_message_t msg = this->build_command(target_body_command);                                                                                  

    return rcpilot::Connector::send_mav_message(msg, rcpilot::EncodeType::POSITION_TARGET_BODY);
}

rcpilot::ConnectionStatus rcpilot::ConnectionPixHawkArdupilot::set_yaw(float* position){

    mavlink_set_position_target_local_ned_t target_body_command = rcpilot::create_set_yaw(position);

    mavlink_message_t msg = this->build_command(target_body_command);                                                                                  

    return rcpilot::Connector::send_mav_message(msg, rcpilot::EncodeType::POSITION_TARGET_BODY);
}

rcpilot::ConnectionStatus rcpilot::ConnectionPixHawkArdupilot::set_yaw_rate(float* position){

    mavlink_set_position_target_local_ned_t target_body_command = rcpilot::create_set_yaw_rate(position);

    mavlink_message_t msg = this->build_command(target_body_command);                                                                                  

    return rcpilot::Connector::send_mav_message(msg, rcpilot::EncodeType::POSITION_TARGET_BODY);
}


rcpilot::ConnectionStatus rcpilot::ConnectionPixHawkArdupilot::arm_check(){
    rcpilot::MessageStatus msg = rcpilot::Connector::wait_message(MAVLINK_MSG_ID_HEARTBEAT, 5);

    if (msg.heartbeat.base_mode & MAV_MODE_FLAG_SAFETY_ARMED){
        return rcpilot::ConnectionStatus::Finished;
    }
    return rcpilot::ConnectionStatus::Failed;

}

}

