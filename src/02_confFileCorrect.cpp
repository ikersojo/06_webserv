/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02_confFileCorrect.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 21:34:37 by isojo-go          #+#    #+#             */
/*   Updated: 2023/09/12 21:55:30 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/webserv.hpp"

static bool	configOK(void)
{
	debug("Config File OK");
	return (true);
}

static bool	configError(void)
{
	error("Error: Config File: the provided file is not valid");
	return (false);
}

bool confFileCorrect(const char **argv)
{
	std::string filename(argv[1]);

	if (1)
		/*
		TODO:
		Check the config file:
		- 
		-
		-
		-
		-
		-
		-

		*/
			return (configOK());
	return (configError());
}