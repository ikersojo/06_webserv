#ifndef SERVER_HPP
	#define SERVER_HPP

	#define BACKLOG		10
	#define	BUFFSIZE	4096

	#include <iostream>
	#include <string>
	#include <cstring>
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <unistd.h>
	#include <vector>
	#include <sys/select.h>

	#include "../../aux/inc/aux.hpp"

	class Server
	{
		public:
			Server(void);
			~Server(void);

			void	loadConfig(const std::string & configFile);
			void	init(void);

		private:
			size_t					_maxPorts;
			std::vector < int >		_ports;
			std::vector < int >		_serverSockets;

			void	startListening(void);
			void	acceptConnections(int serverSocket);
	};

#endif // SERVER_HPP
