/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtassel <dtassel@42.nice.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:23:57 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/11 13:22:36 by dtassel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/user.hpp"

User::User(int socket, const std::string &nick, const std::string &host, const std::string &ip)
    : _socket(socket), _marked(false), _ip(ip), _host(host), _prefix(), _nickname(nick), _joinedChannels(0), _umode(NULL) {}


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

const std::string& User::getHost() const
{
    return (_host);
}

const std::string& User::getIP() const
{
    return (_ip);
}

void User::setHost(const std::string& host)
{
    _host = host;
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
