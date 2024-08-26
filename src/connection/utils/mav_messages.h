#pragma once

#include <string>
#include <memory>
#include "../quad_connection.h"
#include <mavsdk/mavsdk.h>
#include <mavlink/common/mavlink.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cmath>
#include <iostream>



// quadlink::ConnectionStatus check_message(const uint8_t* buffer, size_t size);