/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelMode.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 15:31:27 by phudyka           #+#    #+#             */
/*   Updated: 2024/04/08 09:28:47 by phudyka          ###   ########.fr       */
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

// void	Command::processInvite(User *user, std::vector<Channel*> &channels)
// {
//     if (parameters.size() < 2)
//     {
//         user->sendMessage(ERR_NEEDMOREPARAMS(user->getNickname(), "INVITE"));
//         return ;
//     }
//     std::string	channelName = parameters[0];
//     User		*userToInvite = parameters[1];
//     if (!user->isOperator())
//     {
//         user->sendMessage(ERR_NOPRIVILEGES(user->getNickname()));
//         return ;
//     }
//     Channel	*channel = NULL;
//     for (size_t i = 0; i < channels.size(); ++i)
//     {
//         if (channels[i]->getName() == channelName)
//         {
//             channel = channels[i];
//             break ;
//         }
//     }
//     if (!channel)
//     {
//         user->sendMessage(ERR_NOSUCHCHANNEL(user->getNickname(), channelName));
//         return;
//     }
//     channel->addUser(userToInvite, user->getMode());
//     std::string	response = RPL_INVITING(user->getNickname(), userToInvite, channelName);
//     user->sendMessage(response);
// 	response = RPL_INVITE(user->getNickname(), userToInvite, channelName);
// 	user->sendMessage(response);
// }

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