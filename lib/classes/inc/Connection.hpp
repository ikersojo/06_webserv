#ifndef CONNECTION_HPP
	#define CONNECTION_HPP

	#define	BUFFSIZE	4096

	#include <iostream>
	#include <string>
	#include <cstring>
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <unistd.h>
	#include <cstdlib>

	#include "../../aux/inc/aux.hpp"

	class Connection
	{
		public:
			Connection(int serverSocket);
			~Connection(void);

		private:
			Connection(void);
			int					_serverSocket;
			int					_clientSocket;
			sockaddr_in			_clientAddr;
			socklen_t			_clientAddrLen;

			void	receiveRequest(void);
	};

#endif // CONNECTION_HPP
