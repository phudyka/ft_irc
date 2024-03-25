/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtassel <dtassel@42.nice.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 16:58:24 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/25 08:58:13 by dtassel          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/command.hpp"

void	Command::processPass(User *user, const std::string& serverPass)
{
	int			socket = user->getSocket();
	std::string	clientPass = parameters[0].substr(0, parameters[0].length() - 2);
	std::string	wrongPass = ERR_PASSWDMISMATCH(user->getNickname());

	if (clientPass != serverPass)
	{
		send(socket, wrongPass.c_str(), wrongPass.size(), 0);
		close(socket);
	}
}

bool    Command::isValidNick(const std::string &nick)
{
    if (nick.length() > 9)
        return false;
    for (size_t i = 0; i < nick.length(); i++)
    {
        if (!isalnum(nick[i]) && nick[i] != '_' && nick[i] != '-')
            return false;
    }
    return true;
}

bool    Command::isAlreadyUse(const std::string &nick, std::vector<User*> &users)
{
    std::vector<User*>::iterator it = users.begin();
    for (; it != users.end(); it++)
    {
        if ((*it)->getNickname() == nick)
            return false;
    }
    return true; 
}

void	Command::processNick(User *user, std::vector<User*> &users)
{
    std::string newNickname = parameters[0].substr(0, parameters[0].length() - 2);
    std::string response;
    if (!isValidNick(newNickname))
        response = ERR_ERRONEUSNICKNAME(user->getNickname(), newNickname);
	else if (newNickname.empty())
		response = ERR_NONICKNAMEGIVEN(user->getNickname());
    else if(!isAlreadyUse(newNickname, users))
    {
        if (user->getNickname().empty())
        {
            std::string tempName = "*";
            response = ERR_NICKNAMEINUSE(tempName, newNickname);
        }
        else
            response = ERR_NICKNAMEINUSE(user->getNickname(), newNickname);
    }
    else
    {
        std::string	oldNickname = user->getNickname();
        user->setNickname(newNickname);
        //:Gerard!freiko@localhost NICK :david
        response = RPL_NICK(oldNickname, user->getUsername(), user->getNickname());
        user->setAuthentified();
    }
    send(user->getSocket(), response.c_str(), response.size(), 0);
    if (user->isAuthentified() == true && !user->temp_USER.empty())
    {
        parseIRCMessage(user->temp_USER);
        processUser(user);
    }
}

void	Command::processUser(User *user)
{
	/*if (user->getHostname() == )
	{
        std::string	response = ERR_ALREADYREGISTERED(user->getNickname());
        send(user->getSocket(), response.c_str(), response.length(), 0);
        return ;
	}*/
        user->setUsername(parameters[0]);
        user->setRealname(parameters[1]);
        user->setHostname(parameters[2]);
        /*if (user->getNickname().empty())
            user->setNickname("*");*/
        std::string welcome = ":" + user->getNickname() + "!" + user->getRealname() + "@localhost 001 " + user->getNickname() + " :Welcome to the Internet Relay Network"
                    " " + user->getNickname() + "!" + user->getRealname() + "@" + "localhost" + "\r\n";
        send(user->getSocket(), welcome.c_str(), welcome.length(), 0);
}

void Command::processMode(User *user)
{
    UserMode& userMode = user->umode();

    std::string client = user->getNickname();
    std::string symbol = parameters[1].substr(0, 1);
    std::string mode = parameters[1].substr(1, parameters[1].length() - 3);

    if (symbol != "+" && symbol != "-")
    {
        std::string	response = ERR_UMODEUNKNOWNFLAG(client);
        send(user->getSocket(), response.c_str(), response.length(), 0);
        return ;
    }

    bool	isSetMode = (symbol == "+");

    if (mode == "i")
        userMode.set(UserMode::INVISIBLE, isSetMode);
    else if (mode == "m")
        userMode.set(UserMode::MARK, isSetMode);
    else
    {
        std::string response = ERR_UMODEUNKNOWNFLAG(client);
        send(user->getSocket(), response.c_str(), response.length(), 0);
        return;
    }
    std::string	response = MODE_USERMSG(client, mode);
    send(user->getSocket(), response.c_str(), response.length(), 0);
    response = RPL_UMODEIS(client, mode);
    send(user->getSocket(), response.c_str(), response.length(), 0);
}

// void	Command::processWhoIs(User *user, const std::string &command)
// {
	
// }

void	Command::processPing(User *user)
{
    std::string	pingParam = parameters[0];
    std::string	pong = RPL_PONG(":" + user->getHostname(), pingParam);
    send(user->getSocket(), pong.c_str(), pong.size(), 0);
}

void	Command::processJoinChannel(User *user, std::vector<Channel*> &channels)
{
    std::string	channelName = parameters[0];
    channelName = extractParameter(channelName, "#");

    if (channelName.empty())
        channelName = "Default";

    std::vector<Channel*>::iterator it;
    for (it = channels.begin(); it != channels.end(); ++it)
    {
        if ((*it)->getName() == channelName)
        {
            if ((*it)->isBanned(user))
            {
                std::string	response = ERR_BANNEDFROMCHAN(user->getNickname(), channelName);
                send(user->getSocket(), response.c_str(), response.length(), 0);
                return ;
            }
            /*if ((*it)->isKeyRequired())
            {
                std::string key;
                if (parameters.size() > 1)
                    key = parameters[1];
                else
                    key = "";
                if (!(*it)->checkKey(key))
                {
                    std::string response = ERR_BADCHANNELKEY(user->getNickname(), channelName);
                    send(user->getSocket(), response.c_str(), response.length(), 0);
                    return;
                }
            }*/
            (*it)->addUser(user);
            user->setJoinedChannels(*it);
            std::string	welcomeMessage = ":" + user->getNickname() + " JOIN #"
                                         + channelName + " :Welcome to the channel "
                                         + channelName + "! " + user->getNickname() + "\r\n";
            user->sendMessage(welcomeMessage);
            std::string response = RPL_JOIN(":" + user->getNickname(), channelName);
            /*(*it)->sendToAll(response);*/
            return ;
        }
    }
    std::string response = "Channel #" + channelName + " not found.\r\n";
    user->sendMessage(response);
}

void	Command::processSendMess(User *user, std::vector<Channel*> &channels, std::vector<User*> &_users)
{
    std::string	message;
    std::string	target = parameters[0].substr(0);

    if (target.empty())
    {
        std::string	response = ERR_NORECIPIENT(user->getNickname());
        user->sendMessage(response);
        return ;
    }
    if (trailing.empty())
    {
        std::string response = ERR_NOTEXTTOSEND(user->getNickname());
        user->sendMessage(response);
        return ;
    }
    if (target.find("#") != std::string::npos)
    {
        target = parameters[0].substr(1);
        for (std::vector<Channel*>::iterator it = channels.begin(); it != channels.end(); ++it)
        {
            if ((*it)->getName() == target)
            {
                std::vector<User*>	users = (*it)->getUsers();
                for (std::vector<User*>::iterator user_it = users.begin(); user_it != users.end(); ++user_it)
                {
                    if (*user_it != user)
                    {
                        std::cout << "Envoi du message au client" << std::endl;
                        message = ":" + user->getNickname() + "!" + user->getUsername();
                        message += "@" + user->getHostname() + " " + commandName + " #" + target + " :" + trailing + "\r\n";
                        (*user_it)->sendMessage(message);
                    }
                }
                return ;
            }
        }
        std::string	response = ERR_NOSUCHNICK(user->getNickname(), target);
        user->sendMessage(response);
    }
    else
    {
        std::vector<User*>::iterator	it = _users.begin();
        for (; it != _users.end(); it++)
        {
            if ((*it)->getNickname() == target)
            {
                std::string	senderPrefix = ":" + user->getNickname() + "!" + user->getUsername() + "@" + user->getHostname();
                message = senderPrefix + " PRIVMSG " + target + " :" + trailing + "\r\n";
                (*it)->sendMessage(message);
                return;
            }
        }
        std::string	response = ERR_NOSUCHNICK(user->getNickname(), target);
        user->sendMessage(response);
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