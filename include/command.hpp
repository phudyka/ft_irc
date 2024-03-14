/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 15:38:14 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/14 11:31:27 by phudyka          ###   ########.fr       */
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

    void		processCap(int userSocket);
    void		processPass(int UserSocket, std::string correctPassword);
    void		processNick(User *user, const std::string &command);
    void		processUser(User *user, const std::string &command);
    void		processCapEnd(User *user);
	void		processMode(User *user, const std::string &command);
	void		processWhoIs(User *user, const std::string &command);
    void		processPing(int userSocket, const std::string& pingCommand);
    void		joinChannel(User *user, const std::string &command, std::vector<Channel*> &channel);
    std::string	extractParameter(const std::string& command, const std::string& prefix);
};
#endif // COMMAND_HPP
