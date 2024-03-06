/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 11:32:49 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/06 11:33:03 by phudyka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_irc.hpp"

void	ft_irc::shutDown(void)
{
	close(_socketServer);
	std::cout << CYAN << "[Server socket has been succesfully closed]" << RESET << std::endl;
	std::cout << YELLOW << "[Webserv has been shutdown succesfully]" << RESET << std::endl;
}
