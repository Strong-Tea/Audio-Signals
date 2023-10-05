cmake_minimum_required(VERSION 3.10)

project(WaveProcessing)

# Find the necessary libraries
find_library(LIBM m)
find_library(LIBSNDFILE sndfile)

# Creating an executable target for modulated_wave
add_executable(modulated_wave waves.c wave.c modulated_wave.c)
target_link_libraries(modulated_wave PRIVATE ${LIBM} ${LIBSNDFILE})

# Creating an executable target for merge_wave
add_executable(merge_wave waves.c wave.c merge_wave.c)
target_link_libraries(merge_wave PRIVATE ${LIBM} ${LIBSNDFILE})

# Creating an executable target for single_wave
add_executable(single_wave waves.c wave.c single_wave.c)
target_link_libraries(single_wave PRIVATE ${LIBM} ${LIBSNDFILE})
