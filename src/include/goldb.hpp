#ifndef SRC_INCLUDE_GOLDB_HPP
#define SRC_INCLUDE_GOLDB_HPP

#include <string>

#include "pqxx/pqxx"

#include "user.hpp"

namespace goldb {

class GolDB {
public:
    GolDB(const std::string& con) :
        con(con)
    {}

    GolDB(const GolDB&) = delete;
    GolDB& operator=(const GolDB&) = delete;

    GolDB(GolDB&& other) :
        con(std::move(other.con))
    {}

    GolDB& operator=(GolDB&& other) {
        con = std::move(other.con);
    }

public:
    /**
     * @brief Проверяет, существует ли пользователь с указанным логином и паролем в базе данных.
     * 
     * @param user Объект пользователя.
     * @return true Если пользователь с таким логином и паролем существует.
     * @return false Если пользователь с таким логином и паролем не найден.
     */
    bool isUserExist(const user::User& user);

    /**
     * @brief Вставляет нового пользователя, если не существует такового с тем же логином.
     * 
     * @param user Объект пользователя.
     * @return true Если удалось вставить нового пользователя.
     * @return false Если не удалось вставить нового пользователя.
     */
    bool insertUserIfNotExists(const user::User& user);
    
    /**
     * @brief Проверяет, существует ли пользователь с указанным логином в базе данных.
     * 
     * @param user Объект пользователя.
     * @return true Если пользователь с таким логином существует.
     * @return false Если пользователь с таким логином не найден.
     */
    bool isUserExistsWithLogin(const std::string& login);

private:
    pqxx::connection con;
};

} // namespace goldb

#define DEBUG
#endif // SRC_INCLUDE_GOLDB_HPP