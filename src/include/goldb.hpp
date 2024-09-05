#ifndef SRC_INCLUDE_GOLDB_HPP
#define SRC_INCLUDE_GOLDB_HPP

#include <string>
#include <format>
#include <iostream>
#include <optional>

#include "pqxx/pqxx"
#include "sw/redis++/redis++.h"

#include "user.hpp"
#include "redisUserCacher.hpp"

namespace goldb {

struct UseRedisIsAlive {};

/**
 * @brief Class for postgres db interaction.
 */
class GolDB {
public:
    //TODO: add try catch for connections constructions 
    GolDB(
        const std::string& DBName,
        const std::string& DBUser,
        const std::string& DBPword,
        const std::string& redisIP, 
        std::uint16_t redisPort
    ) :
        con(std::format("dbname={} user={} password={}", DBName, DBUser, DBPword)),
        redis(std::in_place_t(), redisIP, redisPort)
    {   
        try {
            redis->ping();
        } catch (const std::exception& e) {
            std::cout << e.what() << '\n';
            throw;
        }
    }

    GolDB(
        const UseRedisIsAlive&,
        const std::string& DBName,
        const std::string& DBUser,
        const std::string& DBPword,
        const std::string& redisIP, 
        std::uint16_t redisPort
    ) :
        con(std::format("dbname={} user={} password={}", DBName, DBUser, DBPword)),
        redis(std::in_place_t(), redisIP, redisPort)
    {   
        if (!redis->isConnectionAlive()) {
            redis.reset();
        }
    }

    GolDB(
        const std::string& DBName,
        const std::string& DBUser,
        const std::string& DBPword
    ):
        con(std::format("dbname={} user={} password={}", DBName, DBUser, DBPword))
    {}

    GolDB(const GolDB&) = delete;
    GolDB& operator=(const GolDB&) = delete;
    GolDB(GolDB&&) = delete;
    GolDB& operator=(GolDB&&) = delete;

public:
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
     * @brief Проверяет, существует ли пользователь с указанным логином и паролем в базе данных.
     * 
     * @param user Объект пользователя.
     * @return true Если пользователь с таким логином и паролем существует.
     * @return false Если пользователь с таким логином и паролем не найден.
     */
    bool isUserExist(const user::User& user);

    user::User getUserByLogin(const std::string& login);

private:
    pqxx::connection con;
    std::optional<redis_user_cacher::RedisUserCacher> redis;
};

} // namespace goldb

#define DEBUG
#endif // SRC_INCLUDE_GOLDB_HPP