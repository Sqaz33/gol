cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(gol)

find_package(libpqxx CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(Crow CONFIG REQUIRED)

add_executable(main src/main.cpp src/goldb.cpp)
target_link_libraries(main PRIVATE 
    libpqxx::pqxx
    nlohmann_json::nlohmann_json
    Crow::Crow 
    asio::asio
)

add_custom_command(TARGET main POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/static $<TARGET_FILE_DIR:main>/static
    COMMENT "Copying static directory to the output directory"
)
