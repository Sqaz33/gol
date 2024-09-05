#ifndef REDIS_USER_CACHER_HPP
#define REDIS_USER_CACHER_HPP

#include <exception>
#include <format>
#include <memory>
#include <string>

#include "sw/redis++/redis++.h"

#include "user.hpp"

namespace redis_user_cacher {

class RedisUserCacher {
public:
    RedisUserCacher(const std::string& serverIP, std::uint16_t serverPort) :    
        con(std::format("tcp://{}:{}", serverIP, serverPort))
    {
        // try {
        //     con = std::make_unique<redis>();
        // } catch (const std::exception& e) {
        //     std::cout << e.what();
        //     throw;
        // }
    }

    RedisUserCacher(const RedisUserCacher&) = delete;
    RedisUserCacher& operator=(const RedisUserCacher&) = delete;
    RedisUserCacher(RedisUserCacher&&) = delete;
    RedisUserCacher& operator=(RedisUserCacher&&) = delete;

public:
    bool cacheUser(const user::User& user);
    bool isUserCachedWithLogin(const std::string& login);
    bool isUserCached(const user::User& user);
    user::User getUserByLogin(const std::string& login);

    decltype(auto) ping() {
        return con.ping();
    }
    
private:
    template <class T>
    using Optional = sw::redis::Optional<T>;

private:
    sw::redis::Redis con; 
};

} // namespace redis_user_hasher


#endif // REDIS_USER_CACHER_HPP