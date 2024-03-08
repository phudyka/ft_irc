/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:23:57 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/08 10:11:41 by phudyka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/user.hpp"

User::User() : socket(0), marked(false), prefix(), nickname(), joinedChannels(0), _umode(NULL) {}

User::User(const User&) {}

User::~User() {}

User& User::operator=(const User& other)
{
    if (this != &other) {}
    return *this;
}

const std::string& User::getNickname() const
{
    return (nickname);
}

const std::string& User::getPrefix() const
{
    return (prefix);
}

size_t User::getJoinedChannels() const
{
    return (joinedChannels);
}

int User::getSocket() const
{
    return (socket);
}

void User::mark()
{
    marked = true;
}

bool User::isMarked() const
{
    return (marked);
}

UserMode& User::umode()
{
    return (*_umode);
}
