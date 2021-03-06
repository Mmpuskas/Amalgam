cmake_minimum_required(VERSION 3.5)

message(STATUS "Configuring Load Test")

# Load test client
add_executable(LoadTestClient
    Private/LoadTestClientMain.cpp
    Private/SimulatedClient.cpp
    Public/SimulatedClient.h
    Private/WorldSim.cpp
    Public/WorldSim.h
    
    # Client objects
    ${PROJECT_SOURCE_DIR}/Client/Config/Private/Config.cpp
    ${PROJECT_SOURCE_DIR}/Client/Config/Public/Config.h
    ${PROJECT_SOURCE_DIR}/Client/Network/Private/Network.cpp
    ${PROJECT_SOURCE_DIR}/Client/Network/Public/Network.h
    ${PROJECT_SOURCE_DIR}/Client/Network/Private/MessageHandler.cpp
    ${PROJECT_SOURCE_DIR}/Client/Network/Public/MessageHandler.h
)

target_include_directories(LoadTestClient
    PRIVATE
        ${SDL2_INCLUDE_DIRS}
        ${SDL2PP_INCLUDE_DIRS}
        ${CMAKE_CURRENT_SOURCE_DIR}/Private
        ${CMAKE_CURRENT_SOURCE_DIR}/Public

        # Client objects
        ${PROJECT_SOURCE_DIR}/Client/Network/Public
        ${PROJECT_SOURCE_DIR}/Client/Config/Public
)

target_link_libraries(LoadTestClient
    PRIVATE
        -static-libgcc
        -static-libstdc++
        ${SDL2_LIBRARIES}
        ${SDL2PP_LIBRARIES}
        SDL2_net-static
        readerwriterqueue
        EnTT::EnTT
        Shared
)

# Compile with C++17
target_compile_features(LoadTestClient PRIVATE cxx_std_20)
set_target_properties(LoadTestClient PROPERTIES CXX_EXTENSIONS OFF)

# Enable compile warnings.
target_compile_options(LoadTestClient PUBLIC -Wall -Wextra)
