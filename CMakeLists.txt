# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)
#include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project (as current working directory)
get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_LIST_DIR} NAME)
string(REPLACE " " "_" PROJECT_NAME ${PROJECT_NAME})
project(${PROJECT_NAME} C CXX ASM)

# Uncomment to see command lines for compilation.
#set( CMAKE_VERBOSE_MAKEFILE on )

# Set C Standard
set(CMAKE_C_STANDARD 11)
# Set C++ Standard
set(CMAKE_CXX_STANDARD 17)
# Set the target board to be a standard pico, you may change this for different targets
set(PICO_BOARD pico)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

set(FONT_MAKEFILE "Fonts/fonts.mk")
set(FONT_OUTFILES 
	${CMAKE_SOURCE_DIR}/Fonts/C/*.c
)
add_custom_command(OUTPUT ${FONT_OUTFILES}
	PRE_BUILD
	COMMAND make -f ${CMAKE_SOURCE_DIR}/Fonts/fonts.mk
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Fonts
    DEPENDS ${FONT_MAKEFILE}
)
add_custom_target(MAKE_FONTS ALL DEPENDS ${FONT_OUTFILES})

# Compile a list of Font source files.
file(GLOB FONT_FILES CONFIGURE_DEPENDS
	Fonts/C/*.c
)

set(IMAGES_MAKEFILE "Images/images.mk")
set(IMAGES_OUTFILES 
	${CMAKE_SOURCE_DIR}/Images/S/*.S
)
add_custom_command(OUTPUT ${IMAGES_OUTFILES}
	PRE_BUILD
	COMMAND make -f ${CMAKE_SOURCE_DIR}/Images/images.mk
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Images
    DEPENDS ${IMAGES_MAKEFILE}
)
add_custom_target(MAKE_IMAGES ALL DEPENDS ${IMAGES_OUTFILES})

# Compile a list of Image source files.
file(GLOB IMAGE_FILES CONFIGURE_DEPENDS
	Images/S/*.S
)

file(GLOB SRC_FILES CONFIGURE_DEPENDS
	Sources/main.c
	Sources/eve_keyboard.c
	Sources/usb_descriptors.c
	lib/eve_ui/eve_ui_arch_rpi/*.c
	lib/eve_ui/source/*.c
	lib/eve_ui_keyboard/*.c
)

# point out the CMake, where to find the executable source file
add_executable(${PROJECT_NAME} ${SRC_FILES})

# Make sure TinyUSB can find tusb_config.h
include_directories(
        ${CMAKE_CURRENT_LIST_DIR}
        ${CMAKE_CURRENT_LIST_DIR}/lib/eve/include
        ${CMAKE_CURRENT_LIST_DIR}/lib/eve_ui/include
        ${CMAKE_CURRENT_LIST_DIR}/lib/eve_ui_keyboard
        ${CMAKE_CURRENT_LIST_DIR}/Includes
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

add_dependencies(${PROJECT_NAME} MAKE_FONTS MAKE_IMAGES)
target_sources(${PROJECT_NAME} PRIVATE ${FONT_FILES} ${IMAGE_FILES})

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
