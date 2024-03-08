/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/08 15:38:14 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/08 15:56:45 by phudyka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_HPP
#define COMMAND_HPP

# include "ft_irc.hpp"

class User;

class Command
{
public:
    Command();
	Command(const Command& other);
    ~Command();
    Command& operator=(const Command& other);
    void masterCommand(int userSocket, const std::string& command);

private:
    void processUser(int userSocket);
    void processCapReq(int userSocket);
    void processCapEnd(int userSocket);
    void processPing(int userSocket, const std::string& pingCommand);

};

#endif // COMMAND_HPP
