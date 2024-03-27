/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 06:53:43 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/27 11:49:32 by phudyka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_irc.hpp"

ft_irc	*globalInstance = NULL;

static void	signalHandler(int sig)
{
	if (globalInstance)
		globalInstance->shutDown();
	exit (sig);
}

int main(int argc, char **argv)
{
    if (argc == 3)
	{
        try
		{
			int	port = atoi(argv[1]);
			std::string	pass = std::string(argv[2]);

            globalInstance = new ft_irc(port, pass);
            signal(SIGINT, signalHandler);
            globalInstance->start();
        }
		catch (const std::exception& e)
		{
            std::cerr << e.what() << '\n';
            exit(EXIT_FAILURE);
        }
        delete (globalInstance);
        return (EXIT_SUCCESS);
    }
	else
	{
        std::cout << RED << "Error : [Bad arguments] './ircserv <port> <password>'" << RESET << std::endl;
        return (EXIT_FAILURE);
    }
    return (EXIT_FAILURE);
}