/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locationInSocketVector.cpp                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/12 08:33:18 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/12 09:26:31 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/aux.hpp"

int		locationInSocketVector(int fd, std::vector < int > socketList)
{
	std::vector < int >::iterator	it = socketList.begin();
	int	i = 0;

	while (it != socketList.end())
	{
		if (*it == fd)
			return (i);
		i++;
		it++;
	}
	return (-1);
}
