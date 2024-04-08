/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelMode.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 15:31:27 by phudyka           #+#    #+#             */
/*   Updated: 2024/04/08 11:42:58 by phudyka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/channel.hpp"

void	Command::processKick(User *user)
{
    if (parameters.size() < 2)
    {
        user->sendMessage(ERR_NEEDMOREPARAMS(user->getNickname(), "KICK"));
        return ;
    }
    if (!user->isOperator())
    {
        user->sendMessage(ERR_NOPRIVILEGES(user->getNickname()));
        return ;
    }

    Channel	*channel = NULL;
    std::string	channelName = parameters[0];
    std::string	target = parameters[1];
    if (!channel)
    {
        user->sendMessage(ERR_NOSUCHCHANNEL(user->getNickname(), channelName));
        return ;
    }
    if (!channel->isInChannel(target, channel))
    {
        user->sendMessage(ERR_USERNOTINCHANNEL(user->getNickname(), target, channelName));
        return ;
    }
    channel->kickUser(target, "Kicked by " + user->getNickname());
    std::string response = RPL_KICK(user->getNickname(), channelName, target, "Kicked by " + user->getNickname());
    channel->sendToAll(response);
}

void Command::processInvite(User *user, std::vector<Channel*> &channels)
{
    if (parameters.size() < 2)
    {
        user->sendMessage(ERR_NEEDMOREPARAMS(user->getNickname(), "INVITE"));
        return;
    }
   
    std::string channelName = parameters[1];
    Channel *channel = NULL;
    for (size_t i = 0; i < channels.size(); ++i)
    {
        if (channels[i]->getName() == channelName)
        {
            channel = channels[i];
            break;
        }
    }
    if (!channel)
    {
        user->sendMessage(ERR_NOSUCHCHANNEL(user->getNickname(), channelName));
        return;
    }
    
    std::string operatorName = channel->getOperator();
    if (operatorName != user->getNickname())
    {
        user->sendMessage(ERR_NOPRIVILEGES(user->getNickname()));
        return;
    }

    User *userToInvite = NULL;
    std::string userToInviteNick = parameters[0];
    for (size_t i = 0; i < channel->getUsers().size(); ++i)
    {
        if (channel->getUsers()[i]->getNickname() == userToInviteNick)
        {
            userToInvite = channel->getUsers()[i];
            break;
        }
    }
    if (!userToInvite)
    {
        user->sendMessage(ERR_NOSUCHNICK(user->getNickname(), userToInviteNick));
        return;
    }
    
    std::string response = RPL_INVITING(user_id(user->getNickname(), user->getUsername()), user->getNickname(), userToInviteNick, channelName);
    user->sendMessage(response);
    
    if (channel->addUser(userToInvite, user->getMode()))
    {
        response = RPL_INVITE(user_id(user->getNickname(), user->getUsername()), user->getNickname(), userToInviteNick, channelName);
        channel->sendToAll(response);
    }
}

void	Command::processTopic(User *user, std::vector<Channel*> &channels)
{
    if (parameters.empty())
    {
        user->sendMessage(ERR_NEEDMOREPARAMS(user->getNickname(), "TOPIC"));
        return;
    }
    std::string	channelName = parameters[0];
    std::string	newTopic;
    if (parameters.size() > 1)
    {
        newTopic = parameters[1];
    }
    Channel *channel = NULL;
    for (size_t i = 0; i < channels.size(); ++i)
    {
        if (channels[i]->getName() == channelName)
        {
            channel = channels[i];
            break;
        }
    }
    if (!channel)
    {
        user->sendMessage(ERR_NOSUCHCHANNEL(user->getNickname(), channelName));
        return;
    }
    if (!user->isOperator())
    {
        user->sendMessage(ERR_NOPRIVILEGES(user->getNickname()));
        return;
    }
    channel->setTopic(newTopic);
    std::string	response = RPL_TOPIC(user->getNickname(), channelName, newTopic);
    channel->sendToAll(response);
}