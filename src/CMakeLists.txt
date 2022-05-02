add_subdirectory(adc)
add_subdirectory(bluetooth)
add_subdirectory(imu)

target_sources(app PRIVATE main.c)