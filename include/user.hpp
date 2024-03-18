/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:20:13 by phudyka           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/03/15 16:07:55 by dtassel          ###   ########.fr       */
=======
/*   Updated: 2024/03/15 11:31:16 by phudyka          ###   ########.fr       */
>>>>>>> 6832a67cfd9c2bdbd3612ba0759c5adc97c17e7b
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include "../include/ft_irc.hpp"
#include "../include/userMode.hpp"

class UserMode;
class Channel;

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
    std::vector<Channel*> _listChannel;
    UserMode*	_umode;

    std::string _username;
    std::string _realname;
    std::string _hostname;

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
    const std::string getUsername() const;
    const std::string getRealname() const;
    const std::string getHostname() const;
    const std::string getPass() const;
    void    setUsername(const std::string &username);
    void    setRealname(const std::string &realname);
    void    setHostname(const std::string &hostname);
    void    setNickname(const std::string &nickname);
<<<<<<< HEAD
    void	setJoinedChannels(Channel *channel);
=======
    void    setPassword(const std::string &nickname);
>>>>>>> 6832a67cfd9c2bdbd3612ba0759c5adc97c17e7b
    size_t	getJoinedChannels() const;
	void	setUserName(const std::string& user);
    void	setIP(const std::string& ip);
    void    sendMessage(const std::string& message) const;
    UserMode&	umode();
};

#endif // USER_HPP