/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarrien- <aarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:17:18 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/10 15:44:27 by aarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
	#define CONFIG_HPP

	#include <iostream>
	#include <string>
	#include <vector>
	#include <map>
	#include <fstream>
	#include <sstream>


	#include "../../aux/inc/aux.hpp"

	struct Location {
		std::string	path;
		bool		autoindex;
		std::string	file;
		std::string	root;
		bool		allowedGET;
		bool		allowedPOST;
		bool		allowedDELETE;
		std::string	errorPage;
		int			bufferSize;

		Location() : \
			path(""), \
			autoindex(false), \
			file("./www/default.html"), \
			root(""), \
			allowedGET(true), \
			allowedPOST(true), \
			allowedDELETE(true), \
			errorPage("./www/404.html"), \
			bufferSize(4096) {}
	};

	class Config
	{
		public:
			Config(const std::string & configFile);
			~Config(void);

			size_t			getMaxPorts(void);
			std::string		getServerName(size_t i);
			int				getPort(size_t i);
			std::string		getAddress(size_t i);
			bool			getAutoIndex(size_t i, std::string req);
			std::string		getFile(size_t i, std::string req);
			bool			getAllowedGET(size_t i, std::string req);
			bool			getAllowedPOST(size_t i, std::string req);
			bool			getAllowedDELETE(size_t i, std::string req);
			std::string		getErrorPage(size_t i, std::string req);
			int				getBufferSize(size_t i, std::string req);

			void			resizeVectors(size_t size);
			void			printConfig(void);

		private:
			Config(void);

			size_t													_maxPorts;
			std::vector < std::string >								_servername;
			std::vector < int >										_port;
			std::vector < std::string >								_address;
			std::vector < std::map < std::string, bool > >			_autoindex;
			std::vector < std::map < std::string, std::string > >	_file;
			std::vector < std::map < std::string, std::string > >	_root;
			std::vector < std::map < std::string, bool > >			_allowedGET;
			std::vector < std::map < std::string, bool > >			_allowedPOST;
			std::vector < std::map < std::string, bool > >			_allowedDELETE;
			std::vector < std::map < std::string, std::string > >	_errorPage;
			std::vector < std::map < std::string, int > >			_bufferSize;
	};

#endif // CONFIG_HPP
