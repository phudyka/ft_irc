/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 08:59:08 by dtassel           #+#    #+#             */
/*   Updated: 2024/03/06 11:43:42 by phudyka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_irc.hpp"

ft_irc::ft_irc(int port, std::string pass)
{
    this->_port = port;
	this->_pass = pass;
    this->_rootDirectory = ".";
    this->_isRunning = false;
}

ft_irc::~ft_irc() {}

void ft_irc::start()
{
    _socketServer = socket(AF_INET, SOCK_STREAM, 0);
    if (_socketServer == -1)
        throw std::runtime_error(RED "Error: [Fail to create a socket server]" RESET);
    int reuseAddr = 1;
    if (setsockopt(_socketServer, SOL_SOCKET, SO_REUSEADDR, &reuseAddr, sizeof(reuseAddr)) == -1)
        throw std::runtime_error(RED "Error: [Fail to set SO_REUSEADDR option]" RESET);
    fcntl(_socketServer, F_SETFL, O_NONBLOCK);

    _serverAddr.sin_family = AF_INET;
    _serverAddr.sin_addr.s_addr = INADDR_ANY;
    _serverAddr.sin_port = htons(_port);

    socklen_t sockLen = sizeof(_serverAddr);

    int bindReturn = bind(_socketServer, (struct sockaddr*)&_serverAddr, sockLen);
    if (bindReturn == -1)
        throw std::runtime_error(RED "Error: [Fail to bind the socket server]" RESET);

    int listenReturn = listen(_socketServer, 5);
    if (listenReturn == -1)
        throw std::runtime_error(RED "Error: [Fail to listen to connections]" RESET);

    _pollfds.push_back(pollfd());
    _pollfds.back().fd = _socketServer;
    _pollfds.back().events = POLLIN;
    _pollfds.back().revents = 0;

    _isRunning = true;
}

void ft_irc::firstConnection(void)
{
	std::cout << YELLOW << "[Welcome to ft_irc]" << RESET << std::endl << std::endl;
	std::cout << PURPLE << "Waiting for connection..." << RESET << std::endl << std::endl;
	
	while (_isRunning)
	{
        int	readyCount = poll(&_pollfds[0], static_cast<nfds_t>(_pollfds.size()), -1);

        if (readyCount == -1)
        {
            if (errno == EINTR)
                continue ;
            else
                throw std::runtime_error(RED "Error: [poll() failed]" RESET);
        }
        if (_pollfds[0].revents & POLLIN)
            newConnection();
        // for (size_t i = 1; i < _pollfds.size(); ++i)
        // {
        //     if (_pollfds[i].revents & POLLIN)
        //         clientData(i);
        // }
    }
}

void ft_irc::newConnection(void)
{
    struct sockaddr_in 	clientAddr;
    socklen_t			clientLen = sizeof(clientAddr);
    int					clientSocket = accept(_socketServer, (struct sockaddr*)&clientAddr, &clientLen);

    if (clientSocket == -1)
    {
        std::cerr << RED << "Error: [Fail to accept a new connection]" << RESET << std::endl;
        return ;
    }

    struct pollfd newPollfd;
    newPollfd.fd = clientSocket;
    newPollfd.events = POLLIN;
    newPollfd.revents = 0;
	
	_pollfds.push_back(newPollfd);
	// Client* newClient = new Client(clientSocket, inet_ntoa(clientAddr.sin_addr));
	// _clients.push_back(newClient);
	// logConnection("Connection from client: ", inet_ntoa(clientAddr.sin_addr));
}

// void ft_irc::clientData(size_t index)
// {
//     char buff[2048];
//     int len = recv(_pollfds[index].fd, buff, sizeof(buff), 0);

//     if (len <= 0)
//         removeClient(index);
//     else
//     {
//         buff[len] = '\0';
//         int clientSocket = _pollfds[index].fd;

//         for (size_t i = 0; i < _clients.size(); i++)
//         {
//             if (_clients[i]->getSocket() == clientSocket)
//             {
//                 Request request(clientSocket, _clients[i]->getAdIP(), std::string(buff));
//                 request.handleRequest();
//             }
//         }
//     }
// }

// void ft_irc::removeClient(size_t index)
// {
//     close(_pollfds[index].fd);
//     _pollfds.erase(_pollfds.begin() + index);
//     index--;
//     delete (_clients[index]);
//     _clients.erase(_clients.begin() + index);
// 	std::cout << CYAN << "[Client [" << index << "] has been succesfully removed]" << RESET << std::endl;
//     std::cout << GREEN << "Connection closed." << RESET << std::endl;
// }

