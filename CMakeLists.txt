cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(gol)

find_package(libpqxx CONFIG REQUIRED)

add_executable(main main.cpp)
target_link_libraries(main PRIVATE libpqxx::pqxx)

