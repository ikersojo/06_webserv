/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trimDoubleQuotes.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 08:41:51 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/02 08:42:35 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/aux.hpp"

std::string	trimDoubleQuotes(const std::string& str)
{
	size_t	start = 0, end = str.length();

	while (start < end && str[start] == '"')
		start++;

	while (end > start && str[end - 1] == '"')
		end--;
	return str.substr(start, end - start);
}
