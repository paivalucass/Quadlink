#include <iostream>
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
    std::this_thread::sleep_for(std::chrono::seconds(7));
    status = quadcopter.change_acceleration(0.3,0.3,0);
    std::this_thread::sleep_for(std::chrono::seconds(4));
    return 0;
}