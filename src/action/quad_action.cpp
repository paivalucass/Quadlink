#include "quad_action.h"


quadlink::QuadAction::QuadAction() : msg_factory(std::make_shared<quadlink::MessageFactory>()){
}


quadlink::ConnectionStatus quadlink::QuadAction::action_arm(){

    mavlink_command_long_t arm_command = msg_factory->create_arm_command();

    return quadlink::QuadConnector::send_mav_message(arm_command);
}

quadlink::ConnectionStatus quadlink::QuadAction::action_change_mode(uint8_t mode){

    mavlink_command_long_t mode_command = msg_factory->create_mode_command(mode);

    return quadlink::QuadConnector::send_mav_message(mode_command);
}

