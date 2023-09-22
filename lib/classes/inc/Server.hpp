/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:17:04 by isojo-go          #+#    #+#             */
/*   Updated: 2023/09/22 08:14:49 by isojo-go         ###   ########.fr       */
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
			Server(Config * config);
			~Server(void);

			static void		SignalHandler(int signal);

			void	init(void);

		private:
			Server(void);

			static bool				_shutdownRequested;

			size_t					_maxPorts;
			std::vector < int >		_ports;
			std::vector < int >		_serverSockets;
			Config * 				_config;

			void	startListening(void);
	};

#endif // SERVER_HPP
