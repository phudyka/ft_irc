/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   userMode.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: phudyka <phudyka@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/07 10:26:30 by phudyka           #+#    #+#             */
/*   Updated: 2024/03/18 16:00:52 by phudyka          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/userMode.hpp"

UserMode::UserMode()
{
    modeFlags.reset();
}

UserMode::~UserMode() {}

void UserMode::set(Flag mode, bool value)
{
    modeFlags.set(static_cast<size_t>(mode), value);
}

bool UserMode::isSet(Flag mode) const 
{
    return (modeFlags.test(static_cast<size_t>(mode)));
}