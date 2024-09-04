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
                std::format(
                    R"delim(SELECT 
                                CASE 
                                    WHEN EXISTS (
                                        SELECT 1 
                                        FROM "Users" 
                                        WHERE "login" = '{}' AND "password" = '{}'
                                    ) 
                                    THEN 1 
                                    ELSE 0 
                                END AS user_exists;
                            )delim"
                    , login, pword)
            );
            return res;
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
            tx.commit();
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
                std::format(
                    R"delim(SELECT 
                                CASE 
                                    WHEN EXISTS (
                                        SELECT 1 
                                        FROM "Users" 
                                        WHERE "login" = '{}'
                                    ) 
                                    THEN 1 
                                    ELSE 0 
                                END AS user_exists;
                            )delim"
                    , logincpy)
            );
            return res;
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

    bool GolDB::registerUser(const user::User& user) {
        using sql_tools::replace;
        if (GolDB::isUserExistsWithLogin(user.login())) {
            return false;
        }
        try {
            pqxx::work tx(con);
            std::string login = user.login();
            replace(login, "'", "''");
            std::string pword = user.password();
            replace(pword, "'", "''");
            tx.exec0(std::format(R"delim(INSERT INTO "Users"("login", "password") 
	                                VALUES ('{}', '{}');)delim", 
                                    login, pword));
            tx.commit();
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