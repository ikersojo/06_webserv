/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_main.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarrien- <aarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 15:53:16 by isojo-go          #+#    #+#             */
/*   Updated: 2023/11/16 15:48:37 by aarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/webserv.hpp"

int		main(int argc, const char ** argv, char ** envp)
{
	signal(SIGPIPE, SIG_IGN);
	echoOff();
	printLogo();
	debug("starting main program");
	(void)argc;
	if (syntaxCorrect(argc, argv) && confFileCorrect(argv))
	{
		debug("syntax and config file are valid");
		std::string configFile(argv[1]);

		Config* config = new Config(configFile, envp);
		if (DEBUG)
			config->printConfig();
		Server	webserv(config);

		webserv.init();

		delete config;
		echoOn();
	}
	return (0);
}
