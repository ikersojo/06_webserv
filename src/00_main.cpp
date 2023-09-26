/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_main.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 15:53:16 by isojo-go          #+#    #+#             */
/*   Updated: 2023/09/26 22:59:59 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/webserv.hpp"

int		main(int argc, const char** argv)
{
	printLogo();
	debug("starting main program");
	if (syntaxCorrect(argc, argv) && confFileCorrect(argv))
	{
		debug("syntax and config file are valid");
		std::string configFile(argv[1]);

		Config* config = new Config(configFile);
		// if (DEBUG)
		// 	config.printConfig();
		Server	webserv(config);

		webserv.init();

		delete config;
	}
	return (0);
}
