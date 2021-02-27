project(i2c C)

add_avr_library(${PROJECT_NAME} i2c_bitbang.c)

avr_target_include_directories(${PROJECT_NAME} PUBLIC ${PROJECT_SOURCE_DIR})
