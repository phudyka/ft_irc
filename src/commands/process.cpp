/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/21 16:58:24 by phudyka           #+#    #+#             */
/*   Updated: 2024/04/25 11:22:56 by phudyka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/command.hpp"

bool	Command::processPass(User *user, const std::string& serverPass)
{
	int			socket = user->getSocket();
	std::string	wrongPass = ERR_PASSWDMISMATCH(user->getNickname());
	
	if (parameters.size() < 1)
	{
		user->sendMessage(wrongPass);
		return false;
	}
	std::string	clientPass = parameters[0];
	if (clientPass != serverPass)
	{
		send(socket, wrongPass.c_str(), wrongPass.size(), 0);
		return false;
	}
    user->setPassword();
    return true;
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


void	Command::processNick(User *user, std::vector<User*> &users, std::vector<Channel*> &channels)
{
    std::string response;
	if (parameters.size() < 1)
	{
		response = ERR_NEEDMOREPARAMS(user->getNickname(), "");
		user->sendMessage(response);
		return ;
	}
    std::string newNickname = parameters[0];
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
        if (user->getJoinedChannels() > 0)
        {
            std::vector<Channel*>::iterator it = channels.begin();
            for (; it != channels.end(); it++)
            {
                if ((*it)->isInChannel(user->getNickname(), (*it)) == true)
                {
                    (*it)->majNickInChannel(oldNickname, newNickname);
                    (*it)->sendToAll(response);
                }
            }
            
        }
    }
    send(user->getSocket(), response.c_str(), response.size(), 0);
    if (user->isAuthentified() == true && !user->temp_USER.empty())
    {
        parseIRCMessage(user->temp_USER);
        processUser(user);
        user->temp_USER.clear();
    }
}

void	Command::processUser(User *user)
{
        if (parameters.size() == 3)
        {
            user->setUsername(parameters[0]);
            user->setRealname(parameters[1]);
            user->setHostname(parameters[2]);

            std::string welcome = ":" + user->getNickname() + "!" + user->getRealname() + "@localhost 001 " + user->getNickname() + " :Welcome to the Internet Relay Network"
                        " " + user->getNickname() + "!" + user->getRealname() + "@" + "localhost" + "\r\n";
            send(user->getSocket(), welcome.c_str(), welcome.length(), 0);
        }
}

void    Command::processPart(User *user, std::vector<Channel *> &channel)
{
    if (parameters.size() > 0)
    {
        std::string channelName;
        channelName = parameters[0].substr(1);
        if (trailing.empty() == true)
            channelName = parameters[0].substr(1);
        std::vector<Channel *>::iterator it = channel.begin();
        for (; it != channel.end(); it++)
        {
            if ((*it)->getName() == channelName)
            {
                user->removeListChannels(*it);
                (*it)->removeUser(user->getNickname());
                std::string reason = "Ciao";
                std::string response = RPL_PART(user_id(user->getNickname(), user->getUsername()), channelName, reason);
				response += RPL_ENDOFNAMES(user->getNickname(), channelName);
                send(user->getSocket(), response.c_str(), response.length(), 0);
            }
        }
    }
}

void	Command::processPing(User *user)
{
    if (commandName.find("PING") != std::string::npos)
    {
        if (!parameters.empty())
        {
            std::string pingParam = parameters[0];
            std::string pong = RPL_PONG(pingParam);
            send(user->getSocket(), pong.c_str(), pong.length(), 0);
        }
    } 
}

void	Command::processJoinChannel(User *user, std::vector<Channel*> &channels)
{
    std::string channelName;
    if (parameters.size() > 0)
    {
        if (parameters[0].find("#") != std::string::npos)
            channelName = parameters[0].substr(1);
        else
        {
            std::string response = ERR_UNKNOWNCOMMAND(user->getNickname(), commandName);
            send(user->getSocket(), response.c_str(), response.length(), 0);
            return;
        }
    }
    else
        return;

    if (channelName.empty())
    {
        std::string response = ERR_NEEDMOREPARAMS(user->getNickname(), commandName);
        send(user->getSocket(), response.c_str(), response.length(), 0);
        return;
    }
	
    std::vector<Channel*>::iterator it;
    for (it = channels.begin(); it != channels.end(); ++it)
    {
        if ((*it)->getName() == channelName)
        {
            if ((*it)->isBanned(user))
            {
                std::string response = ERR_BANNEDFROMCHAN(user->getNickname(), channelName);
                send(user->getSocket(), response.c_str(), response.length(), 0);
                return;
            }
			// Check la limite des users
			size_t	userLimit = (*it)->getUserLimit();
            if (userLimit != static_cast<size_t>(-1) && (*it)->getUsers().size() >= userLimit)
			{
                std::string response = ERR_CHANNELISFULL(user->getNickname(), channelName);
                send(user->getSocket(), response.c_str(), response.length(), 0);
                return ;
            }
            if ((*it)->getMode().find("k") != std::string::npos)
            {
                std::cout << "premiere condition" << std::endl;
                if ((parameters.size() < 2 || (*it)->checkPassword(parameters[1]) == false) && (*it)->getStPass() == true)
                {
                    std::string	wrongPass = ERR_BADCHANNELKEY(user->getNickname(), channelName);
                    send(user->getSocket(), wrongPass.c_str(), wrongPass.length(), 0);
                    return;
                }
            }
            // Ajouter l'utilisateur au canal
            if ((*it)->addUser(user, user->getMode()) == true)
            {
                user->setJoinedChannels(*it);
                std::string client = user_id(user->getNickname(), user->getUsername());

                // Preparer les réponses pour le nouvel utilisateur
                std::string responses;
                std::string symbol = user->getMode();
                std::string list = (*it)->getListInstring();
                responses += RPL_NAMREPLY(user->getNickname(), symbol, channelName, list);
                responses += RPL_ENDOFNAMES(user->getNickname(), channelName);
                responses += RPL_JOIN(client, channelName);

                // Envoyer les réponses au nouvel utilisateur
                send(user->getSocket(), responses.c_str(), responses.length(), 0);

                // Envoyer le nouveau client a tous les utilisateurs present
                std::vector<User*> users = (*it)->getUsers();
                std::vector<User*>::iterator itu = users.begin();
                for (; itu != users.end(); itu++)
                {
                    if ((*itu)->getNickname() != user->getNickname())
                        send((*itu)->getSocket(), responses.c_str(), responses.length(), 0);
                }
                return;
            }
            else // Dans le cas ou mode i et que le client n'est pas invite
            {
                std::string responses;
                responses = ERR_INVITEONLY(user->getNickname(), channelName);
                send(user->getSocket(), responses.c_str(), responses.length(), 0);
                return;
            }
        }
    }

    // Creer un nouveau canal si le canal n'existe pas deja
    Channel *newChannel = new Channel(channelName);
    newChannel->addUser(user, "+o");
    user->setJoinedChannels(newChannel);
    channels.push_back(newChannel);
    std::string client = user_id(user->getNickname(), user->getUsername());
    std::string responses;
    std::string symbol = "+o";
    responses += RPL_JOIN(client, channelName);
    std::string list = (newChannel)->getListInstring();
    responses += RPL_NAMREPLY(user->getNickname(), symbol, channelName, list);
    responses += RPL_ENDOFNAMES(user->getNickname(), channelName);

    send(user->getSocket(), responses.c_str(), responses.length(), 0);
}


void	Command::processSendMess(User *user, std::vector<Channel*> &channels, std::vector<User*> &_users)
{
    std::string	message;

	if (parameters.empty())
	{
		std::string	response = ERR_NOSUCHNICK(user->getNickname(), "");
        user->sendMessage(response);
		return ;
	}
    std::string	target = parameters[0];
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
                if ((*it)->isInChannel(user->getNickname(), (*it)))
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
                else
                {
                    std::string response = ERR_NOTONCHANNEL(user->getNickname(), target);
                    user->sendMessage(response);
                    return;
                }
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
            if ((*it)->getNickname() == target && user->getNickname() != target)
            {
                message = RPL_PRIVMSG(user->getNickname(), user->getUsername(), (*it)->getNickname(), trailing);
                (*it)->sendMessage(message);
                return ;
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

void	Command::processKill(User *user, std::vector<User*> &_users)
{
	if (!user->isOperator())
    {
        std::string response = ERR_NOPRIVILEGES(user->getNickname());
        send(user->getSocket(), response.c_str(), response.size(), 0);
        return ;
    }
	std::string	target = parameters[0];
	std::vector<User*>::iterator	it = _users.begin();
    for (; it != _users.end(); ++it)
    {
        if ((*it)->getNickname() == target)
        {
            std::string	response = RPL_KILL(":" + user->getNickname(), target, "Killed by operator");
            send((*it)->getSocket(), response.c_str(), response.size(), 0);
            close((*it)->getSocket());
            _users.erase(it);
            return ;
        }
    }
}

void	Command::processWhoIs(User *user, std::vector<Channel*> &channels, std::vector<User*> &users)
{
    if (parameters.size() < 1)
    {
        std::string response = ERR_NEEDMOREPARAMS(user->getNickname(), commandName);
        user->sendMessage(response);
        return;
    }
    std::string target = parameters[0];
    if (target[0] == '#')
    {
        std::string channelName = target.substr(1);
        std::vector<Channel*>::iterator it = channels.begin();
        for (; it != channels.end(); it++)
        {
            if ((*it)->getName() == channelName)
            {
                std::string list = ((*it))->getListInstring();
                std::string responses;
                std::string symbol = user->getMode();
                responses += RPL_NAMREPLY(user->getNickname(), symbol, channelName, list);
                responses += RPL_ENDOFNAMES(user->getNickname(), channelName);
                responses += RPL_TOPIC(user->getNickname(), channelName, (*it)->getTopic());
                send(user->getSocket(), responses.c_str(), responses.length(), 0);
                return;
            }
        }
    }
    else
    {
        std::vector<User*>::iterator it = users.begin();
        for (; it != users.end(); it++)
        {
            if ((*it)->getNickname() == target)
            {
                std::string response = RPL_WHOISUSER(user_id(user->getNickname(), user->getUsername()), user->getNickname(), (*it)->getNickname(), (*it)->getUsername(), (*it)->getHostname(), (*it)->getRealname());
                send(user->getSocket(), response.c_str(), response.length(), 0);
                response = RPL_ENDOFWHOIS(user_id(user->getNickname(), user->getUsername()), (*it)->getNickname());
                send(user->getSocket(), response.c_str(), response.length(), 0);
                return ;
            }
        }
    }
}

void	Command::processQuit(User *user)
{
    user->quitAllChannels();
	std::string	response = RPL_QUIT(":" + user->getNickname(), " disconnected");
	send(user->getSocket(), response.c_str(), response.size(), 0);
}