/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:20:13 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/14 10:16:37 by phudyka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include "../include/ft_irc.hpp"
#include "../include/userMode.hpp"

class UserMode;

class User
{
private:

    int		    _socket;
    bool	    _marked;
	std::string	_ip;
	std::string	_user;
    std::string	_prefix;
    std::string	_nickname;
	std::string	_pass;
    size_t		_joinedChannels;
    UserMode*	_umode;

public:

    User(int socket, const std::string &nick, const std::string &user, const std::string &ip);
    User(const User& other);
    ~User();
    User& operator=(const User& other);
    int		getSocket() const;
    void	mark();
    bool	isMarked() const;
    const std::string&	getNickname() const;
    const std::string&	getPrefix() const;
	const std::string&	getUser() const;
    const std::string&	getIP() const;
    void    setNickname(const std::string &nickname);
    size_t	getJoinedChannels() const;
	void	setUserName(const std::string& user);
    void	setIP(const std::string& ip);
    UserMode&	umode();
};

#endif // USER_HPP