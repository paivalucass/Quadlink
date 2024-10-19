cmake_minimum_required(VERSION 3.15)

project(quadlink)

add_subdirectory(src/gui)
add_subdirectory(src/ros/src/quadlink_controller)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/../mavlink/install/include)

add_executable(quadlink src/movement/test.cpp
                        src/movement/quadlink.cpp
                        src/connection/quad_connection.cpp
                        src/connection/utils/strings.cpp
                        src/connection/utils/mav_messages.cpp
                        src/connection/utils/clock.cpp
                        src/telemetry/quad_telemetry.cpp
                        src/connection/message_factory.cpp
                        src/action/quad_action.cpp)

target_link_libraries(quadlink 
)