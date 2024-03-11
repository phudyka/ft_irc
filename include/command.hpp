/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dtassel <dtassel@42.nice.fr>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 15:38:14 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/11 13:21:46 by dtassel          ###   ########.fr       */
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
    void processUser(int userSocket);
    void processCapReq(int userSocket);
    void processCapEnd(int userSocket);
    void processHost(int userSocket);
    void processPing(int userSocket, const std::string& pingCommand);
    void joinChannel(User *user, const std::string &command, std::vector<Channel*> &channel);
    void processNick(User *user, const std::string &command);
    std::string extractParameter(const std::string& command, const std::string& prefix);
};

#endif // COMMAND_HPP
