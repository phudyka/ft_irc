/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtassel <dtassel@42.nice.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 11:37:59 by phudyka           #+#    #+#             */
/*   Updated: 2024/04/03 11:43:18 by dtassel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_irc.hpp"

bool    Command::searchChannelName(std::string channelName, std::vector<Channel*> &channel)
{
    std::vector<Channel*>::iterator it = channel.begin();
    std::string channelName = parameters[0];
    channelName = extractParameter(channelName, "#");
    for (; it != channel.end(); it++)
    {
        if ((*it)->getName() == channelName)
        {
            return true;
        }
    }
    return false;
}

void	Command::channelMode(User *user, std::vector<Channel*> &channel)
{
    if (!parameters[1].empty())
    {
        if (parameters[1].find("+i\r\n") || parameters[1].find("+t\r\n") || parameters[1].find("+k\r\n")
            || parameters[1].find("+o\r\n") || parameters[1].find("+l\r\n"))
        {
            
        }
    }
	std::vector<Channel*>::iterator it = channel.begin();
    std::string channelName = parameters[0];
    channelName = extractParameter(channelName, "#");
    for (; it != channel.end(); it++)
    {
        if ((*it)->getName() == channelName)
        {
            std::string response = RPL_CHANNELMODEIS(user->getNickname(), channelName, (*it)->getMode());
            send(user->getSocket(), response.c_str(), response.length(), 0);
            return ;
        }
    }
}

void	Command::userMode(User *user, std::vector<Channel *> &channels)
{
    (void)channels;
    if (parameters[1].empty())
        return;
	else
    {
        user->setMode(parameters[1].substr(0, parameters[1].length() -2));
        std::string	response = MODE_USERMSG(user->getNickname(), user->getMode());
        send(user->getSocket(), response.c_str(), response.length(), 0);
        response = RPL_UMODEIS(user->getNickname(), user->getMode());
        send(user->getSocket(), response.c_str(), response.length(), 0);
    }
}

void	Command::processMode(User *user, std::vector<Channel*> &channel, std::vector<User*> &users)
{
    bool    isSet = false;

    if (parameters[0].empty() && parameters[1].empty())
    {
        std::string response = ERR_UMODEUNKNOWNFLAG(user->getNickname());
        send(user->getSocket(), response.c_str(), response.length(), 0);
    }
    std::vector<User*>::iterator it = users.begin();
    std::string name = parameters[0];
    for (; it != users.end(); it++)
    {
        if ((*it)->getNickname() == name)
            isSet = true;
    }
    if (parameters[0].find("#") != std::string::npos)
    {
        channelMode(user, channel);
    }
    else if (isSet == true)
    {
        userMode(user, channel);
    }
}