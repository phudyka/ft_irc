/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelMode.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 15:31:27 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/27 16:21:10 by phudyka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/channel.hpp"

void Command::processKick(User *user)
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
// }

// void	Command::processTopic(User *user, std::vector<Channel*> &channels)
// {
// }

// void	Command::processChannelMode(User *user, std::vector<Channel*> &channels)
// {
// }