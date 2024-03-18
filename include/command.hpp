/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 15:38:14 by phudyka           #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2024/03/15 14:32:27 by dtassel          ###   ########.fr       */
=======
/*   Updated: 2024/03/15 11:48:44 by phudyka          ###   ########.fr       */
>>>>>>> 6832a67cfd9c2bdbd3612ba0759c5adc97c17e7b
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
    void masterCommand(User *user, const std::string& command, std::vector<Channel*> &channel, const std::string& serverPass);

private:
    std::string commandName;
    std::vector<std::string> parameters;
    std::string trailing;
<<<<<<< HEAD
    void parseIRCMessage(const std::string& command);
    void processCap(int userSocket);
    void processCapReq(int userSocket);
    void processCapEnd(int userSocket);
    void processUser(User *user);
    void processPing(int userSocket);
    void joinChannel(User *user, std::vector<Channel*> &channel);
    void processNick(User *user);
	void		processMode(User *user, const std::string &command);
	void		processWhoIs(User *user, const std::string &command);
    void sendMess(User *user, std::vector<Channel*> &channel);
=======
    void	parseIRCMessage(const std::string& command);
    void	processCap(int userSocket);
	void	processPass(User *user, const std::string& serverPass);
    void	processCapReq(int userSocket);
    void	processCapEnd(int userSocket);
    void	processUser(int userSocket);
    void	processPing(int userSocket);
    void	joinChannel(User *user, std::vector<Channel*> &channel);
    void	processNick(User *user);
	void	processMode(User *user, const std::string &command);
	void	processWhoIs(User *user, const std::string &command);
    void	sendMess(User *user, std::vector<Channel*> &channel);
>>>>>>> 6832a67cfd9c2bdbd3612ba0759c5adc97c17e7b
    std::string extractParameter(const std::string& command, const std::string& prefix);
};
#endif // COMMAND_HPP
