#include "message_factory.h"


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

mavlink_command_long_t quadlink::MessageFactory::create_mode_command(uint8_t mode){

    mavlink_command_long_t command = {0}; 

    command.command = MAV_CMD_DO_SET_MODE;  
    command.param1 = mode; 

    return command;    
}
