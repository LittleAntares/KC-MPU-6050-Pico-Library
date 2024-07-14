cmake_minimum_required(VERSION 3.13)

# Include the Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Define the project name
set(ProjectName kcmpu6050)

# Create the project
project(${ProjectName})

# Initialize the Pico SDK
pico_sdk_init()

# Create the library
add_library(${ProjectName}
    kcmpu6050.h
    kcmpu6050.cpp
)

# Link the necessary libraries
target_link_libraries(${ProjectName}
    pico_stdlib
    hardware_i2c
)

# Specify the include directories
target_include_directories(${ProjectName} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")