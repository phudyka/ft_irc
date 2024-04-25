/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtassel <dtassel@42.nice.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 15:38:14 by phudyka           #+#    #+#             */
/*   Updated: 2024/04/25 08:25:24 by dtassel          ###   ########.fr       */
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
    int masterCommand(User *user, const std::string& command, std::vector<Channel*> &channel, const std::string& serverPass, std::vector<User*> &_users);

private:
    std::string commandName;
    std::vector<std::string> parameters;
    std::string trailing;
	bool	processPass(User *user, const std::string& serverPass);
    void	processNick(User *user, std::vector<User*> &users, std::vector<Channel*> &channels);
    void	processUser(User *user);
	void	processMode(User *user, std::vector<Channel*> &channel, std::vector<User*> &users);
	void	processWhoIs(User *user, std::vector<Channel*> &channels, std::vector<User*> &users);
    void	processPing(User *user);
    void	processJoinChannel(User *user, std::vector<Channel*> &channel);
    void    processPart(User *user, std::vector<Channel *> &channel);
    void	processSendMess(User *user, std::vector<Channel*> &channels, std::vector<User*> &_users);
    void    processList(User *user, std::vector<Channel*> &channel);
    void    processKill(User *user, std::vector<User*> &_users);
    void    processQuit(User *user);
	void	processKick(User *user, std::vector<Channel*> &channel, std::vector<User*> &users);
	void	processInvite(User *user, std::vector<Channel*> &channels, std::vector<User*> &users);
	void	processTopic(User *user, std::vector<Channel*> &channels);
    void	parseIRCMessage(const std::string& command);
    std::vector<Channel *>::iterator    searchChannelName(std::string channelName, std::vector<Channel*> &channel);

// PARSING

    bool    isValidNick(const std::string &nick);
    bool    isAlreadyUse(const std::string &nick, std::vector<User*> &users);
    void    channelMode(User *user, std::vector<Channel*> &channel);
	void	processUserMode(User *user, std::vector<Channel *> &channels);
	void	processChannelMode(User *user, std::vector<Channel*> &channels, std::vector<User*> &users);
	void	addMode(User *user, std::vector<Channel*> &channel, std::string channelName, std::vector<User*> &users);
	void	removeMode(User *user, std::vector<Channel*> &channel, std::string channelName, std::vector<User*> &users);
};
#endif
