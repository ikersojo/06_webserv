/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_main.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarrien- <aarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 15:53:16 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/23 10:18:35 by aarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/webserv.hpp"

int		main(int argc, const char ** argv)
{
	signal(SIGPIPE, SIG_IGN);
	echoOff();
	printLogo();
	debug("starting main program");
	(void)argc;
	//if (syntaxCorrect(argc, argv) && confFileCorrect//(argv))
	//{
		debug("syntax and config file are valid");
		std::string configFile(argv[1]);

		Config* config = new Config(configFile);
		if (DEBUG)
			config->printConfig();
		Server	webserv(config);

		webserv.init();

		delete config;
		echoOn();
	//}
	return (0);
}

/*
TODO List:

isojo-go:
	Include custom error pages
	Implement uploads
	Limit client body size (for POST)
	Chunked POST requests
	autoindex default file (currently manually included index.html)
	Implement CGI:
	- use the full path as PATH_INFO
	- CGI with the file requested as first argument
	- run in the correct directory for relative path file access
	- CGI will expect EOF as end of the body
	- If no content_length is returned from the CGI, EOF will mark the end of the returned data
	Cookies

	Decidir como repartimos root/uri/location en el conf (tendremos que checkear y parsear alineadamente)
 */
