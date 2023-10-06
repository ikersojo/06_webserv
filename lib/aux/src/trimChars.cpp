/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   trimChar.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarrien- <aarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 08:41:51 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/04 17:09:50 by aarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/aux.hpp"

std::string trimChars(const std::string& str, const std::string& chars)
{
    size_t start = 0, end = str.length();

    while (start < end && chars.find(str[start]) != std::string::npos)
        start++;

    while (end > start && chars.find(str[end - 1]) != std::string::npos)
        end--;

    return str.substr(start, end - start);
}
