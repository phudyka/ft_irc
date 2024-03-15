/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtassel <dtassel@42.nice.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:20:13 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/15 08:51:26 by dtassel          ###   ########.fr       */
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
	std::string	_host;
    std::string	_prefix;
    std::string	_nickname;
	std::string	_pass;
    size_t		_joinedChannels;
    UserMode*	_umode;

    std::string _username;
    std::string _realname;
    std::string _hostname;

public:

    User(int socket, const std::string &nick, const std::string &host, const std::string &ip);
    User(const User& other);
    ~User();
    User& operator=(const User& other);
    int		getSocket() const;
    void	mark();
    bool	isMarked() const;
    const std::string&	getNickname() const;
    const std::string&	getPrefix() const;
	const std::string&	getHost() const;
    const std::string&	getIP() const;
    const std::string getUsername() const;
    const std::string getRealname() const;
    const std::string getHostname() const;
    void    setUsername(const std::string &username);
    void    setRealname(const std::string &realname);
    void    setHostname(const std::string &hostname);
    void    setNickname(const std::string &nickname);
    size_t	getJoinedChannels() const;
	void	setHost(const std::string& host);
    void	setIP(const std::string& ip);
    void    sendMessage(const std::string& message) const;
    UserMode&	umode();
};

#endif // USER_HPP