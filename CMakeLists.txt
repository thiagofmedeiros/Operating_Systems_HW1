cmake_minimum_required(VERSION 3.21)
project(HW1 C)

set(CMAKE_C_STANDARD 99)

add_executable(HW1 main.c)
find_library(LIBRT rt)
if(LIBRT)
    target_link_libraries(HW1 ${LIBRT})
endif()