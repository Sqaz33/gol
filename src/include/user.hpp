#ifndef SRC_INCLUDE_USER_HPP
#define SRC_INCLUDE_USER_HPP

#include <iostream>
#include <string>
#include <tuple>

namespace user {

class User {
public:
    User() = default;

    User(const std::tuple<std::string, std::string>& tpl) :
        m_login(std::get<0>(tpl)),
        m_password(std::get<1>(tpl))
    {}

    User(const std::string& login, const std::string& password) noexcept :
        m_login(login),
        m_password(password)
    {}

    User(const User& other) :
        m_login(other.m_login),
        m_password(other.m_password)
    {}

    User(User&& other) noexcept :
        m_login(std::move(other.m_login)),
        m_password(std::move(other.m_password))
    {} 

    User& operator=(const User& other) {
        m_login = other.m_login;
        m_password = other.m_password;
    }

    User& operator=(User&& other) {
        m_login = std::move(other.m_login);
        m_password = std::move(other.m_password);
    }

    const std::string& login() const {
        return m_login;
    }

    const std::string& password() const {
        return m_password;
    }

    void setPassword(const std::string& newPassword) {
        m_password = newPassword;
    }
    
private:
    std::string m_login;
    std::string m_password;
};

} // namespace user


inline std::ostream& operator<<(std::ostream& out, const user::User& u) {
    std::cout << "login: " << u.login() << " ; " << "password: " << u.password();
    return out;
}

#endif // SRC_INCLUDE_USER_HPP