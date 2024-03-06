/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 06:53:43 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/06 10:35:56 by phudyka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_irc.hpp"

ft_irc	*globalInstance = NULL;

// static void	signalHandler(int sig)
// {
// 	if (globalInstance)
// 		globalInstance->shutDown();
// 	exit (sig);
// }

int main(/*int argc, char **argv*/)
{
    // if (argc == 2)
	// {
        ServerConfig	serverConfig;
	
        // if (!parseConfig(argv[1], serverConfig))
        //     return (EXIT_FAILURE);
        try
		{
            globalInstance = new ft_irc(serverConfig.port);
            // signal(SIGINT, signalHandler);

            globalInstance->start();
            globalInstance->firstConnection();
        }
		catch (const std::exception& e)
		{
            std::cerr << e.what() << '\n';
            exit(EXIT_FAILURE);
        }
        delete (globalInstance);
        return (EXIT_SUCCESS);
    // }
	// else
	// {
    //     std::cout << RED << "Error : [Bad arguments] './ft_irc + configFile'" << RESET << std::endl;
    //     return (EXIT_FAILURE);
    // }
    // return (EXIT_FAILURE);
}