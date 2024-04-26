/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:10:05 by phudyka           #+#    #+#             */
/*   Updated: 2024/04/26 12:15:43 by phudyka          ###   ########.fr       */
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
    {        BAN_SET,
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
    std::string					_topic;
    bool                        _topicOn;
    std::string					_password;
    bool                        _passwordOn;
    std::vector<std::string>	_operators;
    int							_userLimit;

public:


    Channel(const std::string &name);
    Channel(const Channel&);
    ~Channel();
    Channel& operator=(const Channel& other);
    bool	addUser(User* user, const std::string &mode);
    void	addMask(MaskSetType type, const std::string& mask);
    void	invite(const User* user);
    bool	isBanned(const User* u) const;
    bool	isInvited(const User* user) const;
	bool	isInChannel(const std::string& nickname, Channel* channel);
	void	kickUser(User *user, const std::string& targetNickname, const std::string& reason);
    void	removeUser(const std::string& username);
    const std::string getMode();
	void	sendToAll(const std::string& message);
    void	archiveMessages();
    void    setMode(const std::string &mode);
    void    unsetMode(const std::string &mode);
	void	setInviteOnly(bool value);
	void	setTopic(const std::string& topic);
	void	setPassword(const std::string& newPass);
    bool    checkPassword(const std::string &pass);
	bool	addOperator(const std::string& operatorName);
	bool	removeOperator(const std::string& operatorName);
	void	setUserLimit(int limit);
	int		getUserLimit();
    void    enableTopic(bool value);
    bool    getStTopic();
    void    enablePass(bool value);
    bool    getStPass();
    void    majNickInChannel(const std::string &oldNickname, const std::string &newNickname);
    std::string getTopic();
    std::string getOperator(const std::string &nick);
    std::string getListInstring();
    std::vector<User*>	getUsers();
    size_t	count() const;
    std::string	getName();
};

#endif // CHANNEL_HPP