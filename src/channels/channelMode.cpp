/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   channelMode.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtassel <dtassel@42.nice.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 15:31:27 by phudyka           #+#    #+#             */
/*   Updated: 2024/04/10 10:00:54 by dtassel          ###   ########.fr       */
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
    std::string operatorName = (*it)->getOperator();
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
    std::cout << "DANS INVITE" << std::endl;
    if (parameters.size() < 2)
    {
        user->sendMessage(ERR_NEEDMOREPARAMS(user->getNickname(), "INVITE"));
        return;
    }
    bool    chanExist = false;
    std::string channelName = parameters[1].substr(0, parameters[1].length()-2);
    std::vector<Channel*>::iterator it = channels.begin();
    for (; it < channels.end(); it++)
    {
        if ((*it)->getName() == channelName)
        {
            std::cout << "chann exist" << std::endl;
            chanExist = true;
            break;
        }
    }
    if (chanExist == false)
    {
        user->sendMessage(ERR_NOSUCHCHANNEL(user->getNickname(), channelName));
        return;
    }
    
    std::string operatorName = (*it)->getOperator();
    if (operatorName != user->getNickname())
    {
        user->sendMessage(ERR_NOPRIVILEGES(user->getNickname()));
        return;
    }

    std::string userToInviteNick = parameters[0];
    std::cout << "userTo = " << userToInviteNick << "salut" << std::endl;
    std::vector<User*>::iterator itu = users.begin();
    bool    userExist = false;
    for (; itu != users.end(); itu++)
    {
        if ((*itu)->getNickname() == userToInviteNick)
        {
            std::cout << "user exist" << std::endl;
            userExist = true;
            break;
        }
    }
    if (userExist == false)
    {
        user->sendMessage(ERR_NOSUCHNICK(user->getNickname(), userToInviteNick));
        return;
    }
    std::string response = RPL_INVITING(user_id(user->getNickname(), user->getUsername()), user->getNickname(), channelName, userToInviteNick);
    user->sendMessage(response);
    
    (*it)->invite(*itu);

    response = RPL_INVITE(user_id(user->getNickname(), user->getUsername()), user->getNickname(), (*itu)->getNickname(), channelName);
    send((*itu)->getSocket(), response.c_str(), response.length(), 0);
    std::cout << "FIN" << std::endl;
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