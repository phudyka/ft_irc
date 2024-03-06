/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 10:30:50 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/06 11:43:08 by phudyka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_IRC_HPP
# define FT_IRC_HPP

// GLOBAL BINDS
# define FD_MAX		1024
# define PORT		6667
# define MAX_EVENT	10

#include <string>
#include <poll.h>
#include <vector>
#include <fcntl.h>
#include <netdb.h>
#include <cstring>
#include <fstream>
#include <csignal>
#include <sstream>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define RED		"\x1b[31m"
#define GREEN   "\x1b[32m"
#define YELLOW	"\x1b[33m"
#define BLUE	"\x1b[34m"
#define PURPLE	"\x1b[35m"
#define CYAN	"\x1b[36m"
#define ORANGE  "\x1b[38;5;208m"
#define RESET   "\x1b[0m"

class	Client;

struct	ServerConfig
{
    int							port;
    size_t						maxBodySize;
    std::string					host;
    std::vector<std::string>	serverNames;
    std::string					defaultErrorPage;

};

class	ft_irc	
{

public:
    ft_irc(int port, std::string pass);
    ~ft_irc();
    void    start();
	void	shutDown(void);
    void    firstConnection(void);

private:
    int         _socketServer;
    int         _port;
	std::string	_pass;
    std::string	_rootDirectory;
    bool		_isRunning;
    struct sockaddr_in _serverAddr;
	struct	ft_ircPollfd
	{
		int		fd;
		short	events;
		short	revents;
	};

	std::vector<struct pollfd>	_pollfds;
	std::vector<Client*> _clients;

	void 	newConnection(void);
    void	closeClientData(int clientSocket);
    void	removeClient(size_t index);
	void	clientData(size_t index);
	void	logConnection(const std::string& msg, const std::string id);
	void	broadcastShutdown(void);
};

#endif