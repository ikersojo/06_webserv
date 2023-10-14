/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   countOccurrences.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/14 10:36:56 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/14 10:38:30 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/aux.hpp"

size_t countOccurrences(const std::string& str, char ch)
{
	size_t count = 0;
	size_t i = 0;

	while (i < str.size())
	{
		if (str[i] == ch)
			count++;
		i++;
	}
	return count;
}
