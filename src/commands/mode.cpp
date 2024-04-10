/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 11:37:59 by phudyka           #+#    #+#             */
/*   Updated: 2024/04/10 14:29:31 by phudyka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_irc.hpp"

std::vector<Channel*>::iterator     Command::searchChannelName(std::string channelName, std::vector<Channel*> &channel)
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

void	Command::processChannelMode(User *user, std::vector<Channel*> &channel)
{
    std::string	channelName = parameters[0].substr(1);
    if (parameters.size() >= 2)
    {
        if (parameters[1].find("+i") != std::string::npos || parameters[1].find("+t") != std::string::npos || 
            parameters[1].find("+k") != std::string::npos || parameters[1].find("+o") != std::string::npos || 
            parameters[1].find("+l") != std::string::npos)
        {
            std::vector<Channel*>::iterator it = searchChannelName(channelName, channel);
            if (it != channel.end() && (*it)->getOperator() == user->getNickname())
            {
                char	modeFlag = parameters[1][1];
                switch (modeFlag)
                {
                    case 'i':
                        (*it)->setInviteOnly(true);
                        break;
                    // case 't':
                    //     (*it)->setTopic(true);
                        // break;
                    case 'k':
                        if (parameters.size() >= 3)
                            (*it)->setPassword(parameters[2]);
                        break;
                    case 'o':
                        if (parameters.size() >= 3)
                            (*it)->addOperator(parameters[2]);
                        break;
                    case 'l':
                        if (parameters.size() >= 3)
                            (*it)->setUserLimit(atoi(parameters[2].c_str()));
                        break;
                    default:
                        break;
                }
                (*it)->setMode(parameters[1]);
                std::string	response = RPL_CHANNELMODEIS(user->getNickname(), channelName, (*it)->getMode());
                send(user->getSocket(), response.c_str(), response.length(), 0);
                return;
            }
        }
    }
    else
    {
        channelName = channelName.substr(0, channelName.length() -2);
        std::vector<Channel*>::iterator it = searchChannelName(channelName, channel);
        if (it != channel.end())
        {
            std::string	response = RPL_CHANNELMODEIS(user->getNickname(), channelName, (*it)->getMode());
            send(user->getSocket(), response.c_str(), response.length(), 0);
            return;
        }
    }
}

void	Command::processUserMode(User *user, std::vector<Channel *> &channels)
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

    if (parameters[0].empty())
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
        processChannelMode(user, channel);
    else if (isSet == true)
        processUserMode(user, channel);
}