/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   intToString.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/01 08:13:13 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/02 08:13:53 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/aux.hpp"

std::string	intToString(int n)
{
	std::string	str;
	std::stringstream ss;

	ss << n;
	str = ss.str();
	return (str);
}
