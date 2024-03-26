/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:10:02 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/26 11:30:14 by phudyka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/channel.hpp"

Channel::Channel(const std::string &name)
{
    this->_name = name;
}

Channel::Channel(const Channel&) {}

Channel::~Channel() {}

Channel&	Channel::operator=(const Channel& other)
{
    if (this != &other) {}
    return *this;
}

void	Channel::addUser(User* user)
{
    _users.push_back(user);
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

// void Channel::markAllMembers()
// {
//     for (std::vector<std::string>::const_iterator i = _users.begin(); i != _users.end(); ++i)
//         (*i)->mark();
// }

size_t	Channel::count() const
{
    return (_users.size());
}

// size_t	Channel::nbUserVisible() const
// {
//     size_t	nb = 0;

//     for (std::vector<User>::const_iterator i = _users.begin(); i != _users.end(); ++i)
// 	{
//         const UserMode&	umode = i->umode();
//         if (!umode.isSet(UserMode::INVISIBLE))
//             nb++;
//     }
//     return (nb);
// }

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

void	Channel::sendMessage(const std::string& message)
{
   _messageHistory.push_back(message);
    std::cout << "Message sent to channel '" << _name << "': " << message << std::endl;
}

// void	Channel::sendToAll(const std::string& message)
// {
//     std::vector<User*>::iterator	it;

//     for (it = users.begin(); it != users.end(); ++it)
//         (*it)->sendMessage(message);
// }

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