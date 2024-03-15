/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtassel <dtassel@42.nice.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 15:38:14 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/15 08:44:27 by dtassel          ###   ########.fr       */
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
    void masterCommand(User *user, const std::string& command, std::vector<Channel*> &channel);

private:
    std::string commandName;
    std::vector<std::string> parameters;
    std::string trailing;
    void parseIRCMessage(const std::string& command);
    void processCap(int userSocket);
    void processCapReq(int userSocket);
    void processCapEnd(int userSocket);
    void processUser(int userSocket);
    void processPing(int userSocket);
    void joinChannel(User *user, std::vector<Channel*> &channel);
    void processNick(User *user);
    void sendMess(User *user, std::vector<Channel*> &channel);
    std::string extractParameter(const std::string& command, const std::string& prefix);
};

#endif // COMMAND_HPP
