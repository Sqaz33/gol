#ifndef SRC_INCLUDE_GOLDB_HPP
#define SRC_INCLUDE_GOLDB_HPP

#include <string>
#include <format>

#include "pqxx/pqxx"
#include "sw/redis++/redis++.h"

#include "user.hpp"

namespace goldb {

/**
 * @brief class for postgres db interaction
 * 
 */
class GolDB {
public:
    GolDB(
        const std::string& DBName,
        const std::string& DBUser,
        const std::string& DBPword,
        const std::string& redisIP, 
        std::uint8_t redisPort
    ) :
        con(std::format("dbname={} user={} password={}", DBName, DBUser, DBPword)),
        redis(std::format("tcp://{}:{}", redisIP, redisPort))
    {}

    GolDB(const GolDB&) = delete;
    GolDB& operator=(const GolDB&) = delete;
    GolDB(GolDB&&) = delete;
    GolDB& operator=(GolDB&&) = delete;

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

    /**
     * @brief Заносит пользователя с его логином и паролем в базу данных.
     * 
     * @param user Объект пользователя.
     * @return true Если удалось зарегистрировать пользователя.
     * @return false Если не удалось зарегистрировать пользователя.
     */
    bool registerUser(const user::User& user);

private:
    pqxx::connection con;
    sw::redis::Redis redis;
};

} // namespace goldb

#define DEBUG
#endif // SRC_INCLUDE_GOLDB_HPP