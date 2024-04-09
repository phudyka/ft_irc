/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_irc.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtassel <dtassel@42.nice.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 08:59:08 by dtassel           #+#    #+#             */
/*   Updated: 2024/04/09 14:07:25 by dtassel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/ft_irc.hpp"

std::vector<std::string> split(std::string s, std::string delimiter)
{
    size_t pos_start = 0, pos_end;
    std::vector<std::string> res;

    while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos)
    {
        std::string token = s.substr(pos_start, pos_end - pos_start);
        res.push_back(token + delimiter);
        pos_start = pos_end + delimiter.length();
    }
    if (pos_start < s.length())
    {
        std::string lastToken = s.substr(pos_start);
        res.push_back(lastToken);
    }
    return (res);
}

ft_irc::ft_irc(int port, std::string pass)
{
    this->hostname = "ft_irc";
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
	//displayClients();
    handleConnection();
}

void ft_irc::handleConnection(void)
{
    while (_isRunning)
    {
        int readyCount = poll(&_pollfds[0], static_cast<nfds_t>(_pollfds.size()), -1);

        if (readyCount == -1)
        {
            if (errno == EINTR)
                continue;
            else
                throw std::runtime_error(RED "Error: [poll() failed]" RESET);
        }

        if (_pollfds[0].revents & POLLIN)
            newConnection();

        for (size_t i = 1; i < _pollfds.size(); ++i)
        {
            if (_pollfds[i].revents & POLLIN)
            {
                clientData(i);
                //displayClients();
            }
            else if (_pollfds[i].revents & (POLLHUP | POLLERR))
            {
                removeClient(i);
            }
        }
    }
}


bool    ft_irc::connectClient(int socket, User *user)
{
    size_t len;
    char client[256];
    len = recv(socket, client, sizeof(client), 0);
    client[len] = '\0';
    std::vector<std::string> infoClient = split(client, "\r\n");
    std::vector<std::string>::iterator it = infoClient.begin();
    for (; it != infoClient.end(); it++)
    {
            Command commandHandler;
            commandHandler.masterCommand(user, *it, _channels, _pass, _clients);
            if (it->find("PASS") != std::string::npos && user->getAuthPass() == false)
                return false;
    }
    return true;
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

    struct pollfd newPollfd;
    newPollfd.fd = UserSocket;
    newPollfd.events = POLLIN;
    newPollfd.revents = 0;
	
	_pollfds.push_back(newPollfd);
	struct hostent*	UserInfo = gethostbyaddr((const char*)&UserAddr.sin_addr, sizeof(UserAddr.sin_addr), AF_INET);

 	if (UserInfo && UserInfo->h_name)
    {
        User* newUser = new User(UserSocket, "", UserInfo->h_name, UserIP);
        newUser->setHostname(UserInfo->h_name);
        newUser->setIP(UserIP);
        _clients.push_back(newUser);
        if (connectClient(UserSocket, newUser) == false)
        {
            close(UserSocket);
            _pollfds.pop_back();
            delete newUser;
            _clients.pop_back();
        }
    }
    else
    {
        std::cerr << RED << "Error: [Fail to get username]" << RESET << std::endl;
        close(UserSocket);
    }
	//displayClients();
}

void ft_irc::clientData(size_t index)
{
    char buff[4096];
    int len = recv(_pollfds[index].fd, buff, sizeof(buff), 0);
	Command	commandHandler;

    if (len <= 0)
        removeClient(index);
    else
    {
        buff[len] = '\0';
        int UserSocket = _pollfds[index].fd;
        std::string message = buff;

        for (size_t i = 0; i < _clients.size(); i++)
        {
            if (_clients[i]->getSocket() == UserSocket)
                commandHandler.masterCommand(_clients[i], message, _channels, _pass, _clients);
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
	displayClients();
}