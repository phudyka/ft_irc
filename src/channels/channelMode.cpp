/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelMode.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 15:31:27 by phudyka           #+#    #+#             */
/*   Updated: 2024/04/25 11:12:52 by phudyka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/channel.hpp"

void	Command::processKick(User *user, std::vector<Channel*> &channel, std::vector<User*> &users)
{
    (void)users;
    if (parameters.size() < 2)
    {
        user->sendMessage(ERR_NEEDMOREPARAMS(user->getNickname(), "KICK"));
        return ;
    }
    std::string	channelName = parameters[0].substr(1);
    std::string	target = parameters[1];

    std::vector<Channel*>::iterator it = channel.begin();
    bool    chanExist = false;
    for (; it != channel.end(); it++)
    {
        if ((*it)->getName() == channelName)
        {
            chanExist = true;
            break;
        }
    }
    if (chanExist == false)
    {
        user->sendMessage(ERR_NOSUCHCHANNEL(user->getNickname(), channelName));
        return ;
    }
    std::string operatorName = (*it)->getOperator(user->getNickname());
    if (operatorName != user->getNickname())
    {
        user->sendMessage(ERR_NOPRIVILEGES(user->getNickname()));
        return;
    }

    if ((*it)->isInChannel(target, *it) == false)
    {
        user->sendMessage(ERR_USERNOTINCHANNEL(user->getNickname(), target, channelName));
        return ;
    }
    (*it)->kickUser(user, target, "");
    std::string response = RPL_KICK(user_id(user->getNickname(), user->getUsername()), channelName, target, "");
    (*it)->sendToAll(response);
}

void Command::processInvite(User *user, std::vector<Channel*> &channels, std::vector<User*> &users)
{
    if (parameters.size() < 2)
    {
        user->sendMessage(ERR_NEEDMOREPARAMS(user->getNickname(), "INVITE"));
        return;
    }
    if (parameters[1].find("#") == std::string::npos)
    {
        user->sendMessage(ERR_NOSUCHCHANNEL(user->getNickname(), parameters[1]));
        return ;
    }

    bool    chanExist = false;
    std::string channelName = parameters[1].substr(1);
    std::vector<Channel*>::iterator it = channels.begin();
    for (; it < channels.end(); it++)
    {
        if ((*it)->getName() == channelName)
        {
            chanExist = true;
            break ;
        }
    }
    if (chanExist == false)
    {
        user->sendMessage(ERR_NOSUCHCHANNEL(user->getNickname(), channelName));
        return ;
    }
    
    std::string operatorName = (*it)->getOperator(user->getNickname());
    if (operatorName != user->getNickname())
    {
        user->sendMessage(ERR_NOPRIVILEGES(user->getNickname()));
        return ;
    }

    std::string userToInviteNick = parameters[0];
    std::vector<User*>::iterator itu = users.begin();
    bool    userExist = false;
    for (; itu != users.end(); itu++)
    {
        if ((*itu)->getNickname() == userToInviteNick)
        {
            userExist = true;
            break ;
        }
    }
    if (userExist == false)
    {
        user->sendMessage(ERR_NOSUCHNICK(user->getNickname(), userToInviteNick));
        return ;
    }
    std::string response = RPL_INVITING(user_id(user->getNickname(), user->getUsername()), user->getNickname(), channelName, userToInviteNick);
    user->sendMessage(response);
    
    (*it)->invite(*itu);

    response = RPL_INVITE(user_id(user->getNickname(), user->getUsername()), user->getNickname(), (*itu)->getNickname(), channelName);
    send((*itu)->getSocket(), response.c_str(), response.length(), 0);
}

void	Command::processTopic(User *user, std::vector<Channel*> &channels)
{
    if (parameters.empty())
    {
        user->sendMessage(ERR_NEEDMOREPARAMS(user->getNickname(), "TOPIC"));
        return ;
    }
	bool	chanExist = false;
	std::string channelName = parameters[0].substr(1);
    std::vector<Channel*>::iterator it = channels.begin();
    for (; it < channels.end(); it++)
    {
        if ((*it)->getName() == channelName)
        {
            chanExist = true;
            break ;
        }
    }
    if (chanExist == false)
    {
        user->sendMessage(ERR_NOSUCHCHANNEL(user->getNickname(), channelName));
        return ;
    }
    if (trailing.empty())
    {
        std::string    response = RPL_TOPIC(user->getNickname(), channelName, (*it)->getTopic());
        send(user->getSocket(), response.c_str(), response.length(), 0);
        return ;
    }
    std::string operatorName = (*it)->getOperator(user->getNickname());
    if (operatorName != user->getNickname() && (*it)->getStTopic() == true)
    {
        user->sendMessage(ERR_NOPRIVILEGES(user->getNickname()));
        return ;
    }
    (*it)->setTopic(trailing);
    std::string	response = RPL_TOPIC(user->getNickname(), channelName, trailing);
    (*it)->sendToAll(response);
}