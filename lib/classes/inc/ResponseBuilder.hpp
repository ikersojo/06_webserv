/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 09:16:00 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/13 15:49:05 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSEBUILDER_HPP
	#define RESPONSEBUILDER_HPP

	#include <iostream>
	#include <string>
	// #include <cstring>
	// #include <unistd.h>
	// #include <cstdlib>
	#include <sstream>
	#include <fstream>
	#include <dirent.h>

	#include "../../aux/inc/aux.hpp"
	#include "../inc/Config.hpp"

	class Config;

	class ResponseBuilder
	{
		public:
			ResponseBuilder(Config * config, int configIndex, std::string requestStr);
			~ResponseBuilder(void);

			static void		SignalHandler(int signal);

			std::string		computeRegularResponse(void);

		private:
			ResponseBuilder(void);

			static bool			_shutdownRequested;

			Config * 			_config;
			int					_configIndex;
			bool				_ok;

			std::string					_requestStr;
			std::vector < std::string >	_requestParams;
			std::string					_responseStr;


			void		assessRequest(void);
			std::string	getResponse(void);

			std::string	errorResponse(int code);
			std::string	fileResponse(void);
			std::string	redirResponse(void);
			std::string	aiResponse(void);



			// std::string	postResponse(void);
			// std::string	deleteResponse(void);

	};

#endif // RESPONSEBUILDER_HPP
