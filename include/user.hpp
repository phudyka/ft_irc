/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtassel <dtassel@42.nice.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:20:13 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/08 10:27:05 by dtassel          ###   ########.fr       */
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
    std::string	_prefix;
    std::string	_nickname;
    size_t		_joinedChannels;
    UserMode*	_umode;

public:

    User(int socket, const std::string &nick);
    User(const User& other);
    ~User();
    User& operator=(const User& other);

    int		getSocket() const;
    void	mark();
    bool	isMarked() const;
    const std::string&	getNickname() const;
    const std::string&	getPrefix() const;
    size_t	getJoinedChannels() const;
    UserMode&	umode();
};

#endif // USER_HPP