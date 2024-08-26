#include <string>
#include <memory>
#include "../quad_connection.h"
#include <mavlink/common/mavlink.h>
#include <mavsdk/plugins/action/action.h>
#include <mavsdk/plugins/telemetry/telemetry.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cmath>
#include <iostream>

// quadlink::ConnectionStatus check_message(const uint8_t* buffer, ssize_t size, uint8_t target_ID)
// {
//     mavlink_message_t msg;
//     mavlink_status_t status;

//     for (ssize_t i = 0; i < size; ++i) {
//         if (mavlink_parse_char(MAVLINK_COMM_0, buffer[i], &msg, &status)) {
//             if (msg.msgid == target_ID) {
//                 return quadlink::ConnectionStatus::Success;
//             }
//         }
//     }
//     return quadlink::ConnectionStatus::Failed;
// }