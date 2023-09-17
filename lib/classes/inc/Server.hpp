#ifndef SERVER_HPP
	#define SERVER_HPP

	#define BACKLOG 10

	#include <iostream>
	#include <string>
	#include <cstring>
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <unistd.h>
	#include <vector>
	#include <sys/select.h>
	#include <pthread.h>

	#include "../../aux/inc/aux.hpp"

	class Server
	{
		public:
			Server(void);
			~Server(void);

			void loadConfig(const std::string & configFile);
			void init(void);

		private:
			std::vector < int >			_ports;
			std::vector < int >			_serverSockets;
			size_t						_maxPorts;

			void			startListeningOnAllPorts(void);
			void			acceptConnections(int threadIndex);
	};

#endif // SERVER_HPP
