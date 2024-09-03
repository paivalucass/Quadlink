#include "mav_messages.h"

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