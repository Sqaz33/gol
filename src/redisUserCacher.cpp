#include "include/redisUserCacher.hpp"

#include <string>

#include "sw/redis++/redis++.h"

#include "include/user.hpp"

namespace redis_user_cacher {
    bool RedisUserCacher::cacheUser(const user::User& user) {
        if (RedisUserCacher::isUserCachedWithLogin(user.login())) {
            return false;
        }
        con.set(user.login(), user.password());
        return true;
    }

    bool RedisUserCacher::isUserCached(const user::User& user) {
        RedisUserCacher::Optional<std::string> p = con.get(user.login());
        return p.value() == user.password();
    }

    bool RedisUserCacher::isUserCachedWithLogin(const std::string& login) {
        RedisUserCacher::Optional<std::string> p = con.get(login);
        return static_cast<bool>(p);
    }

    user::User RedisUserCacher::getUserByLogin(const std::string& login) {
        RedisUserCacher::Optional<std::string> p = con.get(login);
        return user::User(login, p.value());
    }

} // redis_user_cacher