#include <iostream>
#include <mavsdk/mavsdk.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <mavlink/common/mavlink.h>
#include <chrono>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

using namespace mavsdk;
using namespace std;


int main() {
    string connection_url = "127.0.0.1";
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    
    if (sockfd < 0) {
        perror("socket creation failed");
        return 1;
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(14562);
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY); // Bind to all interfaces

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(sockfd);
        return 1;
    }

    uint8_t buffer[2048];
    sockaddr_in drone_addr;
    socklen_t addr_len = sizeof(drone_addr);

    cout << "Waiting for messages..." << endl;
    ssize_t len = recvfrom(sockfd, buffer, sizeof(buffer), 0, (struct sockaddr *)&drone_addr, &addr_len);

    if (len < 0) {
        perror("recvfrom failed");
    } else {
        cout << "Message received, length: " << len << endl;
        mavlink_message_t msg;
        mavlink_status_t status;
        // Decode the MAVLink message
        for (ssize_t i = 0; i < len; ++i) {
            if (mavlink_parse_char(MAVLINK_COMM_0, buffer[i], &msg, &status)) {
                if (msg.msgid == MAVLINK_MSG_ID_HEARTBEAT) {
                    cout << "Heartbeat received from drone." << endl;
                }
            }
        }
    }

    close(sockfd);
    return 0;
}


// #include "uav.h"
// #include <sys/socket.h>
// #include <arpa/inet.h>
// #include <unistd.h>
// #include <iostream>
// #include <cmath>

// UAV::UAV(uint8_t sys_id, uint8_t comp_id, const std::string &connection_url) 
//     : system_id(sys_id), component_id(comp_id), target_system_id(sys_id), target_component_id(comp_id) {

//     // Configuração da conexão UDP
//     sockfd = socket(AF_INET, SOCK_DGRAM, 0);
//     drone_addr.sin_family = AF_INET;
//     drone_addr.sin_port = htons(14550);  // Porta padrão do MAVLink
//     inet_pton(AF_INET, connection_url.c_str(), &drone_addr.sin_addr);  // Endereço IP do drone
// }

// void UAV::send_mavlink_message(mavlink_message_t &msg) {
//     uint8_t buffer[1024];
//     int len = mavlink_msg_to_send_buffer(buffer, &msg);
//     sendto(sockfd, buffer, len, 0, (struct sockaddr*)&drone_addr, sizeof(drone_addr));
// }

// void UAV::calculate_relative_position(float current_lat, float current_lon, float current_alt,
//                                       float distance_north, float distance_east, float &new_lat, float &new_lon, float &new_alt) {
//     const double earth_radius = 6378137.0; // raio da Terra em metros

//     // Converter latitude e longitude atuais de graus para radianos
//     double lat_rad = current_lat * M_PI / 180.0;
//     double lon_rad = current_lon * M_PI / 180.0;

//     // Calcular a nova latitude e longitude em radianos
//     new_lat = current_lat + (distance_north / earth_radius) * (180.0 / M_PI);
//     new_lon = current_lon + (distance_east / (earth_radius * cos(lat_rad))) * (180.0 / M_PI);

//     // Altitude permanece a mesma
//     new_alt = current_alt;
// }

// void UAV::go_to(float distance_north, float distance_east, float target_altitude) {
//     // Aqui você obteria a posição atual via telemetria. Vamos usar valores fictícios para este exemplo.
//     float current_lat = -35.363261;
//     float current_lon = 149.165230;
//     float current_alt = 584.0;

//     // Calcular a nova posição alvo
//     float new_lat, new_lon, new_alt;
//     calculate_relative_position(current_lat, current_lon, current_alt, distance_north, distance_east, new_lat, new_lon, new_alt);

//     // Construir a mensagem MAVLink para comando de movimento
//     mavlink_message_t msg;
//     mavlink_msg_set_position_target_global_int_pack(
//         system_id, component_id, &msg, 0, target_system_id, target_component_id,
//         MAV_FRAME_GLOBAL_RELATIVE_ALT_INT,  // Quadro de referência
//         0b0000111111111000,  // Máscara de ignorar valores específicos
//         new_lat * 1E7,       // Nova latitude
//         new_lon * 1E7,       // Nova longitude
//         target_altitude,     // Nova altitude
//         0, 0, 0,             // Velocidades em x, y, z (ignoradas)
//         0, 0, 0,             // Acelerações em x, y, z (ignoradas)
//         0, 0);               // Yaw e Yaw rate (ignorados)

//     // Enviar a mensagem
//     send_mavlink_message(msg);
// }
