#include <iostream>
#include <fstream>
#include <string>
#include <format>
#include <memory>

#include "nlohmann/json.hpp"
#include "crow.h"

#include "include/goldb.hpp"
#include "include/user.hpp"
#include "include/golCrowApp.hpp"

int main() {
    SetConsoleOutputCP(CP_UTF8); 
    std::ifstream jn("D:/gol/config.json");

    nlohmann::json data = nlohmann::json::parse(jn);

    std::string dbname = data["dbname"];
    std::string user = data["userlogin"];
    unsigned pword = data["userpword"];

    auto db = std::make_shared<goldb::GolDB>(
        std::format("dbname={} user={} password={}", dbname, user, pword)
    );

    gol_crow_app::GolCrowApp app("192.168.0.102", 80, db);
    app.run();

}