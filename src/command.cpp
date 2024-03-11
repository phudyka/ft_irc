/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 15:37:59 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/11 09:23:59 by phudyka          ###   ########.fr       */
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

void	Command::masterCommand(int userSocket, const std::string& command)
{
    if (command.find("USER ") == 0)
        processUser(userSocket);
    else if (command.find("CAP REQ :multi-prefix") == 0)
        processCapReq(userSocket);
    else if (command.find("CAP END") == 0)
        processCapEnd(userSocket);
    else if (command.find("PING ") == 0)
        processPing(userSocket, command);
	// else if (command.find("USERHOST ") == 0)
    //     processHost(userSocket);
    // else
    //     std::cout << ORANGE << "Command unknown: " << RESET << command << std::endl;
}

void	Command::processUser(int userSocket)
{
    send(userSocket, "CAP * LS :multi-prefix sasl\r\n", strlen("CAP * LS :multi-prefix sasl\r\n"), 0);
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