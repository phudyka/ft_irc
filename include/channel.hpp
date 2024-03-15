/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtassel <dtassel@42.nice.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:10:05 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/14 16:02:56 by dtassel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CHANNEL_HPP
#define CHANNEL_HPP

#include "ft_irc.hpp"
#include "user.hpp"

class	User;

class	Channel
{
private:
    enum	MaskSetType
    {
        BAN_SET,
        EXCEPTION_SET,
        INVITATION_SET
    };

    std::string	                _name;
	std::vector<User*>			_users;
    std::set<std::string>		_invitations;
    std::set<std::string>		_masks[3];
    std::vector<std::string>	_messageHistory;

public:


    Channel(const std::string &name);
    Channel(const Channel&);
    ~Channel();
    Channel& operator=(const Channel& other);
	void markAllMembers();
    void addUser(User* user);
    void addMask(MaskSetType type, const std::string& mask);
    void invite(const User* user);
    bool isBanned(const User* u) const;
    bool isInvited(const User* u) const;
    void send(const std::string& msg, User* origin, bool useReceipt) const;
    void removeUser(const std::string& username);
    void sendMessage(const std::string& message);
    void archiveMessages();
    std::vector<User*> getUsers();
    size_t count() const;
    size_t nbUserVisible() const;
    std::string getName();

};

#endif // CHANNEL_HPP
