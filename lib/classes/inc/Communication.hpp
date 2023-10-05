/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Communication.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:16:50 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/05 22:54:59 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMUNICATION_HPP
	#define COMMUNICATION_HPP

	#define	BUFFSIZE	4096

	#include <iostream>
	#include <string>
	#include <cstring>
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <unistd.h>
	#include <cstdlib>
	#include <sstream>
	#include <fstream>

	#include "../../aux/inc/aux.hpp"
	#include "../inc/Config.hpp"

	class Config;

	class Communication
	{
		public:
			Communication(int serverSocket, Config * config, int location);
			~Communication(void);

			static void		SignalHandler(int signal);

		private:
			Communication(void);

			static bool			_shutdownRequested;

			bool				_ok;
			int					_serverSocket;
			int					_clientSocket;
			sockaddr_in			_clientAddr;
			socklen_t			_clientAddrLen;
			Config * 			_config;
			int					_location;

			std::string					_requestString; //only for debugging (so far...)
			std::vector < std::string >	_requestParams;
			std::string					_responseStr;

			void	manageRequest(void);

			void	readRequest(void);

				void	handleGetRequest(void);
					void	buildFileResponse(void);

				void	handlePostRequest(void);

				void	handleDeleteRequest(void);

			void	sendResponse(void);
	};

#endif // COMMUNICATION_HPP
