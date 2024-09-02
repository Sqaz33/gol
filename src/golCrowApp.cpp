#include "include/golCrowApp.hpp"

#include <exception>

#include "crow.h"
#include "boost/asio.hpp"

#include "include/user.hpp"

namespace gol_crow_app {

void GolCrowApp::run() {
    app
    .bindaddr(serverIP)
    .port(serverPort)
    .multithreaded()
    .run();
}

void GolCrowApp::setupRoutes() {
    CROW_ROUTE(app, "/")([](){
        crow::response res;
        res.set_static_file_info("static/index.html");
        return res;
    });

    CROW_ROUTE(app, "/login")
    .methods(crow::HTTPMethod::Get)([](){
        crow::response res;
        res.set_static_file_info("static/login.html");
        return res;
    });

    CROW_ROUTE(app, "/login") 
    .methods(crow::HTTPMethod::Post)([&](const crow::request& req){
        auto hostIP = boost::asio::ip::make_address_v4(req.remote_ip_address).to_uint();
        if (hostsIP.count(hostIP)) {
            return "it's ok";
        }
        if (GolCrowApp::verifyUserByJsonPostReq(req)) {
            hostsIP.insert(hostIP);
            return "it's ok";
        } 
        return "dont know you";
    });


    
    CROW_ROUTE(app, "/register")
    .methods(crow::HTTPMethod::Get)([](){
        crow::response res;
        res.set_static_file_info("static/register.html");
        return res;
    });

    CROW_ROUTE(app, "/register") 
    .methods(crow::HTTPMethod::Post)([&](const crow::request& req){
        auto hostIP = boost::asio::ip::make_address_v4(req.remote_ip_address).to_uint();
        if (hostsIP.count(hostIP)) {
            return "it's ok";
        }
        if (GolCrowApp::registerUserByJsonPostReq(req)) {
            hostsIP.insert(hostIP);
            return "it's ok";
        } 
        return "not ok";
    });


    CROW_CATCHALL_ROUTE(app)([&]() {
        return "nothing here";
    });

}


bool GolCrowApp::verifyUserByJsonPostReq(const crow::request& req) {
    auto data = crow::json::load(req.body);
    if (!data) {
        #ifdef DEBUG
        throw (std::format_error("no json in body"));
        #endif
        return false;
    }
    std::string login = data["login"].s();
    std::string pword = data["pword"].s();
    return db->isUserExist(user::User(login, pword));
}

bool GolCrowApp::registerUserByJsonPostReq(const crow::request& req) {
    auto data = crow::json::load(req.body);
    if (!data) {
        #ifdef DEBUG
        throw (std::format_error("no json in body"));
        #endif
        return false;
    }
    std::string login = data["login"].s();
    std::string pword = data["pword"].s();
    return db->registerUser(user::User(login, pword));
}

} // namespace gol_crow_app

