/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   99_debug.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 19:51:59 by isojo-go          #+#    #+#             */
/*   Updated: 2023/09/12 20:41:51 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/webserv.hpp"

void	debug(const std::string & str)
{
	if (DEBUG == 1)
		std::cout << GREY << "[DEBUG: " << str << "]" << DEF_COL << std::endl;
}
