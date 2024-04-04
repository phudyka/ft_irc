/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:10:05 by phudyka           #+#    #+#             */
/*   Updated: 2024/04/04 15:42:06 by phudyka          ###   ########.fr       */
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
	std::vector<User*>			_users; // liste des utilisateurs actifs
    std::vector<std::string>    _modeChannel; // les modes du channel a implementer
    std::set<std::string>		_invitations;
    std::set<std::string>		_masks[3];
    std::map<std::string, std::string> _modeUser; // liste des utilisateurs et leur mode(dans le channel)
	bool						_inviteOnly;
    bool						_topic;
    std::string					_password;
    std::vector<std::string>	_operators;
    int							_userLimit;

public:


    Channel(const std::string &name);
    Channel(const Channel&);
    ~Channel();
    Channel& operator=(const Channel& other);
    void	addUser(User* user, const std::string &mode);
    void	addMask(MaskSetType type, const std::string& mask);
    void	invite(const User* user);
    bool	isBanned(const User* u) const;
    bool	isInvited(const User* u) const;
	bool	isInChannel(const std::string& nickname, Channel* channel);
	void	kickUser(const std::string& targetNickname, const std::string& reason);
    void	send(const std::string& msg, User* origin, bool useReceipt) const;
    void	removeUser(const std::string& username);
    const std::string getMode();
	void	sendToAll(const std::string& message);
    void	archiveMessages();
    void    setMode(const std::string &mode);
	void	setInviteOnly(bool value);
	void	setTopic(bool value);
	void	setPassword(const std::string& newPass);
	void	addOperator(const std::string& operatorName);
	void	setUserLimit(int limit);
    std::string getOperator();
    std::string getListInstring();
    std::vector<User*>	getUsers();
    size_t	count() const;
    std::string	getName();

};

#endif // CHANNEL_HPP