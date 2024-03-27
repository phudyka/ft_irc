/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/27 11:37:59 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/27 11:41:36 by phudyka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_irc.hpp"

void	Command::channelMode(int socket, std::vector<Channel*> &channel, std::string channelName, std::string client)
{
	std::vector<Channel*>::iterator it = channel.begin();

    for (; it != channel.end(); ++it)
    {
        if ((*it)->getName() == channelName)
        {
			std::string mode = "";
            std::string response = RPL_CHANNELMODEIS(client, channelName, mode);
            send(socket, response.c_str(), response.length(), 0);
            return ;
        }
    }
    std::string response = "Channel #" + channelName + " not found.\r\n";
    send(socket, response.c_str(), response.length(), 0);
}

void	Command::userMode(int socket, bool isSetMode, std::string mode, std::string client, UserMode& uMode)
{
	if (mode == "i")
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
    std::string symbol = parameters[1].substr(0, 1);
    std::string mode = parameters[1].substr(1, parameters[1].length() - 3);

    if (symbol != "+" && symbol != "-")
    {
        std::string	response = ERR_UMODEUNKNOWNFLAG(client);
        send(socket, response.c_str(), response.length(), 0);
        return ;
    }
    bool	isSetMode = (symbol == "+");
	if (symbol == "#")
		channelMode(socket, channel, symbol, client);
	else
		userMode(socket, isSetMode, mode, client, uMode);
}