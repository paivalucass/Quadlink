#include "quad_action.h"


namespace quadlink{

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

quadlink::ConnectionStatus quadlink::QuadAction::action_set_home_position(){

    mavlink_command_long_t home_command = msg_factory->create_set_home_command();

    return quadlink::QuadConnector::send_mav_message(home_command);
}
}

