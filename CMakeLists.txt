# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)
#include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project (as current working directory)
get_filename_component(ProjectId ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId} C CXX ASM)

# Set C Standard
set(CMAKE_C_STANDARD 11)
# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
# Set the target board to be a standard pico, you may change this for different targets
set(PICO_BOARD pico)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# point out the CMake, where to find the executable source file
add_executable(${PROJECT_NAME}
        ${CMAKE_CURRENT_LIST_DIR}/main.c
        #${CMAKE_CURRENT_LIST_DIR}/usb_main.c
        ${CMAKE_CURRENT_LIST_DIR}/usb_descriptors.c
        lib/eve_ui/source/eve_ui_main.c
        lib/eve_ui/source/eve_ui_images.c
        lib/eve_ui/source/eve_ui_keyboard.c
        lib/eve_ui/source/eve_ui_utility.c
        lib/eve_ui/source/eve_ui_ext_font.c
        Fonts/C/arial.ttf_15_L4.c
        Images/S/bridgetek_logo.s
        Images/S/cancel.s  
        Images/S/keyboard.s  
        Images/S/keypad.s  
        Images/S/media.s  
        Images/S/refresh.s  
        Images/S/settings.s  
        Images/S/tick.s  
        Images/S/z.s
)
include_directories(
        lib/eve/include
)
# Make sure TinyUSB can find tusb_config.h
target_include_directories(${PROJECT_NAME} PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/lib/eve/include
        ${CMAKE_CURRENT_LIST_DIR}/lib/eve_ui/include
        ${CMAKE_CURRENT_LIST_DIR}/Include
        ${CMAKE_CURRENT_LIST_DIR}/Images
        )

# Tell EVE library to use RP2040 as base class
add_compile_definitions(PLATFORM_RP2040)

# Source code for EVE library (targetted at RP2040)
add_library(eve_library # EVE library name
        # Source code for EVE library
        lib/eve/source/EVE_API.c
        lib/eve/source/EVE_HAL.c
        lib/eve/ports/eve_arch_rpi/EVE_MCU_RP2040.c
)
# Header files used for EVE library
target_include_directories(eve_library PUBLIC
        lib/eve/include
)
# EVE library dependencies
target_link_libraries(eve_library
            pico_stdlib
            hardware_spi
)

# Project dependencies
# for TinyUSB device support and tinyusb_board for the additional board support library used by the example
target_link_libraries(${PROJECT_NAME} 
        eve_library # Project depends on EVE library
        hardware_flash # For non-volatile storage
        hardware_sync # For non-volatile storage
        pico_stdlib # Commonly used features (gpio, timer-delay etc)
        tinyusb_device 
        tinyusb_board
)
# Create map/bin/hex/uf2 files.
pico_add_extra_outputs(${PROJECT_NAME})

# Enable stdio on UART
pico_enable_stdio_uart(${PROJECT_NAME} 1)
# Enable stdio on USB
pico_enable_stdio_usb(${PROJECT_NAME} 1)
