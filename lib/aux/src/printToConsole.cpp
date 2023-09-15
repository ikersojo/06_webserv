/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printToConsole.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 19:51:59 by isojo-go          #+#    #+#             */
/*   Updated: 2023/09/15 17:31:28 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/aux.hpp"

void	debug(const std::string & str)
{
	if (DEBUG == 1)
		std::cout << GREY << "[DEBUG: " << str << "]" << DEF_COL << std::endl;
}

void	error(const std::string & str)
{
	std::cerr << RED << str << DEF_COL << std::endl;
}
