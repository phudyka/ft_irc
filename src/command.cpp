/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtassel <dtassel@42.nice.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 15:37:59 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/13 17:06:40 by dtassel          ###   ########.fr       */
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
    if (command.find("CAP LS") == 0)
        processUser(user->getSocket());
    else if (command.find("NICK") == 0)
        processNick(user, command);
	// else if (command.find("USERHOST ") == 0)
    //     processHost(userSocket);
    else if (command.find("CAP END") == 0)
        processCapEnd(user->getSocket());
    else if (command.find("PING ") == 0)
        processPing(user->getSocket(), command);
    else if (command.find("JOIN") == 0)
        joinChannel(user, command, channel);
    // else
    //     std::cout << ORANGE << "Command unknown: " << RESET << command << std::endl;
}

void	Command::processUser(int userSocket)
{
    send(userSocket, "CAP * LS :none\r\n", strlen("CAP * LS :none\r\n"), 0);
}

void	Command::processCapReq(int userSocket)
{
    send(userSocket, "CAP * ACK multi-prefix\r\n", strlen("CAP * ACK multi-prefix\r\n"), 0);
}

void	Command::processCapEnd(int userSocket)
{
    std::string welcome = "001 USER :Welcome to the Internet Relay Network\r\n";
    send(userSocket, welcome.c_str(), welcome.size(), 0);
}

// void	Command::processHost(int userSocket)
// {
// 	User*	targetUser = getSocket(userSocket); //

//     if (targetUser)
//     {
//         std::string response = "USERHOST " + targetUser->getNickname() + " :" + targetUser->getHost() + " " + targetUser->getIP() + "\r\n";
//         send(userSocket, response.c_str(), response.size(), 0);
//     }
// }

void	Command::processPing(int userSocket, const std::string& pingCommand)
{
    std::string pingParam = pingCommand.substr(5);
    std::string pong = "PONG " + pingParam + "\r\n";
    send(userSocket, pong.c_str(), pong.size(), 0);
}

void Command::joinChannel(User *user, const std::string &command, std::vector<Channel*> &channels)
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
            break;
        }
    }
    std::cout << user->getNickname() << " Join the channel : " << name << std::endl;
}

void Command::processNick(User *user, const std::string &command)
{
    std::string nickname = extractParameter(command, "NICK");
    user->setNickname(nickname);
}

std::string Command::extractParameter(const std::string& command, const std::string& prefix)
{
    size_t begin = command.find(prefix) + prefix.length();
    if (begin == 0)
        return "";
    return command.substr(begin);
}