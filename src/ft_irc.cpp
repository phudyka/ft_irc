/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 08:59:08 by dtassel           #+#    #+#             */
/*   Updated: 2024/03/12 12:15:33 by phudyka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/ft_irc.hpp"

ft_irc::ft_irc(int port, std::string pass)
{
    this->_port = port;
	this->_pass = pass;
    this->_isRunning = false;
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
}

ft_irc::~ft_irc() {}

void ft_irc::start()
{
	std::cout << YELLOW << "[Welcome to ft_irc]" << RESET << std::endl << std::endl;
	std::cout << PURPLE << "Waiting for connection..." << RESET << std::endl << std::endl;
    Channel *newChannel = new Channel("Default");
    this->_channels.push_back(newChannel);
    _isRunning = true;
    handleConnection();
}

void ft_irc::handleConnection(void)
{
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
        for (size_t i = 1; i < _pollfds.size(); ++i)
        {
            if (_pollfds[i].revents & POLLIN)
                clientData(i);
        }
    }
}

static void getPassword(int UserSocket, std::string correctPassword)
{
    while (true)
    {
        send(UserSocket, YELLOW "Enter Password: " RESET, strlen(YELLOW) + 17 + strlen(RESET), 0);
        char receivedPass[256];
        int passLen = recv(UserSocket, receivedPass, sizeof(receivedPass) - 1, 0);
        receivedPass[passLen] = '\0';

        size_t len = strlen(receivedPass);
        while (len > 0 && (receivedPass[len - 1] == '\r' || receivedPass[len - 1] == '\n'))
            receivedPass[--len] = '\0';

        if (correctPassword == std::string(receivedPass))
        {
            send(UserSocket, YELLOW "[Welcome to ft_irc]\r\n" RESET, strlen(YELLOW) + 22 + strlen(RESET), 0);
            break ;
        }
        else
            send(UserSocket, RED "Error: [Invalid Password]\r\n" RESET, strlen(RED) + 28 + strlen(RESET), 0);
    }
}



void ft_irc::newConnection(void)
{
    struct sockaddr_in 	UserAddr;
    socklen_t			UserLen = sizeof(UserAddr);
    int					UserSocket = accept(_socketServer, (struct sockaddr*)&UserAddr, &UserLen);
	char				*UserIP = inet_ntoa(UserAddr.sin_addr);

    if (UserSocket == -1)
    {
        std::cerr << RED << "Error: [Fail to accept a new connection]" << RESET << std::endl;
        return ;
    }

	getPassword(UserSocket, _pass);
    struct pollfd newPollfd;
    newPollfd.fd = UserSocket;
    newPollfd.events = POLLIN;
    newPollfd.revents = 0;
	
	_pollfds.push_back(newPollfd);
	struct hostent*	hostInfo = gethostbyaddr((const char*)&UserAddr.sin_addr, sizeof(UserAddr.sin_addr), AF_INET);

 	if (hostInfo && hostInfo->h_name)
    {
        User* newUser = new User(UserSocket, "caca", hostInfo->h_name, UserIP);
        newUser->setHost(hostInfo->h_name);
        newUser->setIP(UserIP);

        _clients.push_back(newUser);
        logConnection("Connection from User: ", UserIP);
    }
    else
    {
        std::cerr << RED << "Error: [Fail to get host name]" << RESET << std::endl;
        close(UserSocket);
    }
}

void ft_irc::clientData(size_t index)
{
    char buff[2048];
    int len = recv(_pollfds[index].fd, buff, sizeof(buff), 0);
	Command	commandHandler;

    if (len <= 0)
        removeClient(index);
    else
    {
        buff[len] = '\0';
        std::cout << BLUE << "client: " << YELLOW << buff << RESET << std::endl;
        int UserSocket = _pollfds[index].fd;
        std::string message = buff;

        for (size_t i = 0; i < _clients.size(); i++)
        {
            if (_clients[i]->getSocket() == UserSocket)
                commandHandler.masterCommand(_clients[i], message, _channels);
        }
    }
}

void ft_irc::removeClient(size_t index)
{
    close(_pollfds[index].fd);
    _pollfds.erase(_pollfds.begin() + index);
    index--;
    delete (_clients[index]);
    _clients.erase(_clients.begin() + index);
	std::cout << CYAN << "[User [" << index << "] has been succesfully removed]" << RESET << std::endl;
    std::cout << GREEN << "Connection closed." << RESET << std::endl;
}

