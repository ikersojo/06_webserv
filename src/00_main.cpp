/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_main.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 15:53:16 by isojo-go          #+#    #+#             */
/*   Updated: 2023/09/16 17:13:12 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/webserv.hpp"

int		main(int argc, const char** argv)
{
	debug("starting main program");
	if (syntaxCorrect(argc, argv) && confFileCorrect(argv))
	{
		debug("syntax and config file are valid");
		std::string configFile(argv[1]);

		Server	server;
		server.loadConfig(configFile);
		server.init();

	}
	return (0);
}
