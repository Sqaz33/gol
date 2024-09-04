#include <iostream>
#include <fstream>
#include <string>
#include <format>
#include <memory>

#include "nlohmann/json.hpp"

#include "include/goldb.hpp"
#include "include/user.hpp"
#include "include/golCrowApp.hpp"

int main() {
    SetConsoleOutputCP(CP_UTF8); 
    std::ifstream jn("config.json");

    nlohmann::json data = nlohmann::json::parse(jn);
    
    std::string dbname = data["dbname"];
    std::string user = data["userlogin"];
    std::string pword = data["userpword"];
    
    std::string redisIP = data["redisServerIP"];
    std::uint16_t redisPort = data["redisServerPort"];

    auto db = std::make_shared<goldb::GolDB>(
        dbname, user, pword, redisIP, redisPort
    );

    std::string serverIP = data["serverIP"];
    std::uint16_t serverPort = data["serverPort"];
    gol_crow_app::GolCrowApp app(serverIP, serverPort, db);
    app.run();

}