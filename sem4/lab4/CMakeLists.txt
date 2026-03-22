cmake_minimum_required(VERSION 3.17)
project(fmod_lab)

set(CMAKE_CXX_STANDARD 17)

add_executable(fmod_lab main.cpp)

target_include_directories(fmod_lab PRIVATE
        ${CMAKE_SOURCE_DIR}/fmod/inc
)

target_link_directories(fmod_lab PRIVATE
        ${CMAKE_SOURCE_DIR}/fmod/lib
)

target_link_libraries(fmod_lab PRIVATE fmodex64)