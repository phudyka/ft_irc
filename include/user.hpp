/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:20:13 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/07 15:21:47 by phudyka          ###   ########.fr       */
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

    int		socket;
    bool	marked;
    std::string	prefix;
    std::string	nickname;
    size_t		joinedChannels;
    UserMode*	_umode;

public:

    User();
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