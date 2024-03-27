/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtassel <dtassel@42.nice.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 11:37:59 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/27 16:57:29 by dtassel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_irc.hpp"

void	Command::channelMode(int socket, std::vector<Channel*> &channel, std::string client)
{
	std::vector<Channel*>::iterator it = channel.begin();
    std::string channelName = parameters[0];
    channelName = extractParameter(channelName, "#");
    for (; it != channel.end(); it++)
    {
        std::cout << "for" << std::endl;
        if ((*it)->getName() == channelName)
        {
            std::cout << "if" << std::endl;
			std::string mode = "m";
            std::string response = RPL_CHANNELMODEIS(client, channelName, mode);
            send(socket, response.c_str(), response.length(), 0);
            return ;
        }
    }
}

void	Command::userMode(int socket, bool isSetMode, std::string mode, std::string client, UserMode& uMode)
{
	if (mode.find("i") != std::string::npos)
        uMode.set(UserMode::INVISIBLE, isSetMode);
    else if (mode == "m")
        uMode.set(UserMode::MARK, isSetMode);
    else
    {
        std::string response = ERR_UMODEUNKNOWNFLAG(client);
        send(socket, response.c_str(), response.length(), 0);
        return ;
    }
    std::string	response = MODE_USERMSG(client, mode);
    send(socket, response.c_str(), response.length(), 0);
    response = RPL_UMODEIS(client, mode);
    send(socket, response.c_str(), response.length(), 0);
}

void	Command::processMode(User *user, std::vector<Channel*> &channel)
{
    UserMode& uMode = user->umode();

	int			socket = user->getSocket();
    std::string client = user->getNickname();
    std::string symbol;
    std::string mode;
    if (parameters.size() == 1)
        symbol = parameters[0].substr(0, 1);
    else
    {
        symbol = parameters[1].substr(0, 1);
        mode = parameters[1].substr(1);
    }
    bool	isSetMode = (symbol == "+");
    /*if (symbol != "+" && symbol != "-")
    {
        std::string	response = ERR_UMODEUNKNOWNFLAG(client);
        send(socket, response.c_str(), response.length(), 0);
        return ;
    }*/
    std::cout << symbol << std::endl;
    std::cout << mode << std::endl;
	if (symbol.find("#") != std::string::npos)
    {
		channelMode(socket, channel, client);
    }
	else
		userMode(socket, isSetMode, mode, client, uMode);
}