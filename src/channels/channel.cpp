/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:10:02 by phudyka           #+#    #+#             */
/*   Updated: 2024/04/04 15:40:31 by phudyka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/channel.hpp"

Channel::Channel(const std::string &name) : _name(name), _inviteOnly(false), _topic(false), _userLimit(0)
{
    _modeChannel.push_back("+n");
}

Channel::Channel(const Channel&) {}

Channel::~Channel() {}

Channel&	Channel::operator=(const Channel& other)
{
    if (this != &other) {}
    return *this;
}

void	Channel::addUser(User* user, const std::string &mode)
{
    _users.push_back(user);
    _modeUser.insert(std::make_pair(user->getNickname(), mode));
}

void	Channel::addMask(MaskSetType type, const std::string& mask)
{
    _masks[type].insert(mask);
}

void	Channel::invite(const User* user)
{
    _invitations.insert(user->getNickname());
}

bool	Channel::isBanned(const User* u) const
{
    return (_masks[BAN_SET].count(u->getPrefix()) && !_masks[EXCEPTION_SET].count(u->getPrefix()));
}

bool	Channel::isInvited(const User* u) const
{
    return (_invitations.count(u->getNickname()) || _masks[INVITATION_SET].count(u->getPrefix()));
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

void	Channel::kickUser(const std::string& targetNickname, const std::string& reason)
{
    for (std::vector<User*>::iterator it = _users.begin(); it != _users.end(); ++it)
    {
        if ((*it)->getNickname() == targetNickname)
        {
            _users.erase(it);
            (*it)->sendMessage("You have been kicked from #" + _name + " by " + reason);
            break;
        }
    }
}

void	Channel::removeUser(const std::string& username)
{
    for (std::vector<User*>::iterator it = _users.begin(); it != _users.end(); ++it)
    {
        if ((*it)->getNickname() == username)
        {
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
        list_mode += *it + " ";
    }
    std::cout << "resultat getMode : " << list_mode << std::endl;
    return list_mode;
}

void    Channel::setMode(const std::string &mode)
{
    bool exist = false;
    std::vector<std::string>::iterator it = _modeChannel.begin();
    for (; it != _modeChannel.end(); it++)
    {
        if (mode == (*it))
            exist = true;
    }
    if (exist == false)
        _modeChannel.push_back(mode);
    return;
}

void	Channel::setInviteOnly(bool value)
{
	_inviteOnly = value;
}

void	Channel::setTopic(bool value)
{
    _topic = value;
}

void	Channel::setPassword(const std::string& newPass)
{
	_password = newPass;
}

void	Channel::addOperator(const std::string& operatorName)
{
	_operators.push_back(operatorName);
}

void	Channel::setUserLimit(int limit)
{
	_userLimit = limit;
}