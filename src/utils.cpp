/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 11:32:49 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/14 11:20:26 by phudyka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_irc.hpp"

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