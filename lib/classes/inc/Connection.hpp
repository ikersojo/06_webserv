/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:16:50 by isojo-go          #+#    #+#             */
/*   Updated: 2023/09/22 12:04:10 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	#include <sstream>

	#include "../../aux/inc/aux.hpp"
	#include "../inc/Config.hpp"

	class Config;

	class Connection
	{
		public:
			Connection(int serverSocket, Config * config);
			~Connection(void);

			static void		SignalHandler(int signal);

		private:
			Connection(void);

			static bool			_shutdownRequested;

			bool				_ok;
			int					_serverSocket;
			int					_clientSocket;
			sockaddr_in			_clientAddr;
			socklen_t			_clientAddrLen;
			Config * 			_config;

			std::string					_requestString;
			std::vector < std::string >	_requestParams;
			std::string					_responseStr;

			void	manageRequest(void);
			void	readRequest(void);
			void	buildResponse(void);
			void	sendResponse(void);
	};

#endif // CONNECTION_HPP
