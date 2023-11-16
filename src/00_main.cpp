/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   00_main.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarrien- <aarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 15:53:16 by isojo-go          #+#    #+#             */
/*   Updated: 2023/11/16 14:36:03 by aarrien-         ###   ########.fr       */
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
	//if (syntaxCorrect(argc, argv) && confFileCorrect(argv))
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

/*
TODO List:

CHECKS:
- siempre tiene que haber una location "/"

CGI:
- Diferneciar GET y POST
- Extraer parametros (tanto para get como post) -->  a un vector
- Diferenciar extensiones
- Construir argv para lanzar execve

Pendiente:

	Implement uploads
	Limit client body size (for POST)
	Chunked POST requests

	Implement CGI:
	- use the full path as PATH_INFO
	- CGI with the file requested as first argument
	- run in the correct directory for relative path file access
	- CGI will expect EOF as end of the body
	- If no content_length is returned from the CGI, EOF will mark the end of the returned data

	Cookies

 */
