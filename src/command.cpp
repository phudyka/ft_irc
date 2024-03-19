/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtassel <dtassel@42.nice.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 15:37:59 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/19 10:56:27 by dtassel          ###   ########.fr       */
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
    size_t commandEnd = command.find(' ');
    if (commandEnd != std::string::npos)
    {
        commandName = command.substr(0, commandEnd);

        size_t paramsStart = command.find(' ', commandEnd + 1);

        if (command[commandEnd + 1] == ':')
            trailing = command.substr(commandEnd + 2);
        else
        {
            std::string paramsString = command.substr(commandEnd + 1);
            std::stringstream iss(paramsString);
            std::string param;
            while (std::getline(iss, param, ' '))
            {
                if (param.find(':') != std::string::npos)
                    break;
                parameters.push_back(param);
            }
            paramsStart = command.find(':');
            if (paramsStart != std::string::npos)
            {
                trailing = command.substr(paramsStart + 1);
                if (!trailing.empty() && trailing[0] == ':') {
                    trailing = trailing.substr(1);
                }
            }
        }
    }
    else
        commandName = command;
}

void Command::masterCommand(User *user, const std::string& command, std::vector<Channel*> &channel, const std::string& serverPass, std::vector<User*> &_users)
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
    
	if (commandName.find("PASS") != std::string::npos)
		processPass(user, serverPass);
    else if (commandName.find("NICK") != std::string::npos)
        processNick(user);
	else if (commandName.find("USER") != std::string::npos)
        processUser(user);
	else if (commandName.find("MODE") != std::string::npos)
		processMode(user);
	// else if (commandName.find("WHO") != std::string::npos)
	// 	processWhoIs(user);
    else if (commandName.find("PING") != std::string::npos)
        processPing(userSocket);
    else if (commandName.find("JOIN") != std::string::npos)
        joinChannel(user, channel);
    else if (commandName.find("PRIVMSG") != std::string::npos)
        sendMess(user, channel, _users);
    else if (commandName.find("LIST") != std::string::npos)
        processList(user, channel);
    // else
    //     std::cout << ORANGE << "Command unknown: " << RESET << command << std::endl;
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

void	Command::processNick(User *user)
{
    std::string newNickname = parameters[0].substr(0, parameters[0].length() - 2);
    if (newNickname.empty())
    {
        std::cerr << "Error: Enter a valid nickname" << std::endl;
        return ;
    }
    std::string oldNickname = user->getNickname();
    user->setNickname(newNickname);
    //:Gerard!freiko@localhost NICK :david
    std::string response = ":" + oldNickname + "!" + user->getUsername() + "@" + "localhost" + " NICK :" + user->getNickname() + "\r\n";
    send(user->getSocket(), response.c_str(), response.size(), 0);
}

void	Command::processUser(User *user)
{
    user->setUsername(parameters[0]);
    user->setRealname(parameters[1]);
    user->setHostname(parameters[2]);
    std::string welcome = ":" + user->getNickname() + "!" + user->getRealname() + "@localhost 001 " + user->getNickname() + " :Welcome to the Internet Relay Network"
                " " + user->getNickname() + "!" + user->getRealname() + "@" + "localhost" + "\r\n";
    send(user->getSocket(), welcome.c_str(), welcome.length(), 0);
}

void	Command::processMode(User *user)
{
    UserMode&	userMode = user->umode();

    std::string	symbol = parameters[1].substr(0, 1);
    std::string	mode = parameters[1].substr(1, parameters[1].length() - 3);

	std::cout << "Symbol: " << symbol << ", Mode: " << mode << "]" << std::endl;
    if (symbol == "+")
    {
        if (mode == "i")
            userMode.set(UserMode::INVISIBLE, true);
        else if (mode == "m")
            userMode.set(UserMode::MARK, true);
    }
    else if (symbol == "-")
    {
        if (mode == "i")
            userMode.set(UserMode::INVISIBLE, false);
        else if (mode == "m")
            userMode.set(UserMode::MARK, false);
    }
}

// void	Command::processWhoIs(User *user, const std::string &command)
// {
	
// }

void	Command::processPing(int userSocket)
{
    std::string pingParam = parameters[0];
    std::string pong = "PONG " + pingParam + "\r\n";
    send(userSocket, pong.c_str(), pong.size(), 0);
}

void	Command::joinChannel(User *user, std::vector<Channel*> &channels)
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
            user->setJoinedChannels(*it);
            std::string welcomeMessage = ":" + user->getNickname() + " JOIN #" + name + " :" "Welcome to the channel " + name + "! " + user->getNickname() + "\r\n";
            user->sendMessage(welcomeMessage);
            break ;
        }
    }
}

void	Command::sendMess(User *user, std::vector<Channel*> &channels, std::vector<User*> &_users)
{
    std::string target = parameters[0].substr(0);
    std::string message;
    if (target.find("#") != std::string::npos)
    {
        target = parameters[0].substr(1);
        for (size_t i = 0; i < channels.size(); ++i)
        {
            if (channels[i]->getName() == target)
            {
                std::vector<User*> users = channels[i]->getUsers();
                for (size_t j = 0; j < users.size(); ++j)
                {
                    if (users[j] != user)
                    {
                        //:senderNickname!senderUsername@senderHostname PRIVMSG #channelName :messageContent
                        std::cout << "Envoi du message au client" << std::endl;
                        message = ":" + user->getNickname() + "!" + user->getUsername();
                        message += "@" + user->getHostname() + " " + commandName + " #" + target + " :" + trailing + "\r\n";
                        users[j]->sendMessage(message);
                    }
                }
                break ;
            }
        }
    }
    else
    {
        std::vector<User*>::iterator it = _users.begin();
        for (; it != _users.end(); it++)
        {
            if ((*it)->getNickname() == target)
            {
                std::string senderPrefix = ":" + user->getNickname() + "!" + user->getUsername() + "@" + user->getHostname();
                message = senderPrefix + " PRIVMSG " + target + " :" + trailing + "\r\n";
                (*it)->sendMessage(message);
                break;
            }    
        } 
    }
}

void    Command::processList(User *user, std::vector<Channel*> &channel)
{
    std::vector<Channel*>::iterator it = channel.begin();
    std::vector<std::string> list;
    for (; it != channel.end(); it++)
    {
        list.push_back((*it)->getName());
    }
    std::string response = "LIST ";
    std::vector<std::string>::iterator its = list.begin();
    for (; its != list.end(); its++)
    {
        response += "#" + *its;
    }
    response += "\r\n";
    send(user->getSocket(), response.c_str(), response.size(), 0);
    std::cout << response << std::endl;
}

std::string	Command::extractParameter(const std::string& command, const std::string& prefix)
{
    size_t begin = command.find(prefix) + prefix.length();
    size_t end = command.find("\r\n");
    if (begin == std::string::npos || end == std::string::npos)
        return ("");
    std::string parameter = command.substr(begin, end - begin);
    parameter.erase(std::remove(parameter.begin(), parameter.end(), '\r'), parameter.end());
    parameter.erase(std::remove(parameter.begin(), parameter.end(), '\n'), parameter.end());
    return (parameter);
}