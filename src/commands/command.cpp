/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtassel <dtassel@42.nice.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 15:37:59 by phudyka           #+#    #+#             */
/*   Updated: 2024/04/23 13:46:47 by dtassel          ###   ########.fr       */
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
            paramsStart = command.find_last_of(':');
            if (paramsStart != std::string::npos)
            {
                trailing = command.substr(paramsStart + 1);
                if (!trailing.empty() && trailing[0] == ':')
                {
                    trailing = trailing.substr(1);
                }
            }
        }
    }
    else
        commandName = command;

    if (trailing.empty())
    {
        for (size_t i = 0; i < parameters.size(); ++i)
        {
            if (parameters[i].find("\r\n") != std::string::npos)
            {
                parameters[i].erase(parameters[i].find("\r\n"));
                break;
            }
        }
    }
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
    if (commandName.find("CAP") != std::string::npos)
    {
        std::string response = CAP_LS_NONE();
        send(user->getSocket(), response.c_str(), response.length(), 0);
    }
	else if (commandName.find("PASS") != std::string::npos)
    {
		if (processPass(user, serverPass) == false)
            close(user->getSocket());
    }
    else if (commandName.find("NICK") != std::string::npos && user->getAuthPass() == true)
        processNick(user, _users, channel);
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
    else if (commandName.find("PING") != std::string::npos || commandName.find("PONG") != std::string::npos)
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
		processKick(user, channel, _users);
	else if (commandName.find("INVITE") != std::string::npos)
		processInvite(user, channel, _users);
	else if (commandName.find("TOPIC") != std::string::npos)
		processTopic(user, channel);
	else if (commandName.find("MODE") != std::string::npos)
		processChannelMode(user, channel, _users);
	else if (commandName.find("QUIT") != std::string::npos)
		processQuit(user);
    else
        {
            std::string response = ERR_UNKNOWNCOMMAND(user->getNickname(), commandName);
            send(user->getSocket(), response.c_str(), response.length(), 0);
        }
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