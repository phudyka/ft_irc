/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 15:37:59 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/14 11:32:33 by phudyka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/command.hpp"

Command::Command() {}

Command::Command(const Command&) {}

Command::~Command() {}

Command& Command::operator=(const Command& other)
{
    if (this != &other) {}
    return (*this);
}

void Command::masterCommand(User *user, const std::string& command, std::vector<Channel*> &channel)
{
	int	userSocket = user->getSocket();

    if (command.find("CAP LS") == 0)
        processCap(userSocket);
	// else if (command.find("PASS") == 0)
	// 	processPass(userSocket, command);		
    else if (command.find("NICK") == 0)
        processNick(user, command);
	else if (command.find("USER ") == 0)
		processUser(user, command);
    else if (command.find("CAP END") == 0)
        processCapEnd(user);
	// else if (command.find("MODE ") == 0)
	// 	processMode(user, command);
	// else if (command.find("WHOIS ") == 0)
	// 	processWhoIs(user, command);
    else if (command.find("PING ") == 0)
        processPing(userSocket, command);
    else if (command.find("JOIN") == 0)
        joinChannel(user, command, channel);
    else
        std::cout << ORANGE << "Command unknown: " << RESET << command << std::endl;
}

void	Command::processCap(int userSocket)
{
    send(userSocket, "CAP * LS :none\r\n", strlen("CAP * LS :none\r\n"), 0);
}

void	Command::processPass(int userSocket, std::string correctPassword)
{
	char receivedPass[256];
	int passLen = recv(userSocket, receivedPass, sizeof(receivedPass) - 1, 0);
	receivedPass[passLen] = '\0';
    std::string cmd = receivedPass;
    std::cout << cmd << std::endl;

    size_t passIndex = cmd.find("PASS");
    if (passIndex != std::string::npos) {
        std::string password = cmd.substr(passIndex + 5);
        password.erase(std::remove(password.begin(), password.end(), '\r'), password.end());
        password.erase(std::remove(password.begin(), password.end(), '\n'), password.end());
        std::cout << password << std::endl;

        if (correctPassword == password)
            send(userSocket, YELLOW "[Welcome to ft_irc]\r\n RESET", strlen(YELLOW) + 22 + strlen(RESET), 0);
		else
		{
            send(userSocket, RED "Error: [Invalid Password]\r\n" RESET, strlen(RED) + 28 + strlen(RESET), 0);
            close(userSocket);
        }
    }
}

void	Command::processNick(User *user, const std::string &command)
{
    std::string nickname = extractParameter(command, "NICK");
    user->setNickname(nickname);
}

void	Command::processUser(User *user, const std::string &command)
{
	size_t usernameStart = command.find("USER");
	if (usernameStart != std::string::npos)
	{
		size_t usernameEnd = command.find(' ', usernameStart + 5);
		if (usernameEnd != std::string::npos)
		{
            std::string username = command.substr(usernameStart + 5, usernameEnd - usernameStart - 5);
            user->setUserName(username);
        }
    }
}

void	Command::processCapEnd(User *user)
{
    std::string welcome = "001 " + user->getUser() + ":Welcome to the Internet Relay Network\r\n";
    send(user->getSocket(), welcome.c_str(), welcome.size(), 0);
}

// void	Command::processMode(User *user, const std::string &command)
// {
	
// }

// void	Command::processWhoIs(User *user, const std::string &command)
// {
	
// }

void	Command::processPing(int userSocket, const std::string& pingCommand)
{
    std::string pingParam = pingCommand.substr(5);
    std::string pong = "PONG " + pingParam + "\r\n";
    send(userSocket, pong.c_str(), pong.size(), 0);
}

void	Command::joinChannel(User *user, const std::string &command, std::vector<Channel*> &channels)
{
    std::string name = extractParameter(command, "#");
    std::vector<Channel*>::iterator it = channels.begin();
    if (name.empty())
        name = "Default";
    for (; it != channels.end(); it++)
    {
        if ((*it)->getName() == name)
        {
            (*it)->addUser(user);
            break ;
        }
    }
    std::cout << user->getNickname() << " Join the channel : " << name << std::endl;
}


std::string Command::extractParameter(const std::string& command, const std::string& prefix)
{
    size_t begin = command.find(prefix) + prefix.length();
    if (begin == 0)
        return ("");
    return (command.substr(begin));
}
