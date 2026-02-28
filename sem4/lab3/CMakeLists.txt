cmake_minimum_required(VERSION 4.1)
project(lab3)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab3 lab3.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fopenmp")

# Ищем OpenMP (для проверки)
find_package(OpenMP REQUIRED)

if (OpenMP_CXX_FOUND)
    message("OpenMP C++ found")

    # Проверяем, существует ли уже target lab3
    if (NOT TARGET lab3)
        add_executable(lab3 lab3.cpp)
    endif()

    # Линкуем OpenMP
    target_link_libraries(lab3 PRIVATE OpenMP::OpenMP_CXX)

    # Добавляем флаги компиляции для OpenMP
    target_compile_options(lab3 PRIVATE ${OpenMP_CXX_FLAGS})
else()
    message("OpenMP not found, using manual flags")

    if (NOT TARGET lab3)
        add_executable(lab3 lab3.cpp)
    endif()

    target_compile_options(lab3 PRIVATE -fopenmp)
    target_link_options(lab3 PRIVATE -fopenmp)
endif()