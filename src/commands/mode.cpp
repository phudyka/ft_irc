/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtassel <dtassel@42.nice.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 11:37:59 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/28 08:39:37 by dtassel          ###   ########.fr       */
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
        if ((*it)->getName() == channelName)
        {
			std::string mode = "m";
            std::string response = RPL_CHANNELMODEIS(client, channelName, mode);
            send(socket, response.c_str(), response.length(), 0);
            return ;
        }
    }
}

void	Command::userMode(int socket, bool isSetMode, std::string symbol, std::string mode, std::string client, UserMode& uMode)
{
    if (symbol != "+" && symbol != "-")
    {
        std::string	response = ERR_UMODEUNKNOWNFLAG(client);
        send(socket, response.c_str(), response.length(), 0);
        return ;
    }
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
    std::cout << symbol << std::endl;
    std::cout << mode << std::endl;
	if (symbol.find("#") != std::string::npos)
    {
		channelMode(socket, channel, client);
    }
	else
		userMode(socket, isSetMode, symbol, mode, client, uMode);
}