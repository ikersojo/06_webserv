/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:17:18 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/04 23:21:01 by isojo-go         ###   ########.fr       */
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


	class Config
	{
		public:
			Config(const std::string & configFile);
			~Config(void);

			size_t			getMaxPorts(void);
			std::string		getServerName(size_t i);
			int				getPort(size_t i);
			std::string		getAddress(size_t i);
			std::string		getFile(size_t i, std::string req);
			std::string		getErrorPage(size_t i, std::string req);
			int				getBufferSize(size_t i, std::string req);
			bool			isAutoIndex(size_t i, std::string req);
			bool			isRedir(size_t i, std::string req);
			bool			isGET(size_t i, std::string req);
			bool			isPOST(size_t i, std::string req);
			bool			isDELETE(size_t i, std::string req);
			bool			isCgi(size_t i, std::string req);

			void			resizeVectors(size_t size);
			void			printConfig(void);

		private:
			Config(void);

			size_t                                                  _maxPorts;
			std::vector < std::string >                             _servername;
			std::vector < int >                                     _port;
			std::vector < std::string >                             _address;
			std::vector < std::map < std::string, bool > >          _autoindex;
			std::vector < std::map < std::string, std::string > >   _file;
			std::vector < std::map < std::string, bool > >          _allowedGET;
			std::vector < std::map < std::string, bool > >          _allowedPOST;
			std::vector < std::map < std::string, bool > >          _allowedDELETE;
			std::vector < std::map < std::string, std::string > >   _errorPage;
			std::vector < std::map < std::string, int > >           _bufferSize;
			std::vector < std::map < std::string, bool > >          _redir;
			std::vector < std::map < std::string, bool > >          _cgi;
	};

#endif // CONFIG_HPP
