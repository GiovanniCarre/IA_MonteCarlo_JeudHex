cmake_minimum_required(VERSION 3.1.0)

project(Arene_light)

add_subdirectory(joueurs)
add_subdirectory(assets)

find_package(Threads)


set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)
add_compile_options(-Wall -Wpedantic)



aux_source_directory(. SRC_LIST)
add_executable(${PROJECT_NAME} ${SRC_LIST})
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} joueurs)
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} assets)

