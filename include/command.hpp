/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 15:38:14 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/26 17:14:57 by phudyka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP

# include "ft_irc.hpp"

class User;
class Channel;
class Command
{
public:

    Command();
    Command(const Command& other);
    ~Command();
    Command& operator=(const Command& other);
    void masterCommand(User *user, const std::string& command, std::vector<Channel*> &channel, const std::string& serverPass, std::vector<User*> &_users);

private:
    std::string commandName;
    std::vector<std::string> parameters;
    std::string trailing;
	void	processPass(User *user, const std::string& serverPass);
    void	processNick(User *user, std::vector<User*> &users);
    void	processUser(User *user);
	void	processMode(User *user, std::vector<Channel*> &channel);
	void	processWhoIs(User *user);
    void	processPing(User *user);
    void	processJoinChannel(User *user, std::vector<Channel*> &channel);
    void    processPart(User *user, std::vector<Channel *> &channel);
    void	processSendMess(User *user, std::vector<Channel*> &channels, std::vector<User*> &_users);
    void    processList(User *user, std::vector<Channel*> &channel);
    void    processKill(User *user, std::vector<User*> &_users);
    void    processQuit(User *user);
    void	parseIRCMessage(const std::string& command);
    std::string extractParameter(const std::string& command, const std::string& prefix);

// PARSING

    bool    isValidNick(const std::string &nick);
    bool    isAlreadyUse(const std::string &nick, std::vector<User*> &users);
	void	channelMode(int socket, std::vector<Channel*> &channel, std::string channelName, std::string client);
	void	userMode(int socket, bool isSetMode, std::string mode, std::string client, UserMode& uMode);
};
#endif // COMMAND_HPP
