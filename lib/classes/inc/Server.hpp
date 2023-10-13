/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:17:04 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/13 16:01:28 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
	#define SERVER_HPP

	#define BACKLOG		256
	#define TIMEOUT		10000
	#define	BUFFSIZE	4096

	#include <iostream>
	#include <string>
	#include <cstring>
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <unistd.h>
	#include <vector>
	#include <sys/select.h>
	#include <csignal>
	#include <cstdlib>
	#include <sys/event.h>
	#include <fcntl.h>

	#include "../../aux/inc/aux.hpp"
	#include "../inc/ResponseBuilder.hpp"
	#include "../inc/Config.hpp"

	class ResponseBuilder;
	class Config;

	class Server
	{
		public:
			Server(Config * config);
			~Server(void);

			static void		SignalHandler(int signal);

			void	init(void);

		private:
			Server(void);

			static bool					_shutdownRequested;

			// Pointer to config
			Config * 					_config;

			// Server Locations (each index of the vector is aligned with a location)
			int							_maxPorts;
			std::vector < int >			_serverSocket;

			// Client (each vector index aligns socket, request str and response str)
			std::vector < int >			_clientSocket;
			std::vector < int >			_configIndex;
			std::vector < std::string >	_requestStr;
			std::vector < std::string >	_responseStr;

			// Required for I/O management by SELECT
			fd_set						_recvSet;
			fd_set						_sendSet;
			int							_maxSocket;
			struct timeval				_timeout;

			// Listening printing (cosmetic only)
			int							_listenIndex;
			int							_listenWrittenSize;

			// Private methods
			void	startListeningSelect(void);
			void	acceptConnection(int locInServer);
			void	readRequest(int locInClient);
			void	sendResponse(int locInClient);
			void	closeClient(int locInClient, fd_set * set);

	};

#endif // SERVER_HPP
