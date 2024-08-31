#include "include/goldb.hpp"

#include <string>
#include <format>

#include "pqxx/pqxx"

#include "include/user.hpp"

namespace sql_tools {
    void replace(std::string& str, const std::string& src, const std::string& dst) {
        for (size_t pos = str.find(src); pos != str.npos; pos = str.find(src, pos + dst.length())) {
            str.replace(pos, 1, dst);
        }
    }
}

namespace goldb {
    bool GolDB::isUserExist(const user::User& user) {
        using sql_tools::replace;
        try {
            pqxx::work tx(con);
            std::string login = user.login();
            replace(login, "'", "''");
            std::string pword = user.password();
            replace(pword, "'", "''");
            bool res = tx.query_value<bool>(
                std::format(R"(SELECT 1 FROM "Users" WHERE "login" = '{}' AND "password" = '{}')", login, pword)
            );
            return true;
        } 
        #ifdef DEBUG
        catch(const std::exception& e) {
            std::cout << e.what() << '\n';
            throw;
        }  
        #else 
        catch(...) { }
        #endif
        return false;
    }

    bool GolDB::insertUserIfNotExists(const user::User& user) {
        if (GolDB::isUserExistsWithLogin(user.login())) {
            return false;
        }
        using sql_tools::replace;
        try {
            pqxx::work tx(con);
            std::string login = user.login();
            replace(login, "'", "''");
            std::string pword = user.password();
            replace(pword, "'", "''");
            tx.exec0(std::format(R"delim(CALL add_new_user('{}', '{}'))delim", login, pword));
            return true;
        } 
        #ifdef DEBUG
        catch(const std::exception& e) {
            std::cout << e.what() << '\n';
            throw;
        }  
        #else 
        catch(...) { }
        #endif
        return false;
    }

    bool GolDB::isUserExistsWithLogin(const std::string& login) {
        using sql_tools::replace;
        try {
            pqxx::work tx(con);
            std::string logincpy = login;
            replace(logincpy, "'", "''");
            bool res = tx.query_value<bool>(
                std::format(R"(SELECT 1 FROM "Users" WHERE "login" = '{}')", logincpy)
            );
            return true;
        } 
        #ifdef DEBUG
        catch(const std::exception& e) {
            std::cout << e.what() << '\n';
            throw;
        }  
        #else 
        catch(...) { }
        #endif
        return false;
    }
} // namespace goldb