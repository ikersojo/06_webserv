/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:17:04 by isojo-go          #+#    #+#             */
/*   Updated: 2023/09/20 22:27:14 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
	#define SERVER_HPP

	#define BACKLOG		10


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

	#include "../../aux/inc/aux.hpp"
	#include "../inc/Connection.hpp"
	#include "../inc/Config.hpp"

	class Connection;
	class Config;

	class Server
	{
		public:
			Server(void);
			~Server(void);

			static void		SignalHandler(int signal);

			void	loadConfig(Config config);
			void	init(void);

		private:
			static bool				_shutdownRequested;
			size_t					_maxPorts;
			std::vector < int >		_ports;
			std::vector < int >		_serverSockets;

			void	startListening(void);
			void	acceptConnections(int serverSocket);
	};

#endif // SERVER_HPP
