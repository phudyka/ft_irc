/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtassel <dtassel@42.nice.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 15:37:59 by phudyka           #+#    #+#             */
/*   Updated: 2024/04/03 11:11:58 by dtassel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/command.hpp"

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
    
    parseIRCMessage(command);
    std::cout << commandName << std::endl;
    std::vector<std::string>::iterator it = parameters.begin();
    for (; it != parameters.end(); it++)
    {
        std::cout << "Parametre : " << (*it) << std::endl;
    }
	if (commandName.find("PASS") != std::string::npos)
		processPass(user, serverPass);
    else if (commandName.find("NICK") != std::string::npos)
        processNick(user, _users);
	else if (commandName.find("USER") != std::string::npos)
    {
        if (user->isAuthentified() == true)
            processUser(user);
        else
            user->temp_USER = command;
    }
	else if (commandName.find("MODE") != std::string::npos)
		processMode(user, channel, _users);
	else if (commandName.find("WHO") != std::string::npos || commandName.find("WHOIS") != std::string::npos)
	 	processWhoIs(user, channel, _users);
    else if (commandName.find("PING") != std::string::npos)
        processPing(user);
    else if (commandName.find("JOIN") != std::string::npos)
        processJoinChannel(user, channel);
    else if (commandName.find("PART") != std::string::npos)
        processPart(user, channel);
    else if (commandName.find("PRIVMSG") != std::string::npos)
        processSendMess(user, channel, _users);
    else if (commandName.find("LIST") != std::string::npos)
        processList(user, channel);
	else if (commandName.find("KICK") != std::string::npos)
		processKick(user);
	// else if (commandName.find("INVITE") != std::string::npos)
	// 	processInvite(user, channel);
	// else if (commandName.find("TOPIC") != std::string::npos)
	// 	processTopic(user, channel);
	// else if (commandName.find("MODE") != std::string::npos)
	// 	processMode(user, channel);
	// else if (commandName.find("KILL") != std::string::npos)
	// 	processKill(user, _users);
	else if (commandName.find("QUIT") != std::string::npos)
		processQuit(user);
    else
        std::cout << ORANGE << "Command unknown: " << RESET << command << std::endl;
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