project(quadlink)

find_package(MAVLink REQUIRED)

add_executable(quadlink src/movement/quadlink.c)

target_link_libraries(quadlink PRIVATE MAVLink::mavlink)
