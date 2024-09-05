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
            if (redis && redis->isUserCached(user)) {
                return true;
            }

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

            if (redis && res) {
                redis->cacheUser(user);
            }

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
        using sql_tools::replace;

        if (redis && redis->isUserCachedWithLogin(user.login())) {
            return false;
        }

        if (GolDB::isUserExistsWithLogin(user.login())) {
            if (redis) {
                redis->cacheUser(user);
            }
            return false;
        }
        try {
            pqxx::work tx(con);
            std::string login = user.login();
            replace(login, "'", "''");
            std::string pword = user.password();
            replace(pword, "'", "''");
            tx.exec0(std::format(R"delim(CALL add_new_user('{}', '{}'))delim", login, pword));
            tx.commit();

            if (redis) {
                redis->cacheUser(user);
            }

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
        if (redis && redis->isUserCachedWithLogin(login)) {
            return true;
        }
        try {
            pqxx::work tx(con);
            std::string logincpy = login;
            sql_tools::replace(logincpy, "'", "''");
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

            if (res && redis) {
                redis->cacheUser(GolDB::getUserByLogin(login));
            }

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

    user::User GolDB::getUserByLogin(const std::string& login) {
        if (redis && redis->isUserCachedWithLogin(login)) {
            return redis->getUserByLogin(login);
        }

        try {
            pqxx::work tx(con);
            std::string logincpy = login;
            sql_tools::replace(logincpy, "'", "''");
            std::string pword = tx.query_value<std::string>(
                std::format(
                    R"delim(SELECT "password" FROM "Users" WHERE "login" ={})delim",
                    logincpy
                )
            );
            user::User user(login, pword);

            if (redis) {
                redis->cacheUser(user);
            }

            return user;
        }
        #ifdef DEBUG
        catch(const std::exception& e) {
            std::cout << e.what() << '\n';
            throw;
        }  
        #else 
        catch(...) { }
        #endif
        return user::User();
    }

} // namespace goldb