/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarrien- <aarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 09:16:00 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/26 15:12:53 by aarrien-         ###   ########.fr       */
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

			Config * 	_config;
			int			_configIndex;
			bool		_ok;

			std::string					_requestStr;
			std::vector < std::string >	_requestParams;
			std::vector < std::string >	_cgiArgs;
			std::string					_responseStr;

			std::map < std::string, std::string >	_mime;

			void		assessRequest(void);
			void		checkLocation(std::string trimmedURL);
			std::string	getResponse(void);
			std::string	postResponse(void);
			std::string	deleteResponse(void);

			std::string	errorResponse(int code);
			std::string	fileResponse(void);
			std::string	redirResponse(void);
			std::string	aiResponse(void);

			std::string	cgiGETResponse(void);
			std::string	cgiPOSTResponse(void);

			// TODO App
			void	addToList(std::string filePath);
			void	removeFromList(int taskIndex, std::string filePath);
			void	writeToJsonFile(std::string task, std::string filePath);
			void	clearJsonFile(std::string filePath);

	};

#endif // RESPONSEBUILDER_HPP
