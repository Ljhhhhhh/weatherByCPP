cmake_minimum_required(VERSION 3.22)
project(cppWeather)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost 1.82.0 REQUIRED COMPONENTS filesystem)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(cppWeather main.cpp)

target_link_libraries(cppWeather Boost::filesystem)