/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   userMode.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:25:43 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/26 11:08:36 by phudyka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef USERMODE_HPP
#define USERMODE_HPP

#include <bitset>
#include "../include/ft_irc.hpp"

class UserMode
{
public:
    enum Flag
	{
        INVISIBLE,
        MARK,
        NUM_MODES,
		OPERATOR
    };

private:
    std::bitset<NUM_MODES> modeFlags;

public:
    UserMode();
    ~UserMode();
    void set(Flag mode, bool value);
    bool isSet(Flag mode) const;
};

#endif