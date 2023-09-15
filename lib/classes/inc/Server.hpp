#ifndef SERVER_HPP
	#define SERVER_HPP

	#define BACKLOG 10

	#include <iostream>
	#include <string>
	#include <cstring>
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <unistd.h>

	#include "../../aux/inc/aux.hpp"

	class Server
	{
		public:
			Server(int port);
			~Server(void);

		private:
			Server(void);

			int		_port;
			int		_serverSocket;

			void acceptConnections(void);
	};

#endif // SERVER_HPP
