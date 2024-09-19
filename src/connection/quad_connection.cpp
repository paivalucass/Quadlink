#include "quad_connection.h"

namespace quadlink{

quadlink::QuadConnector::QuadConnector() : clock() , buffer_size(2048)
{
    buffer = new uint8_t[buffer_size];
    std::fill(buffer, buffer + buffer_size, 0);
}

quadlink::QuadConnector::~QuadConnector(){
    delete[] buffer;
}

quadlink::MessageStatus quadlink::QuadConnector::check_message(uint16_t target_ID)
{
    mavlink_message_t msg;
    mavlink_status_t status;
    // TODO: one variable for all MessageStatus.
    quadlink::MessageStatus return_status;

    return_status.ack.result = 32; // not used value so it doesnt return fake results (memory garbage)

    for (ssize_t i = 0; i < quadlink::QuadConnector::buffer_size; ++i) {
        if (mavlink_parse_char(MAVLINK_COMM_0, quadlink::QuadConnector::buffer[i], &msg, &status)) {
            if (msg.msgid == target_ID) {
                // TODO: Switch Case here?
                if (target_ID == MAVLINK_MSG_ID_HEARTBEAT){
                    mavlink_msg_heartbeat_decode(&msg, &return_status.heartbeat);
                    // CURRENTLY HARD CODED BUT CAN BE THE CAUSE OF FUTURE PROBLEMS WITH CONNECTION, THIS SHOULD BE CHANGED ASAP TO A MORE DYNAMIC APPROACH
                    quadlink::QuadConnector::target_system_id = msg.sysid;
                    quadlink::QuadConnector::system_id = 1; // Considering both system id and component id from sender are always 1 (should be working for now)
                    quadlink::QuadConnector::component_id = 1;
                    quadlink::QuadConnector::target_component_id = 1; // 1 is USUALLY the autopilot (theres only one)
                }
                else if (target_ID == MAVLINK_MSG_ID_COMMAND_ACK){
                    mavlink_msg_command_ack_decode(&msg, &return_status.ack);
                }
                else if (target_ID == MAVLINK_MSG_ID_SYS_STATUS){
                    mavlink_msg_sys_status_decode(&msg, &return_status.sys);
                }
                else if (target_ID == MAVLINK_MSG_ID_LOCAL_POSITION_NED){
                    mavlink_msg_local_position_ned_decode(&msg, &return_status.local_position);
                }
                return_status.connection = quadlink::ConnectionStatus::Finished;
                return return_status;
            }
        }
    }
    return_status.connection = quadlink::ConnectionStatus::Timeout;
    return return_status;
}

quadlink::ConnectionStatus quadlink::QuadConnector::create_socket(std::string& connection_url)
{
    quadlink::QuadConnector::connection_url = split_string(connection_url, ':');
    std::string ip_url = quadlink::QuadConnector::connection_url[0];
    std::string port_url = quadlink::QuadConnector::connection_url[1];

    u_int16_t port = string_to_int16(port_url);
    
    quadlink::QuadConnector::sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (quadlink::QuadConnector::sockfd < 0)
    {
        close(quadlink::QuadConnector::sockfd);
        std::cerr << "Failed creating socket" << std::endl;
        return quadlink::ConnectionStatus::Failed;
    }

    struct timeval timeout;
    timeout.tv_sec = TIMEOUT_SECONDS; 
    timeout.tv_usec = TIMEOUT_MILISECONDS;           

    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        perror("setsockopt");
        close(sockfd);
        return quadlink::ConnectionStatus::Failed;
    }

    /*
        Binding socket to the determined port.
    */

    quadlink::QuadConnector::server_addr.sin_family = AF_INET;
    quadlink::QuadConnector::server_addr.sin_port = htons(port);
    // TODO: BIND ONLY TO THE SPECIFIED IP IN URL (CURRENTLY BINDING TO ALL)
    quadlink::QuadConnector::server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Bind to all interfaces 

    if (bind(quadlink::QuadConnector::sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        close(quadlink::QuadConnector::sockfd);
        std::cerr << "Failed binding socket" << std::endl;
        return quadlink::ConnectionStatus::Failed;
    }

    return quadlink::ConnectionStatus::Finished;
}

quadlink::MessageStatus quadlink::QuadConnector::wait_message(uint16_t target_ID, double time_waiting)
{
    ssize_t recv_len;
    quadlink::MessageStatus status;
    status.connection = quadlink::ConnectionStatus::Timeout;
    //  TODO: Change this?
    socklen_t addr_len = sizeof(quadlink::QuadConnector::drone_addr);

    /*
        Wait for given message
    */

    //  Reset clock choronometer
    quadlink::QuadConnector::clock.reset();

    while (quadlink::QuadConnector::clock.watch() < time_waiting && status.connection != quadlink::ConnectionStatus::Finished)
    {
        std::fill(quadlink::QuadConnector::buffer, quadlink::QuadConnector::buffer + quadlink::QuadConnector::buffer_size, 0);
        /*
            recvfrom stores the address info into drone_addr
        */
        recv_len = recvfrom(quadlink::QuadConnector::sockfd, quadlink::QuadConnector::buffer, quadlink::QuadConnector::buffer_size, 0, (struct sockaddr *)&drone_addr, &addr_len);

        if (recv_len < 0) {
            status.connection = quadlink::ConnectionStatus::Failed;
            return status;
        } else {
            status = quadlink::QuadConnector::check_message(target_ID);
        }
    }
    
    return status;
}

quadlink::ConnectionStatus quadlink::QuadConnector::connect_udp(std::string& connection_url)
{
    /*
        Identifies the drone in the given IP/PORT and verifies the heartbeat.
        Waits 5 seconds for the heartbet.
        If heartbeat not found, returns a timeout.
    */

    quadlink::ConnectionStatus socket_check = quadlink::QuadConnector::create_socket(connection_url);

    /*
        Wait for heartbet mmessage (5 seconds)
    */

    quadlink::MessageStatus message_status = wait_message(MAVLINK_MSG_ID_HEARTBEAT, 5.0);

    if (message_status.connection == quadlink::ConnectionStatus::Finished)
    {
        return quadlink::ConnectionStatus::Finished;
    }
    else if (message_status.connection == quadlink::ConnectionStatus::Timeout)
    {
        close(quadlink::QuadConnector::sockfd);
        return quadlink::ConnectionStatus::Timeout;
    }
    else
    {  
        close(quadlink::QuadConnector::sockfd);
        return quadlink::ConnectionStatus::Failed;
    }
}

quadlink::ConnectionStatus quadlink::QuadConnector::send_mav_message(mavlink_message_t msg, EncodeType encode)
{   
    std::fill(quadlink::QuadConnector::buffer, quadlink::QuadConnector::buffer + quadlink::QuadConnector::buffer_size, 0);
    int len = mavlink_msg_to_send_buffer(quadlink::QuadConnector::buffer, &msg);
    quadlink::MessageStatus ack_status;

    if (encode == quadlink::EncodeType::COMMAND_LONG){
        /*
            ACKS are only received wwhen using COMMAND LONG messages, so it should only be considered in this case.
        */
        // Try three times to receive ACK (temporary)
        for (int i = 0; i < 3; i++)
        {
            sendto(quadlink::QuadConnector::sockfd, quadlink::QuadConnector::buffer, len, 0, (struct sockaddr*)&drone_addr, sizeof(drone_addr));  

            ack_status = wait_message(MAVLINK_MSG_ID_COMMAND_ACK, 2.0);
            
            if (ack_status.connection == quadlink::ConnectionStatus::Finished && ack_status.ack.result == MAV_RESULT_ACCEPTED)
            {   
                return quadlink::ConnectionStatus::Finished;
            }
            else if (ack_status.connection == quadlink::ConnectionStatus::Timeout || ack_status.ack.result == MAV_RESULT_FAILED)
            {
                continue;
            }
            else{
                return quadlink::ConnectionStatus::Failed;
            }
        }
        return quadlink::ConnectionStatus::Timeout;
    }
    else if (encode == quadlink::EncodeType::POSITION_TARGET_LOCAL_NED || encode == quadlink::EncodeType::POSITION_TARGET_BODY){
        // TODO: Verify message arrival.
        sendto(quadlink::QuadConnector::sockfd, quadlink::QuadConnector::buffer, len, 0, (struct sockaddr*)&drone_addr, sizeof(drone_addr));  
        return quadlink::ConnectionStatus::Finished;
    }
    return quadlink::ConnectionStatus::Finished;
}

mavlink_message_t quadlink::QuadConnector::build_command(mavlink_command_long_t &command)
{
    mavlink_message_t msg;

    command.target_system = quadlink::QuadConnector::target_system_id;
    command.target_component = quadlink::QuadConnector::target_component_id;

    // maybe a buffer manipulation here in the future?
    mavlink_msg_command_long_encode(quadlink::QuadConnector::system_id, quadlink::QuadConnector::component_id, &msg, &command);

    return msg;
}

mavlink_message_t quadlink::QuadConnector::build_command(mavlink_set_position_target_local_ned_t &command){

    mavlink_message_t msg;

    command.target_system = quadlink::QuadConnector::target_system_id;
    command.target_component = quadlink::QuadConnector::target_component_id;

    // maybe a buffer manipulation here in the future?
    mavlink_msg_set_position_target_local_ned_encode(quadlink::QuadConnector::system_id, quadlink::QuadConnector::component_id, &msg, &command);
    
    return msg;
}
}