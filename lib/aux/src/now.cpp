/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   now.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/07 09:41:38 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/07 09:56:28 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/aux.hpp"

std::string	now(void)
{
	char buffer[22]; // [YYYY-MM-DD HH:MM:SS]

	std::time_t	currentTime = std::time(0);
	std::tm*	timeInfo = std::localtime(&currentTime);
	std::strftime(buffer, sizeof(buffer), "[%Y-%m-%d %H:%M:%S]", timeInfo);
	std::string	timeString (buffer);
	return timeString;
}
