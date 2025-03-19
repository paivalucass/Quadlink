#include "communication/connection.hpp"

// TODO: remove ALL console logs 

namespace rcpilot{

int Connector::sockfd = -1;
int Connector::serialfd = -1;

rcpilot::Connector::Connector() : clock() , buffer_size(2048)
{
    buffer = new uint8_t[buffer_size];
    std::fill(buffer, buffer + buffer_size, 0);
}

rcpilot::Connector::~Connector(){
    delete[] buffer;

    if (this->sockfd != -1) {
        close(this->sockfd);
        this->sockfd = -1;
    }

    if (this->serialfd != -1) {
        close(this->serialfd);
        this->serialfd = -1;
    }
}

rcpilot::MessageStatus rcpilot::Connector::check_message(uint16_t target_ID)
{
    mavlink_message_t msg;
    mavlink_status_t status;
    rcpilot::MessageStatus return_status;

    return_status.ack.result = 32; // not used value so it doesnt return fake results (memory garbage)

    for (size_t i = 0; i < rcpilot::Connector::buffer_size; ++i) {
        if (mavlink_parse_char(MAVLINK_COMM_0, rcpilot::Connector::buffer[i], &msg, &status)) {
            if (msg.msgid == target_ID) {
                // TODO: Switch Case here?
                if (target_ID == MAVLINK_MSG_ID_HEARTBEAT){
                    mavlink_msg_heartbeat_decode(&msg, &return_status.heartbeat);
                    rcpilot::Connector::target_system_id = msg.sysid;
                    rcpilot::Connector::system_id = msg.sysid; 
                    rcpilot::Connector::component_id = MAV_COMP_ID_ODID_TXRX_1;
                    rcpilot::Connector::target_component_id = msg.sysid;
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
                return_status.connection = rcpilot::ConnectionStatus::Finished;
                return return_status;
            }
        }
    }
    return_status.connection = rcpilot::ConnectionStatus::Timeout;
    return return_status;
}

rcpilot::ConnectionStatus rcpilot::Connector::create_socket(std::string& connection_url)
{
    
    rcpilot::Connector::connection_url = split_string(connection_url, ':');
    std::string ip_url = rcpilot::Connector::connection_url[0];
    std::string port_url = rcpilot::Connector::connection_url[1];

    u_int16_t port = string_to_int16(port_url);
    
    rcpilot::Connector::sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    if (rcpilot::Connector::sockfd < 0)
    {
        close(rcpilot::Connector::sockfd);
        std::cerr << "Failed creating socket" << std::endl;
        return rcpilot::ConnectionStatus::Failed;
    }

    struct timeval timeout;
    timeout.tv_sec = TIMEOUT_SECONDS; 
    timeout.tv_usec = TIMEOUT_MILISECONDS;           

    if (setsockopt(rcpilot::Connector::sockfd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) < 0) {
        perror("setsockopt");
        close(rcpilot::Connector::sockfd);
        return rcpilot::ConnectionStatus::Failed;
    }

    /*
        Binding socket to the determined port.
    */

    rcpilot::Connector::server_addr.sin_family = AF_INET;
    rcpilot::Connector::server_addr.sin_port = htons(port);
    rcpilot::Connector::server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // local interface BEWARE

    if (bind(rcpilot::Connector::sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        close(rcpilot::Connector::sockfd);
        std::cerr << "Failed binding socket" << std::endl;
        return rcpilot::ConnectionStatus::Failed;
    }

    return rcpilot::ConnectionStatus::Finished;
}

rcpilot::ConnectionStatus rcpilot::Connector::open_serial_port(std::string& serial_port) 
{   
    // e.g., "/dev/ttyUSB0"

    /**
        ATTENTION: The open() method is blocking. If the pixhawk is not connected at the provided port, it will wait until it is plugged. If you no longer want this to be blocking, add the O_NDELAY argument) 
    */
    this->serialfd = open(serial_port.c_str(), O_RDWR | O_NOCTTY);
    if (this->serialfd == -1) 
    {
        return rcpilot::ConnectionStatus::Failed;
    }

    struct termios options;
    tcgetattr(this->serialfd, &options);

    // Set baud rate (115200 as default)
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);

    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~PARENB; // No parity
    options.c_cflag &= ~CSTOPB; // 1 stop bit
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8; // 8 data bits

    // Apply settings
    tcsetattr(this->serialfd, TCSANOW, &options);

    return rcpilot::ConnectionStatus::Finished;
}

rcpilot::MessageStatus rcpilot::Connector::wait_message(uint16_t target_ID, double timeout)
{
    rcpilot::MessageStatus status;
    status.connection = rcpilot::ConnectionStatus::Timeout;
    socklen_t addr_len = sizeof(rcpilot::Connector::drone_addr);
    ssize_t recv_len;

    this->clock.reset();

    while (this->clock.elapsed() < timeout && status.connection != rcpilot::ConnectionStatus::Finished)
    {
        std::fill(rcpilot::Connector::buffer, rcpilot::Connector::buffer + rcpilot::Connector::buffer_size, 0);

        recv_len = recvfrom(rcpilot::Connector::sockfd, rcpilot::Connector::buffer, rcpilot::Connector::buffer_size, 0, 
                            (struct sockaddr *)&drone_addr, &addr_len);

        if (recv_len < 0) {
            status.connection = rcpilot::ConnectionStatus::Failed;
            return status;
        }

        status = rcpilot::Connector::check_message(target_ID);
    }
    
    return status;
}

rcpilot::ConnectionStatus rcpilot::Connector::connect_udp(std::string& connection_url)
{
    /*
        Identifies the drone in the given IP/PORT and verifies the heartbeat.
        Waits 5 seconds for the heartbet.
        If heartbeat not found, returns a timeout.
    */

    rcpilot::ConnectionStatus socket_check = rcpilot::Connector::create_socket(connection_url);

    /*
        Wait for heartbeat mmessage (5 seconds)
    */

    rcpilot::MessageStatus message_status = wait_message(MAVLINK_MSG_ID_HEARTBEAT, 5.0);

    if (message_status.connection == rcpilot::ConnectionStatus::Finished)
    {
        return rcpilot::ConnectionStatus::Finished;
    }
    else if (message_status.connection == rcpilot::ConnectionStatus::Timeout)
    {
        close(rcpilot::Connector::sockfd);
        return rcpilot::ConnectionStatus::Timeout;
    }
    else
    {  
        close(rcpilot::Connector::sockfd);
        return rcpilot::ConnectionStatus::Failed;
    }
}

rcpilot::ConnectionStatus rcpilot::Connector::send_mav_message(mavlink_message_t msg, EncodeType encode)
{   
    int len = mavlink_msg_to_send_buffer(rcpilot::Connector::buffer, &msg);

    if (len <= 0) {
        return rcpilot::ConnectionStatus::Failed;
    }

    rcpilot::MessageStatus ack_status;

    if (encode == rcpilot::EncodeType::COMMAND_LONG)
    {
        constexpr int max_retries = 5;
        constexpr double timeout_seconds = 1.0;

        for (int i = 0; i < max_retries; i++)
        {
            sendto(rcpilot::Connector::sockfd, rcpilot::Connector::buffer, len, 0, 
                   (struct sockaddr*)&drone_addr, sizeof(drone_addr));  

            ack_status = wait_message(MAVLINK_MSG_ID_COMMAND_ACK, timeout_seconds);

            if (ack_status.connection == rcpilot::ConnectionStatus::Finished && ack_status.ack.result == MAV_RESULT_ACCEPTED)
            {   
                return rcpilot::ConnectionStatus::Finished;
            }
            else if (ack_status.connection == rcpilot::ConnectionStatus::Timeout || ack_status.ack.result == MAV_RESULT_FAILED)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(100 * (i + 1))); 
                continue;
            }
            else
            {
                return rcpilot::ConnectionStatus::Failed;
            }
        }

        return rcpilot::ConnectionStatus::Timeout;
    }
    else if (encode == rcpilot::EncodeType::POSITION_TARGET_LOCAL_NED || encode == rcpilot::EncodeType::POSITION_TARGET_BODY)
    {
        constexpr int max_attempts = 3;

        for (int i = 0; i < max_attempts; i++)
        {
            sendto(rcpilot::Connector::sockfd, rcpilot::Connector::buffer, len, 0, 
                   (struct sockaddr*)&drone_addr, sizeof(drone_addr));  
            
            // TODO: Implement a better way to check if the message was received
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        return rcpilot::ConnectionStatus::Finished;
    }

    sendto(rcpilot::Connector::sockfd, rcpilot::Connector::buffer, len, 0, 
           (struct sockaddr*)&drone_addr, sizeof(drone_addr));

    return rcpilot::ConnectionStatus::Finished;
}


mavlink_message_t rcpilot::Connector::build_command(mavlink_command_long_t &command)
{
    mavlink_message_t msg;

    command.target_system = rcpilot::Connector::target_system_id;
    command.target_component = rcpilot::Connector::target_component_id;

    // maybe a buffer manipulation here in the future?
    mavlink_msg_command_long_encode(rcpilot::Connector::system_id, rcpilot::Connector::component_id, &msg, &command);

    return msg;
}

mavlink_message_t rcpilot::Connector::build_command(mavlink_set_position_target_local_ned_t &command){

    mavlink_message_t msg;

    command.target_system = rcpilot::Connector::target_system_id;
    command.target_component = rcpilot::Connector::target_component_id;

    // maybe a buffer manipulation here in the future?
    mavlink_msg_set_position_target_local_ned_encode(rcpilot::Connector::system_id, rcpilot::Connector::component_id, &msg, &command);
    
    return msg;
}
}
