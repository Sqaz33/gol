#ifndef SRC_INCLUDE_GOL_CROW_APP_HP
#define SRC_INCLUDE_GOL_CROW_APP_HP

#include <string>
#include <memory>
#include <unordered_set>

#include "crow.h"

#include "goldb.hpp"

namespace gol_crow_app {

class GolCrowApp {
public:
    GolCrowApp(const std::string& serverIP, std::uint8_t serverPort, std::shared_ptr<goldb::GolDB> db) :
        serverIP(serverIP)
        , serverPort(serverPort)
        , db(db)
    {
        setupRoutes();
    }

    GolCrowApp(const GolCrowApp&) = delete;
    GolCrowApp& operator=(const GolCrowApp&) = delete;
    GolCrowApp(GolCrowApp&&) = delete; 
    GolCrowApp& operator=(GolCrowApp&&) = delete;

    void run();

private:
    std::string serverIP;
    std::uint8_t serverPort;
    crow::SimpleApp app; 
    std::shared_ptr<goldb::GolDB> db;
    std::unordered_set<std::uint32_t> hostsIP;

private:
    void setupRoutes();
    bool verifyUserByJsonPostReq(const crow::request& req);
    bool registerUserByJsonPostReq(const crow::request& req);
};

} // namespace gol_crow_app

#endif // SRC_INCLUDE_GOL_CROW_APP_HP