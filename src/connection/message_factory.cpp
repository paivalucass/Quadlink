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

mavlink_command_long_t quadlink::MessageFactory::create_mode_command(ArdupilotFlightMode flight_mode){

    float mode = static_cast<float>(flight_mode);

    mavlink_command_long_t command = {0}; 

    command.command = MAV_CMD_DO_SET_MODE;  
    command.param1 = MAV_MODE_FLAG_CUSTOM_MODE_ENABLED;
    command.param2 = mode;
    command.param3 = 0; 

    return command;    
}

mavlink_command_long_t quadlink::MessageFactory::create_set_home_command(){

    mavlink_command_long_t command = {0}; 

    command.command = MAV_CMD_DO_SET_HOME;
    command.param1 = 1;  // 1 para usar a posição atual do sistema, 0 para definir manualmente

    return command;
}

mavlink_command_long_t quadlink::MessageFactory::create_takeoff_command(float height){

    mavlink_command_long_t command = {0};

    command.command = MAV_CMD_NAV_TAKEOFF;
    command.param7 = height;

    return command;
}

mavlink_command_long_t quadlink::MessageFactory::create_land_command(){
    mavlink_command_long_t command = {0};

    // TODO: Implement land message construction
    return command;

}

mavlink_set_position_target_local_ned_t quadlink::MessageFactory::create_set_local_ned_command(float* position){
    mavlink_set_position_target_local_ned_t command = {0};

    uint16_t type_mask = 0b000000000000;

    command.coordinate_frame = MAV_FRAME_LOCAL_NED;
    command.type_mask = type_mask;
    command.x = position[0];
    command.y = position[1];
    command.z = position[2];
    command.vx = position[3];
    command.vy = position[4];
    command.vz = position[5];
    command.afx = position[6];
    command.afy = position[7];
    command.afz = position[8];
    // pos 9 reserved for use force
    command.yaw = position[10];
    command.yaw_rate = position[11];

    return command;
}

mavlink_set_position_target_local_ned_t quadlink::MessageFactory::create_set_body_command(float* position){
    mavlink_set_position_target_local_ned_t command = {0};

    uint16_t type_mask = 0b000000000000; // hard coding this value can be the reason for future issues, #TODO: try to change this structure

    command.coordinate_frame = MAV_FRAME_BODY_NED;
    command.type_mask = type_mask;
    command.x = position[0];
    command.y = position[1];
    command.z = position[2];
    command.vx = position[3];
    command.vy = position[4];
    command.vz = position[5];
    command.afx = position[6];
    command.afy = position[7];
    command.afz = position[8];
    // pos 9 reserved for use force
    command.yaw = position[10];
    command.yaw_rate = position[11];

    return command;

}
}