/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 09:16:00 by isojo-go          #+#    #+#             */
/*   Updated: 2023/11/10 11:08:11 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSEBUILDER_HPP
	#define RESPONSEBUILDER_HPP

	#include <iostream>
	#include <string>
	#include <sstream>
	#include <fstream>
	#include <dirent.h>
	#include <unistd.h>
	#include <sys/types.h>
	#include <sys/stat.h>
	#include <sys/wait.h>

	#include "../../aux/inc/aux.hpp"
	#include "../inc/Config.hpp"

	class Config;

	class ResponseBuilder
	{
		public:
			ResponseBuilder(Config * config, int configIndex, std::string requestStr);
			~ResponseBuilder(void);

			static void		SignalHandler(int signal);

			std::string		computeResponse(void);

		private:
			ResponseBuilder(void);

			static bool	_shutdownRequested;
			std::map < std::string, std::string >	_mime;

			Config * 	_config;
			int			_configIndex;
			bool		_ok;

			std::string					_requestStr;
			size_t						_bodySize;
			std::vector < std::string >	_requestParams;
			std::vector < std::string >	_cgiArgs;
			std::string					_responseStr;

			// Generic Response Methods of a Web Server:
			void		assessRequest(void);
			std::string	getResponse(void);
			std::string	postResponse(void);
			std::string	deleteResponse(void);

			std::string	errorResponse(int code);
			std::string	fileResponse(void);
			std::string	redirResponse(void);
			std::string	aiResponse(void);

			void		cgiExecute(std::string execFile);
			void		pushCgiArgs(std::string args);
			std::string	cgiGETResponse(void);
			std::string	cgiPOSTResponse(void);
			std::string	cgiResponse(void);

			void		decodeChuncked(void);

			// Backend Functionality Built-in into the server:
			std::string					_handlePOST;
			std::string					_handleDELETE;

				// TODO App:
				std::string		addToList(void);
				std::string		readFromJsonFile(std::string filePath);
				void			writeToJsonFile(std::string task, std::string filePath);
				std::string		removeFromList(void);
				void			clearJsonFile(std::string filePath);

				// Photo Gallery App:
				std::string		uploadPhoto(void);

				// DeChunker App:
				std::string		deChunk(void);
	};

#endif // RESPONSEBUILDER_HPP
