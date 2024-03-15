/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 15:37:59 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/15 14:33:40 by phudyka          ###   ########.fr       */
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

void Command::parseIRCMessage(const std::string& command)
{
    commandName = "";
    parameters.clear();
    trailing = "";
    std::string message = command;
    size_t commandEnd = message.find(' ');
    commandName = message.substr(0, commandEnd);

    size_t paramsStart = message.find(' ', commandEnd + 1);
    std::stringstream iss(message.substr(commandEnd + 1, paramsStart - commandEnd - 1));
    std::string param;
    while (std::getline(iss, param, ' '))
        parameters.push_back(param);

    if (paramsStart != std::string::npos)
    {
        trailing = message.substr(paramsStart + 1);
        if (!trailing.empty() && trailing[0] == ':')
            trailing = trailing.substr(1);
    }
}

void Command::masterCommand(User *user, const std::string& command, std::vector<Channel*> &channel, const std::string& serverPass)
{
	int userSocket = user->getSocket();
    parseIRCMessage(command);
    std::cout << "Commande parser : " << std::endl;
    std::cout << "command name :" << commandName << std::endl;
    std::vector<std::string>:: iterator it = parameters.begin();
    for (; it < parameters.end(); it++)
    {
        std::cout << "parametre :" << *it << std::endl;
    }
    std::cout << "trailing :" << trailing << std::endl;
    std::cout << "fin du parsing" << std::endl;
    
    if (commandName.find("CAP") != std::string::npos)
        processCap(userSocket);
	else if (commandName.find("PASS") != std::string::npos)
		processPass(user, serverPass);
    else if (commandName.find("NICK") != std::string::npos)
        processNick(user);
	/*else if (commandName.find("USER") != std::string::npos)
        processUser(userSocket);*/
    else if (commandName.find("PING") != std::string::npos)
        processPing(userSocket);
    else if (commandName.find("JOIN") != std::string::npos)
        joinChannel(user, channel);
    else if (commandName.find("PRIVMSG") != std::string::npos)
        sendMess(user, channel);
    // else
    //     std::cout << ORANGE << "Command unknown: " << RESET << command << std::endl;
}

void Command::processCap(int userSocket)
{
    if (parameters[0].find("LS") != std::string::npos)
    {
        send(userSocket, "CAP * LS :none\r\n", strlen("CAP * LS :none\r\n"), 0);
    }
    else if (parameters[0].find("END") != std::string::npos)
    {
        std::string welcome = "001 USER :Welcome to the Internet Relay Network\r\n";
        send(userSocket, welcome.c_str(), welcome.size(), 0);
    }
}

void	Command::processPass(User *user, const std::string& serverPass)
{
	int			socket = user->getSocket();
	std::string	clientPass = parameters[0].substr(0, parameters[0].length() - 2);
	std::string	wrongPass = RED "Error : [Wrong password] - closing connexion" RESET;

	if (clientPass != serverPass)
	{
		send(socket, wrongPass.c_str(), wrongPass.size(), 0);
		close(socket);
	}
}

void Command::processNick(User *user)
{
    std::string newNickname = parameters[0].substr(0, parameters[0].length() - 2);
    if (newNickname.empty())
    {
        std::cerr << "Error: Enter a valid nickname" << std::endl;
        return;
    }

    std::string oldNickname = user->getNickname();
    user->setNickname(newNickname);

    std::string response = ":" + oldNickname + " NICK " + newNickname + "\r\n";
    send(user->getSocket(), response.c_str(), response.size(), 0);
}

/*void	Command::processUser(int userSocket)
{
    
}*/

// void	Command::processMode(User *user, const std::string &command)
// {
	
// }

// void	Command::processWhoIs(User *user, const std::string &command)
// {
	
// }

void	Command::processPing(int userSocket)
{
    std::string pingParam = parameters[0];
    std::string pong = "PONG " + pingParam + "\r\n";
    send(userSocket, pong.c_str(), pong.size(), 0);
}

void Command::joinChannel(User *user, std::vector<Channel*> &channels)
{
    std::string name = parameters[0];
    name = extractParameter(name, "#");
    std::vector<Channel*>::iterator it = channels.begin();
    if (name.empty())
        name = "Default";
    for (; it != channels.end(); it++)
    {
        if ((*it)->getName() == name)
        {
            (*it)->addUser(user);
             std::string welcomeMessage = "332 " + user->getNickname() + " " + name + " :" "Welcome to the channel " + name + "\r\n";
            user->sendMessage(welcomeMessage);
            break;
        }
    }
}

void Command::sendMess(User *user, std::vector<Channel*> &channels)
{
    std::string chanName = parameters[0].substr(1);
    std::string message = trailing;

    for (size_t i = 0; i < channels.size(); ++i)
    {
        if (channels[i]->getName() == chanName)
        {
            std::vector<User*> users = channels[i]->getUsers();
            for (size_t j = 0; j < users.size(); ++j)
            {
                if (users[j] != user)
                {
                    std::cout << "Envoi du message au client" << std::endl;
                    users[j]->sendMessage(message);
                }
            }
            break;
        }
    }
}

std::string Command::extractParameter(const std::string& command, const std::string& prefix)
{
    size_t begin = command.find(prefix) + prefix.length();
    size_t end = command.find("\r\n");
    if (begin == std::string::npos || end == std::string::npos)
        return "";
    std::string parameter = command.substr(begin, end - begin);
    parameter.erase(std::remove(parameter.begin(), parameter.end(), '\r'), parameter.end());
    parameter.erase(std::remove(parameter.begin(), parameter.end(), '\n'), parameter.end());
    return parameter;
}