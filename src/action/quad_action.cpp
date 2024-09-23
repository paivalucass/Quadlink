#include "quad_action.h"


namespace quadlink{

quadlink::QuadAction::QuadAction() : msg_factory(std::make_shared<quadlink::MessageFactory>()){
}


quadlink::ConnectionStatus quadlink::QuadAction::action_arm(){

    mavlink_command_long_t arm_command = msg_factory->create_arm_command();

    mavlink_message_t msg = this->build_command(arm_command);

    return quadlink::QuadConnector::send_mav_message(msg, quadlink::EncodeType::COMMAND_LONG);
}

quadlink::ConnectionStatus quadlink::QuadAction::action_change_mode(ArdupilotFlightMode flight_mode){

    mavlink_command_long_t mode_command = msg_factory->create_mode_command(flight_mode);

    mavlink_message_t msg = this->build_command(mode_command);

    return quadlink::QuadConnector::send_mav_message(msg, quadlink::EncodeType::COMMAND_LONG);
}

quadlink::ConnectionStatus quadlink::QuadAction::action_set_home_position(){

    mavlink_command_long_t home_command = msg_factory->create_set_home_command();

    mavlink_message_t msg = this->build_command(home_command);

    return quadlink::QuadConnector::send_mav_message(msg, quadlink::EncodeType::COMMAND_LONG);
}

quadlink::ConnectionStatus quadlink::QuadAction::action_takeoff(){
    
    mavlink_command_long_t takeoff_command = msg_factory->create_takeoff_command(quadlink::QuadAction::_takeoff_height);

    mavlink_message_t msg = this->build_command(takeoff_command);

    return quadlink::QuadConnector::send_mav_message(msg, quadlink::EncodeType::COMMAND_LONG);
}

quadlink::ConnectionStatus quadlink::QuadAction::action_land(){
    // TODO: Implement action land (actually needed?)
}

quadlink::ConnectionStatus quadlink::QuadAction::action_set_position_target_local_ned(float* position){

    mavlink_set_position_target_local_ned_t target_local_ned_command = msg_factory->create_set_local_ned_command(position);

    mavlink_message_t msg = this->build_command(target_local_ned_command);                                                                                  

    return quadlink::QuadConnector::send_mav_message(msg, quadlink::EncodeType::POSITION_TARGET_LOCAL_NED);
}

quadlink::ConnectionStatus quadlink::QuadAction::action_set_position_target_body(float* position){

    mavlink_set_position_target_local_ned_t target_body_command = msg_factory->create_set_body_command(position);

    mavlink_message_t msg = this->build_command(target_body_command);                                                                                  

    return quadlink::QuadConnector::send_mav_message(msg, quadlink::EncodeType::POSITION_TARGET_BODY);
}

void quadlink::QuadAction::action_set_takeoff_height(float height){

    quadlink::QuadAction::_takeoff_height = height;
}
}

