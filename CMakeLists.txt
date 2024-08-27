cmake_minimum_required(VERSION 3.15)

project(quadlink)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Encontre e configure o MAVSDK
find_package(MAVSDK REQUIRED)

# Inclua os diretórios necessários
include_directories(${MAVSDK_INCLUDE_DIRS})

# Inclua o caminho para os arquivos de cabeçalho do MAVLink
include_directories(${CMAKE_SOURCE_DIR}/../mavlink/install/include)

# Adicione o executável
add_executable(quadlink src/movement/test.cpp
                        src/movement/quadlink.cpp
                        src/connection/quad_connection.cpp
                        src/connection/utils/strings.cpp
                        src/connection/utils/mav_messages.cpp
                        src/connection/utils/clock.cpp)

# Vincule o MAVSDK ao seu projeto
target_link_libraries(quadlink 
    MAVSDK::mavsdk
)