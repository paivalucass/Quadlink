cmake_minimum_required(VERSION 3.15)

project(quadlink)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(MAVSDK REQUIRED
             MAVLink REQUIRED)

include_directories(${MAVSDK_INCLUDE_DIRS})

include_directories(../mavlink/install/include)

add_executable(quadlink src/movement/quadlink.cpp)

target_link_libraries(quadlink 
    MAVSDK::mavsdk
    mavlink
    # MAVSDK::mavsdk_action
    # MAVSDK::mavsdk_telemetry
)
