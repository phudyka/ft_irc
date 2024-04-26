/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 11:37:59 by phudyka           #+#    #+#             */
/*   Updated: 2024/04/26 12:12:35 by phudyka          ###   ########.fr       */
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

void	Command::addMode(User *user, std::vector<Channel*> &channel, std::string channelName, std::vector<User*> &users)
{
    char	modeFlag = parameters[1][1];
	if (parameters[1].find("+i") != std::string::npos || parameters[1].find("+t") != std::string::npos || 
            parameters[1].find("+k") != std::string::npos || parameters[1].find("+o") != std::string::npos || 
            parameters[1].find("+l") != std::string::npos)
        {
            std::vector<Channel*>::iterator it = searchChannelName(channelName, channel);
            if (it != channel.end() && (*it)->getOperator(user->getNickname()) == user->getNickname())
            {
                switch (modeFlag)
                {
                    case 'i':
                        (*it)->setInviteOnly(true);
                        break ;
                    case 't':
                        (*it)->enableTopic(true);
                        break;
                    case 'k':
                        if (parameters.size() == 3)
                        {
                            (*it)->setPassword(parameters[2]);
                            (*it)->enablePass(true);
                        }
                        else
                            return;
                        break ;
                    case 'o':
                        if (parameters.size() != 3 || (*it)->addOperator(parameters[2]) == false)
                        {
                            if ((*it)->addOperator(parameters[2]) == false)
                            {
                                std::string response = ERR_NOSUCHNICK(user->getNickname(), parameters[2]);
                                send(user->getSocket(), response.c_str(), response.length(), 0);
                            }
                            return;
                        }
                        else
                        {
                            std::string response = RPL_YOUREOPER(parameters[2]);
                            response += RPL_NAMREPLY(user->getNickname(), user->getMode(), channelName, (*it)->getListInstring());
                            response += RPL_ENDOFNAMES(user->getNickname(), channelName);
                            std::vector<User*>::iterator itu = users.begin();
                            for (; itu != users.end(); itu++)
                            {
                                if ((*itu)->getNickname() == parameters[2])
                                    break;
                            }
                            (*it)->sendToAll(response);
                            return;
                        }
                        break ;
                    case 'l':
                        if (parameters.size() == 3)
                            (*it)->setUserLimit(atoi(parameters[2].c_str()));
                        else
                            return;
                        break ;
                    default:
                        break ;
                }
                (*it)->setMode(parameters[1]);
                std::string	response = RPL_CHANNELMODEIS(user_id(user->getNickname(), user->getUsername()), user->getNickname(), channelName, (*it)->getMode());
                send(user->getSocket(), response.c_str(), response.length(), 0);
                return ;
            }
        }
	else
		user->sendMessage(ERR_INVALIDMODEPARAM(user->getNickname(), channelName, std::string(1, modeFlag), ""));
}

void	Command::removeMode(User *user, std::vector<Channel*> &channel, std::string channelName, std::vector<User*> &users)
{
    char	modeFlag = parameters[1][1];
    if (parameters[1].find("-i") != std::string::npos || parameters[1].find("-t") != std::string::npos || 
            parameters[1].find("-k") != std::string::npos || parameters[1].find("-o") != std::string::npos || 
            parameters[1].find("-l") != std::string::npos)
        {
            std::vector<Channel*>::iterator it = searchChannelName(channelName, channel);
            if (it != channel.end() && (*it)->getOperator(user->getNickname()) == user->getNickname())
            {
                switch (modeFlag)
                {
                    case 'i':
                        (*it)->setInviteOnly(false);
                        break ;
                    case 't':
                        (*it)->enableTopic(false);
                        break ;
                    case 'k':
                        (*it)->setPassword("");
                        (*it)->enablePass(false);
                        break ;
					case 'o':
                    	if (parameters.size() != 3 || (*it)->removeOperator(parameters[2]) == false)
                        	return;
                    	else
                    	{
                        	std::string response = RPL_REMOVED_OPERATOR(parameters[2]);
							response += RPL_NAMREPLY(user->getNickname(), user->getMode(), channelName, (*it)->getListInstring());
                            response += RPL_ENDOFNAMES(user->getNickname(), channelName);
                        	std::vector<User*>::iterator itu;
                        	for (itu = users.begin(); itu != users.end(); ++itu)
                        	{
								if ((*itu)->getNickname() == parameters[2])
									break;
							}
							(*it)->sendToAll(response);
							return;
						}
                    	break;
                    case 'l':
                        (*it)->setUserLimit(-1);
                        break ;
                    default:
                        break ;
                }
                (*it)->unsetMode(parameters[1]);
                std::string	response = RPL_CHANNELMODEIS(user_id(user->getNickname(), user->getUsername()), user->getNickname(), channelName, (*it)->getMode());
                send(user->getSocket(), response.c_str(), response.length(), 0);
                return;
            }
        }
		else
			user->sendMessage(ERR_INVALIDMODEPARAM(user->getNickname(), channelName, std::string(1, modeFlag), ""));
}

void	Command::processChannelMode(User *user, std::vector<Channel*> &channels, std::vector<User*> &users)
{
	bool	chanExist = false;
    std::string	channelName;
    channelName = parameters[0].substr(1);
        
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
    if (operatorName != user->getNickname() && parameters.size() > 1)
    {
        user->sendMessage(ERR_CHANOPRIVSNEEDED(user->getNickname(), channelName));
        return;
    }
    if (parameters.size() >= 2)
    {
        if (parameters[1].find("+") != std::string::npos)
			addMode(user, channels, channelName, users);
		else if (parameters[1].find("-") != std::string::npos)
			removeMode(user, channels, channelName, users);
		else
			user->sendMessage(ERR_INVALIDMODEPARAM(user->getNickname(), channelName, std::string(1, parameters[1][1]), ""));
    }
    else
    {
        std::vector<Channel*>::iterator it = searchChannelName(channelName, channels);
        if (it != channels.end())
        {
            std::string	response = RPL_CHANNELMODEIS(user_id(user->getNickname(), user->getUsername()), user->getNickname(), channelName, (*it)->getMode());
            send(user->getSocket(), response.c_str(), response.length(), 0);
            return ;
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
        user->setMode(parameters[1].substr(0));
        std::string	response = MODE_USERMSG(user->getNickname(), user->getMode());
        send(user->getSocket(), response.c_str(), response.length(), 0);
        response = RPL_UMODEIS(user->getNickname(), user->getMode());
        send(user->getSocket(), response.c_str(), response.length(), 0);
    }
}

void	Command::processMode(User *user, std::vector<Channel*> &channel, std::vector<User*> &users)
{
    bool    isSet = false;

    if (parameters.size() < 1)
    {
        std::string response = ERR_UMODEUNKNOWNFLAG(user->getNickname());
        send(user->getSocket(), response.c_str(), response.length(), 0);
		return ;
    }
    std::vector<User*>::iterator it = users.begin();
    std::string name = parameters[0];
    for (; it != users.end(); it++)
    {
        if ((*it)->getNickname() == name)
            isSet = true;
    }
    if (parameters[0].find("#") != std::string::npos)
        processChannelMode(user, channel, users);
    else if (isSet == true)
        processUserMode(user, channel);
}