/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtassel <dtassel@42.nice.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 11:37:59 by phudyka           #+#    #+#             */
/*   Updated: 2024/04/03 15:51:22 by dtassel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_irc.hpp"

std::vector<Channel *>::iterator     Command::searchChannelName(std::string channelName, std::vector<Channel*> &channel)
{
    std::vector<Channel*>::iterator it = channel.begin();

    for (; it != channel.end(); it++)
    {
        if ((*it)->getName() == channelName)
        {
            return it;
        }
    }
    return channel.end();
}

void	Command::channelMode(User *user, std::vector<Channel*> &channel)
{
    std::string channelName = parameters[0];
    channelName = extractParameter(channelName, "#");
    if (parameters[1].empty() == false)
    {
        if (parameters[1].find("+i") || parameters[1].find("+t") || parameters[1].find("+k")
            || parameters[1].find("+o") || parameters[1].find("+l"))
        {
            std::vector<Channel *>::iterator it = searchChannelName(channelName, channel);
            if (it != channel.end())
            {
                (*it)->setMode(parameters[1].substr(0, parameters[1].length() -2));
                std::string response = RPL_CHANNELMODEIS(user->getNickname(), channelName, (*it)->getMode());
                send(user->getSocket(), response.c_str(), response.length(), 0);
                return;
            }
        }
    }
	// std::vector<Channel*>::iterator it = channel.begin();
    // //std::string channelName = parameters[0];
    // channelName = extractParameter(channelName, "#");
    // for (; it != channel.end(); it++)
    // {
    //     if ((*it)->getName() == channelName)
    //     {
    //         std::string response = RPL_CHANNELMODEIS(user->getNickname(), channelName, (*it)->getMode());
    //         send(user->getSocket(), response.c_str(), response.length(), 0);
    //         return ;
    //     }
    // }
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