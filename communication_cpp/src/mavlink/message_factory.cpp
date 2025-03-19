#include "communication/message_factory.hpp"

namespace rcpilot{

mavlink_set_position_target_local_ned_t create_standard_position_command(float* position, uint16_t type_mask, uint8_t frame_type){
    mavlink_set_position_target_local_ned_t command = {0};

    command.coordinate_frame = frame_type;
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

mavlink_command_long_t create_arm_command(){

    mavlink_command_long_t command = {0};
    
    command.command = MAV_CMD_COMPONENT_ARM_DISARM;
    command.confirmation = 0;
    command.param1 = 1.0f;  // 1.0 for arm 
    command.param2 = 0.0f; 

    return command;
}

mavlink_command_long_t create_mode_command(ArdupilotFlightMode flight_mode){

    float mode = static_cast<float>(flight_mode);

    mavlink_command_long_t command = {0}; 

    command.command = MAV_CMD_DO_SET_MODE;  
    command.param1 = MAV_MODE_FLAG_CUSTOM_MODE_ENABLED;
    command.param2 = mode;
    command.param3 = 0; 

    return command;    
}

mavlink_command_long_t create_set_home_command(){

    mavlink_command_long_t command = {0}; 

    command.command = MAV_CMD_DO_SET_HOME;
    command.param1 = 1;  

    return command;
}

mavlink_command_long_t create_takeoff_command(float height){

    mavlink_command_long_t command = {0};

    command.command = MAV_CMD_NAV_TAKEOFF;
    command.param7 = height;

    return command;
}

mavlink_set_position_target_local_ned_t create_set_local_ned_command(float* position){

    uint16_t type_mask = 0b110111111000;

    return create_standard_position_command(position, type_mask, MAV_FRAME_LOCAL_NED);
}

mavlink_set_position_target_local_ned_t create_set_body_command_position(float* position){

    uint16_t type_mask = 0b110111111000; 

    return create_standard_position_command(position, type_mask, MAV_FRAME_BODY_NED);
}

mavlink_set_position_target_local_ned_t create_set_body_command_velocity(float* position){

    uint16_t type_mask = 0b110111000111; 

    return create_standard_position_command(position, type_mask, MAV_FRAME_BODY_NED);
}

mavlink_set_position_target_local_ned_t create_set_yaw_rate(float* position){

    uint16_t type_mask = 0b000000000000; // TODO: find the right mask 

    return create_standard_position_command(position, type_mask, MAV_FRAME_BODY_NED);
}

mavlink_set_position_target_local_ned_t create_set_yaw(float* position){

    uint16_t type_mask = 0b000000000000; // TODO: find the right mask 

    return create_standard_position_command(position, type_mask, MAV_FRAME_BODY_NED);
}
}