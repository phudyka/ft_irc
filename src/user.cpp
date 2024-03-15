/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:23:57 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/15 11:33:21 by phudyka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/user.hpp"

User::User(int socket, const std::string &nick, const std::string &user, const std::string &ip)
    : _socket(socket), _marked(false), _ip(ip), _user(user), _prefix(), _nickname(nick), _joinedChannels(0), _umode(NULL) {}


User::User(const User&) {}

User::~User() {}

User& User::operator=(const User& other)
{
    if (this != &other) {}
    return *this;
}

const std::string& User::getNickname() const
{
    return (_nickname);
}

const std::string& User::getPrefix() const
{
    return (_prefix);
}

const std::string& User::getUser() const
{
    return (_user);
}

const std::string& User::getIP() const
{
    return (_ip);
}

const std::string User::getUsername() const
{
    return this->_username;
}

const std::string User::getRealname() const
{
    return this->_realname;
}

const std::string User::getHostname() const
{
    return this->_hostname;
}

void    User::setUsername(const std::string &username)
{
    this->_username = username;
}

void    User::setRealname(const std::string &realname)
{
    this->_realname = realname;
}
void    User::setHostname(const std::string &hostname)
{
    this->_hostname = hostname;
}

void User::setIP(const std::string& ip)
{
    _ip = ip;
}

size_t User::getJoinedChannels() const
{
    return (_joinedChannels);
}

int User::getSocket() const
{
    return (_socket);
}

void User::mark()
{
    _marked = true;
}

bool User::isMarked() const
{
    return (_marked);
}

UserMode& User::umode()
{
    return (*_umode);
}

void    User::setNickname(const std::string &nickname)
{
    this->_nickname = nickname;
}

void User::sendMessage(const std::string& message) const
{
    if (send(_socket, message.c_str(), message.length(), 0) == -1)
    {
        std::cerr << "Error sending message to user " << _nickname << std::endl;
    }
}

