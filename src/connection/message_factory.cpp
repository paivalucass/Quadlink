#include "message_factory.h"



namespace quadlink{
quadlink::MessageFactory::MessageFactory(){

}


mavlink_command_long_t quadlink::MessageFactory::create_arm_command(){

    mavlink_command_long_t command = {0};
    
    command.command = MAV_CMD_COMPONENT_ARM_DISARM;
    command.confirmation = 0;
    command.param1 = 1.0f;  // 1.0 for arm 
    command.param2 = 0.0f; 

    return command;
}

mavlink_command_long_t quadlink::MessageFactory::create_mode_command(uint16_t mode){

    mavlink_command_long_t command = {0}; 

    command.command = MAV_CMD_DO_SET_MODE;  
    command.param1 = mode; 

    return command;    
}

mavlink_command_long_t quadlink::MessageFactory::create_set_home_command(){

    mavlink_command_long_t command = {0}; 

    command.command = MAV_CMD_DO_SET_HOME;
    command.param1 = 0; 
    command.param2 = 0; 
    command.param3 = 0; 

    return command;
}
}
