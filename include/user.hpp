/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtassel <dtassel@42.nice.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:20:13 by phudyka           #+#    #+#             */
/*   Updated: 2024/04/25 10:05:58 by dtassel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USER_HPP
#define USER_HPP

#include "../include/ft_irc.hpp"

class Channel;

class User
{
private:

    int		    _socket;
    bool	    _marked;
    bool        _isAuthentified;
    bool        _isOperator;
    bool        _passOk;
	std::string	_ip;
	std::string	_user;
    std::string	_prefix;
    std::string	_nickname;
	std::string	_pass;
    size_t		_joinedChannels;
    std::vector<Channel*> _listChannel;
    std::string _mode;

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
    const std::string	getUsername() const;
    const std::string	getRealname() const;
    const std::string	getHostname() const;
    const std::string   getMode() const;
    bool                getAuthPass();
    bool				isAuthentified();
	bool				isOperator();
    const std::string getPass() const;
    void    setUsername(const std::string &username);
    void    setRealname(const std::string &realname);
    void    setHostname(const std::string &hostname);
    void    setNickname(const std::string &nickname);
    void    setMode(const std::string &mode);
    void	setJoinedChannels(Channel *channel);
    void    removeListChannels(Channel *channel);
    void    quitAllChannels();
    void    setPassword();
    void    setAuthentified();
	void	setOperator();
    size_t	getJoinedChannels() const;
    void	setIP(const std::string& ip);
    void    sendMessage(const std::string& message) const;
    void    majBuffer(const std::string &data);
    std::string temp_USER;
    std::string _buffer;
};

#endif