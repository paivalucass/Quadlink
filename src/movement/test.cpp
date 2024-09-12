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
#include "../connection/quad_connection.h"
#include "quadlink.h"

int main() {
    quadlink::Vehicle vehicle = quadlink::Vehicle::QuadCopter;
    quadlink::UAV quadcopter(vehicle);
    std::string connection_url = "127.0.0.1:14562";
    quadlink::ConnectionStatus status = quadcopter.connect(connection_url);
    status = quadcopter.arm(quadlink::ArdupilotFlightMode::GUIDED);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    status = quadcopter.takeoff(1.5, false);
    std::this_thread::sleep_for(std::chrono::seconds(4));
    status = quadcopter.land(false);
    std::this_thread::sleep_for(std::chrono::seconds(4));
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
