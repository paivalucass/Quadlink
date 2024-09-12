#include "quad_action.h"


namespace quadlink{

quadlink::QuadAction::QuadAction() : msg_factory(std::make_shared<quadlink::MessageFactory>()){
}


quadlink::ConnectionStatus quadlink::QuadAction::action_arm(){

    mavlink_command_long_t arm_command = msg_factory->create_arm_command();

    mavlink_message_t msg = this->build_command(arm_command);

    return quadlink::QuadConnector::send_mav_message(msg);
}

quadlink::ConnectionStatus quadlink::QuadAction::action_change_mode(ArdupilotFlightMode flight_mode){

    mavlink_command_long_t mode_command = msg_factory->create_mode_command(flight_mode);

    mavlink_message_t msg = this->build_command(mode_command);

    return quadlink::QuadConnector::send_mav_message(msg);
}

quadlink::ConnectionStatus quadlink::QuadAction::action_set_home_position(){

    mavlink_command_long_t home_command = msg_factory->create_set_home_command();

    mavlink_message_t msg = this->build_command(home_command);

    return quadlink::QuadConnector::send_mav_message(msg);
}

quadlink::ConnectionStatus quadlink::QuadAction::action_takeoff(){
    
    mavlink_command_long_t takeoff_command = msg_factory->create_takeoff_command(quadlink::QuadAction::_takeoff_height);

    mavlink_message_t msg = this->build_command(takeoff_command);

    return quadlink::QuadConnector::send_mav_message(msg);
}

quadlink::ConnectionStatus quadlink::QuadAction::action_land(){
    // TODO: Implement action land (actually needed?)
}

quadlink::ConnectionStatus quadlink::QuadAction::action_set_position_target_local_ned(uint16_t type_mask, float x, float y, float z, float vx = 0, float vy = 0, float vz = 0, float ax = 0, float ay = 0, float az = 0, float yaw = 0, float yaw_rate = 0){

}

void quadlink::QuadAction::action_set_takeoff_height(float height){

    quadlink::QuadAction::_takeoff_height = height;
}




}

