#include <iostream>
#include <fstream>
#include <string>
#include <format>
#include <memory>
#include <exception>

#include "nlohmann/json.hpp"

#include "include/goldb.hpp"
#include "include/user.hpp"
#include "include/golCrowServer.hpp"

int main() {
    SetConsoleOutputCP(CP_UTF8); 
    std::ifstream jn("config.json");

    nlohmann::json data = nlohmann::json::parse(jn);
    
    std::string dbname = data["dbname"];
    std::string user = data["userlogin"];
    std::string pword = data["userpword"];
    std::string redisIP = data["redisServerIP"];
    std::uint16_t redisPort = data["redisServerPort"];
    std::string serverIP = data["serverIP"];
    std::uint16_t serverPort = data["serverPort"];

    auto db = std::make_shared<goldb::GolDB>(
        goldb::UseRedisIsAlive(),
        dbname, 
        user, 
        pword, 
        redisIP, 
        redisPort
    );


    gol_crow_server::GolCrowServer app(serverIP, serverPort, db);
    app.run();

}