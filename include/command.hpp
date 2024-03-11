/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 15:38:14 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/11 09:29:53 by phudyka          ###   ########.fr       */
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
};

#endif // COMMAND_HPP
