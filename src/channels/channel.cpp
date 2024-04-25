/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtassel <dtassel@42.nice.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:10:02 by phudyka           #+#    #+#             */
/*   Updated: 2024/04/25 09:20:40 by dtassel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/channel.hpp"

Channel::Channel(const std::string &name) : _name(name), _inviteOnly(false), _topic("none"), _userLimit(-1)
{
    _modeChannel.push_back("+n");
    _topicOn = false;
    _passwordOn = false;
}

Channel::Channel(const Channel&) {}

Channel::~Channel() {}

Channel&	Channel::operator=(const Channel& other)
{
    if (this != &other) {}
    return *this;
}

bool	Channel::addUser(User* user, const std::string &mode)
{
    if (_inviteOnly == false)
    {
        _users.push_back(user);
        _modeUser.insert(std::make_pair(user->getNickname(), mode));
        return true;
    }
    else if (_inviteOnly == true)
    {
        if (isInvited(user) == true)
        {
            _users.push_back(user);
            _modeUser.insert(std::make_pair(user->getNickname(), mode));
            std::set<std::string>::iterator it = _invitations.begin();
            for (; it != _invitations.end(); it++)
            {
                if (*it == user->getNickname())
                    break;
            }
            _invitations.erase(it);
            return true;
        }
    }
    return false;
}

void	Channel::invite(const User* user)
{
    bool exist = false;
    std::set<std::string>::iterator it = _invitations.begin();
    for (; it != _invitations.end(); it++)
    {
        if (*it == user->getNickname())
            exist = true;
    }
    if (exist == false)
        _invitations.insert(user->getNickname());
}

bool	Channel::isBanned(const User* u) const
{
    return (_masks[BAN_SET].count(u->getPrefix()) && !_masks[EXCEPTION_SET].count(u->getPrefix()));
}

bool	Channel::isInvited(const User* user) const
{
    std::set<std::string>::iterator it = _invitations.begin();
    for (; it != _invitations.end(); it++)
    {
        if (*it == user->getNickname())
            return true;
    }
    return false;
}

size_t	Channel::count() const
{
    return (_users.size());
}

bool	Channel::isInChannel(const std::string& nickname, Channel* channel)
{
    for (size_t i = 0; i < channel->getUsers().size(); ++i)
    {
        if (channel->getUsers()[i]->getNickname() == nickname)
            return (true);
    }
    return (false);
}

void	Channel::kickUser(User *user, const std::string& targetNickname, const std::string& reason)
{
    for (std::vector<User*>::iterator it = _users.begin(); it != _users.end(); ++it)
    {
        if ((*it)->getNickname() == targetNickname)
        {
            std::string response = RPL_KICK(user_id(user->getNickname(), user->getUsername()), _name, targetNickname, reason);
            send((*it)->getSocket(), response.c_str(), response.length(), 0);
            _users.erase(it);
            break;
        }
    }
}

void Channel::removeUser(const std::string& username)
{
    for (std::vector<User*>::iterator it = _users.begin(); it != _users.end(); ++it)
    {
        if ((*it)->getNickname() == username)
        {
            std::string reason = "";
            std::string response = RPL_PART(user_id((*it)->getNickname(), (*it)->getUsername()), this->getName(), reason);
            response += RPL_ENDOFNAMES((*it)->getNickname(), this->getName());
            sendToAll(response);
            _users.erase(it);
            break;
        }
    }
}


void Channel::sendToAll(const std::string& message)
{
    for (std::vector<User*>::iterator it = _users.begin(); it != _users.end(); ++it)
        (*it)->sendMessage(message);
}

void	Channel::archiveMessages()
{
    std::cout << "Archiving messages for channel '" << _name << "'" << std::endl;
}

std::string	Channel::getName()
{
    return this->_name;
}

std::vector<User*>	Channel::getUsers()
{
    return _users;
}

std::string Channel::getListInstring()
{
    std::vector<User *>::iterator it = this->_users.begin();
    std::string listUsers;

    for (; it != _users.end(); it++)
    {
        if (_modeUser.find((*it)->getNickname()) != _modeUser.end())
        {
            std::cout << "Mode de l'utilisateur " << (*it)->getNickname() << " : " << _modeUser[(*it)->getNickname()] << std::endl;
            if (_modeUser[(*it)->getNickname()] != "+o")
                listUsers += (*it)->getNickname() + " ";
            else
                listUsers += "@" + (*it)->getNickname() + " ";
        }
    }
    return listUsers;
}

const std::string Channel::getMode()
{
    std::string list_mode;
    std::vector<std::string>::iterator it = _modeChannel.begin();
    for (; it != _modeChannel.end(); it++)
    {
        list_mode += *it;
    }
    std::cout << "resultat getMode : " << list_mode << std::endl;
    return list_mode;
}

void    Channel::setMode(const std::string &mode)
{
    bool exist = false;
    std::vector<std::string>::iterator it = _modeChannel.begin();
    char modeChar = mode[1];
    for (; it != _modeChannel.end(); it++)
    {
        if (std::string(1, modeChar) == (*it))
            exist = true;
    }
    if (exist == false)
        _modeChannel.push_back(std::string(1, modeChar));
    return ;
}

void Channel::unsetMode(const std::string &mode)
{
    char modeChar = mode[1];
    for (std::vector<std::string>::iterator it = _modeChannel.begin(); it != _modeChannel.end(); ++it)
	{
        if (*it == std::string(1, modeChar))
		{
            _modeChannel.erase(it);
            return ;
        }
    }
}

std::string Channel::getOperator(const std::string &nick)
{
    std::map<std::string, std::string>::iterator it = _modeUser.begin();
    for (; it != _modeUser.end(); it++)
    {
        if (it->first == nick && it->second == "+o")
            return it->first;
    }
    return ("");
}

void	Channel::setInviteOnly(bool value)
{
	_inviteOnly = value;
}

void	Channel::setTopic(const std::string& topic)
{
	_topic = topic;
}

void    Channel::enableTopic(bool value)
{
    _topicOn = value;
}

bool    Channel::getStTopic()
{
    return _topicOn;
}

std::string Channel::getTopic()
{
    return _topic;
}

void	Channel::setPassword(const std::string& newPass)
{
	_password = newPass;
}

bool    Channel::checkPassword(const std::string &pass)
{
    if (pass == _password)
        return true;
    return false;
}

void    Channel::enablePass(bool value)
{
    _passwordOn = value;
}

bool    Channel::getStPass()
{
    return _passwordOn;
}

bool	Channel::addOperator(const std::string& operatorName)
{
	std::map<std::string, std::string>::iterator it = _modeUser.begin();
    for (; it != _modeUser.end(); it++)
    {
        if (it->first == operatorName)
        {
            it->second = "+o";
            return true;
        }
    }
    return false;
}

bool Channel::removeOperator(const std::string& operatorName)
{
    std::map<std::string, std::string>::iterator it = _modeUser.find(operatorName);
    if (it != _modeUser.end())
    {
        it->second = "-o";
        return true;
    }
    return false;
}

void	Channel::setUserLimit(int limit)
{
	_userLimit = limit;
}

int	Channel::getUserLimit()
{
	return _userLimit;
}

void Channel::majNickInChannel(const std::string& oldNickname, const std::string& newNickname)
{
    std::map<std::string, std::string>::iterator it = _modeUser.find(oldNickname);
    
    if (it != _modeUser.end())
    {
        _modeUser.insert(std::make_pair(newNickname, it->second));
        _modeUser.erase(it);
    }
}
