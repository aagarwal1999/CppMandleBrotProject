cmake_minimum_required(VERSION 3.11.3)

add_definitions(-std=c++17)
project(Mandlebrot)
set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS} -std=c++17 -pthread")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread")

find_package(OpenCV 4.1 REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(${OpenCV_LIBRARY_DIRS})
add_definitions(${OpenCV_DEFINITIONS})


add_executable(Mandlebrot src/main.cpp src/Mandlebrot.cpp src/Window.cpp)
target_link_libraries(Mandlebrot ${OpenCV_LIBRARIES})
