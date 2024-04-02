/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtassel <dtassel@42.nice.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 11:32:49 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/29 10:34:02 by dtassel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_irc.hpp"

void	ft_irc::shutDown(void)
{
	close(_socketServer);
	std::cout << CYAN << "[Server socket has been succesfully closed]" << RESET << std::endl;
	std::cout << YELLOW << "[ft_irc has been shutdown succesfully]" << RESET << std::endl;
}

void	ft_irc::logConnection(const std::string& msg, const std::string id)
{
    char		timeStr[20];
	time_t		currentTime = time(NULL);
    struct tm*	localTime = localtime(&currentTime);

    strftime(timeStr, sizeof(timeStr),"[%Y-%m-%d %H:%M] ", localTime);
    std::cout << BLUE << timeStr << RESET << msg << GREEN << id << RESET << std::endl;
}

void ft_irc::displayClients(void)
{
    std::cout << "\033[2J" << "\r";

    std::stringstream table;

    table << "\r" << YELLOW << std::setw(5) << "FD" << std::setw(15) << "Nickname" << std::setw(10) << "IP" << std::setw(15) << "Channel" << std::endl;
    for (size_t i = 0; i < _clients.size(); ++i)
    {
        table << CYAN << std::setw(4) << _clients[i]->getSocket() << RESET
              << std::setw(13) << GREEN << _clients[i]->getNickname() << RESET
              << std::setw(11) << BLUE << _clients[i]->getIP() << RESET
              << std::setw(18) << ORANGE << _clients[i]->getJoinedChannels() << RESET << std::endl;
    }
    std::cout << table.str();
}