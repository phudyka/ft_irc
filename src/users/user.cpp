/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtassel <dtassel@42.nice.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:23:57 by phudyka           #+#    #+#             */
/*   Updated: 2024/04/25 10:57:25 by dtassel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/user.hpp"

User::User(int socket, const std::string &nick, const std::string &user, const std::string &ip)
    : _socket(socket), _marked(false), _isAuthentified(false), _isOperator(false), _passOk(false), _ip(ip), _user(user), _prefix(), _nickname(nick), _joinedChannels(0), _mode("+") {}


User::User(const User&) {}

User::~User()
{
    this->quitAllChannels();
}

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

bool	User::isAuthentified()
{
    return (_isAuthentified);
}

bool	User::isOperator()
{
	return (_isOperator);
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

void    User::setAuthentified()
{
    this->_isAuthentified = true;
}

void	User::setOperator()
{
	this->_isOperator = true;
}

size_t User::getJoinedChannels() const
{
    return (_joinedChannels);
}

const std::string   User::getMode() const
{
    return(_mode);
}

void   User::setMode(const std::string &mode)
{
    _mode = mode;
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

void    User::setNickname(const std::string &nickname)
{
    this->_nickname = nickname;
}

void	User::setJoinedChannels(Channel *channel)
{
    this->_listChannel.push_back(channel);
    this->_joinedChannels++;
}

void    User::removeListChannels(Channel *channel)
{
    std::vector<Channel*>::iterator it = _listChannel.begin();
    for (; it != _listChannel.end(); it++)
    {
        if ((*it)->getName() == channel->getName())
        {
            _listChannel.erase(it);
            this->_joinedChannels--;
            break;
        }
    }
}

void    User::quitAllChannels()
{
    std::vector<Channel*>::iterator it = this->_listChannel.begin();
    for (; it != _listChannel.end(); it++)
    {
        (*it)->removeUser(this->getNickname());
    }
    this->_listChannel.clear();
    this->_joinedChannels = 0;
}

void User::sendMessage(const std::string& message) const
{
    if (send(_socket, message.c_str(), message.length(), 0) == -1)
    {
        std::cerr << "Error sending message to user " << _nickname << std::endl;
    }
}

void    User::setPassword()
{
    _passOk = true;
}

bool    User::getAuthPass()
{
    return _passOk;
}

void    User::majBuffer(const std::string &data)
{
    size_t i = 0;
    std::string temp;
    std::string buf;
    while (_buffer[i])
    {
        if (_buffer[i] != data[i])
            break;
    }
    temp = data;
    buf = _buffer + temp;
    _buffer.clear();
    _buffer = buf;
    std::cout << buf << "/0" << std::endl;
}
