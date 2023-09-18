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

	#include "../../aux/inc/aux.hpp"
	#include "../inc/Connection.hpp"

	class Connection;

	class Server
	{
		public:
			Server(void);
			~Server(void);

			static void		SignalHandler(int signal);

			void	loadConfig(const std::string & configFile);
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
