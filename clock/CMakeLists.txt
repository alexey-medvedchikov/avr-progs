project(clock C)

add_avr_executable(${PROJECT_NAME} main.c)

find_library(C_LIB c)
message(STATUS "avr-libc: ${C_LIB}")

avr_target_link_libraries(${PROJECT_NAME} ${C_LIB} i2c)

add_dependencies(${PROJECT_NAME} i2c)
