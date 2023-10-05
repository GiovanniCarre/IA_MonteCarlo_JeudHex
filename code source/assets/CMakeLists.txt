cmake_minimum_required(VERSION 3.0)

project(assets)

find_package(Threads)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED on)
set(CMAKE_CXX_EXTENSIONS off)
add_compile_options(-Wall -Wpedantic)


set(SRCS
    learner.cc
    learningNode.cc
    playingNode.cc
    fileManager.cc
    )
    
set(HEADERS
    learner.hh
    learningNode.hh
    playingNode.hh
    fileManager.hh
    )

add_library(assets ${SRCS} ${HEADERS})
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
