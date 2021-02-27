cmake_minimum_required(VERSION 2.8)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/cmake/avr-gcc.toolchain.cmake")

set(AVR_UPLOADTOOL avrdude)
set(AVR_PROGRAMMER usbasp)
set(AVR_UPLOADTOOL_PORT usb)
set(AVR_MCU attiny85)
set(AVR_H_FUSE 0xDF)
set(AVR_L_FUSE 0xE2)

project(avr-progs C)

# status messages
message(STATUS "Current uploadtool is: ${AVR_UPLOADTOOL}")
message(STATUS "Current programmer is: ${AVR_PROGRAMMER}")
message(STATUS "Current upload port is: ${AVR_UPLOADTOOL_PORT}")
message(STATUS "Current uploadtool options are: ${AVR_UPLOADTOOL_OPTIONS}")
message(STATUS "Current MCU is set to: ${AVR_MCU}")
message(STATUS "Current H_FUSE is set to: ${AVR_H_FUSE}")
message(STATUS "Current L_FUSE is set to: ${AVR_L_FUSE}")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

set(MCU_SPEED "8000000UL")

if(DEFINED ENV{AVR_FIND_ROOT_PATH})
  set(CMAKE_FIND_ROOT_PATH $ENV{AVR_FIND_ROOT_PATH})
else(DEFINED ENV{AVR_FIND_ROOT_PATH})
  if(EXISTS "/opt/local/avr")
    set(CMAKE_FIND_ROOT_PATH "/opt/local/avr")
  elseif(EXISTS "/usr/avr")
    set(CMAKE_FIND_ROOT_PATH "/usr/avr")
  elseif(EXISTS "/usr/lib/avr")
    set(CMAKE_FIND_ROOT_PATH "/usr/lib/avr")
  elseif(EXISTS "/usr/local/CrossPack-AVR")
    set(CMAKE_FIND_ROOT_PATH "/usr/local/CrossPack-AVR")
  else(EXISTS "/opt/local/avr")
    message(FATAL_ERROR "Please set AVR_FIND_ROOT_PATH in your environment.")
  endif(EXISTS "/opt/local/avr")
endif(DEFINED ENV{AVR_FIND_ROOT_PATH})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
# not added automatically, since CMAKE_SYSTEM_NAME is "generic"
set(CMAKE_SYSTEM_INCLUDE_PATH "${CMAKE_FIND_ROOT_PATH}/include")
set(CMAKE_SYSTEM_LIBRARY_PATH "${CMAKE_FIND_ROOT_PATH}/lib")

message(STATUS "Set CMAKE_FIND_ROOT_PATH to ${CMAKE_FIND_ROOT_PATH}")
message(STATUS "Set CMAKE_SYSTEM_INCLUDE_PATH to ${CMAKE_SYSTEM_INCLUDE_PATH}")
message(STATUS "Set CMAKE_SYSTEM_LIBRARY_PATH to ${CMAKE_SYSTEM_LIBRARY_PATH}")

if(CMAKE_BUILD_TYPE MATCHES Release)
  set(CMAKE_C_FLAGS_RELEASE "-Os")
  set(CMAKE_CXX_FLAGS_RELEASE "-Os")
endif(CMAKE_BUILD_TYPE MATCHES Release)

if(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)
  set(CMAKE_C_FLAGS_RELWITHDEBINFO
      "-Os -save-temps -fstack-usage -g -gdwarf-3 -gstrict-dwarf")
  set(CMAKE_CXX_FLAGS_RELWITHDEBINFO
      "-Os -save-temps -fstack-usage -g -gdwarf-3 -gstrict-dwarf")
endif(CMAKE_BUILD_TYPE MATCHES RelWithDebInfo)

if(CMAKE_BUILD_TYPE MATCHES Debug)
  set(CMAKE_C_FLAGS_DEBUG
      "-O0 -save-temps -fstack-usage -g -gdwarf-3 -gstrict-dwarf")
  set(CMAKE_CXX_FLAGS_DEBUG
      "-O0 -save-temps -fstack-usage -g -gdwarf-3 -gstrict-dwarf")
endif(CMAKE_BUILD_TYPE MATCHES Debug)

add_definitions("-DF_CPU=${MCU_SPEED}")
add_definitions("-Wall")
add_definitions("-Wextra")
add_definitions("-Werror")
add_definitions("-Wshadow")
add_definitions("-Wpadded")
add_definitions("-Wformat=2")
add_definitions("-Wdouble-promotion")
add_definitions("-flto")
add_definitions("-fdata-sections")
add_definitions("-ffunction-sections")
add_definitions("-fpack-struct")
add_definitions("-fshort-enums")
add_definitions("-funsigned-char")
add_definitions("-funsigned-bitfields")
add_definitions("-std=gnu99")

include_directories(include)

add_subdirectory(i2c)
add_subdirectory(clock)
