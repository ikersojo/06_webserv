/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarrien- <aarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:17:18 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/16 12:35:10 by aarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
	#define CONFIG_HPP

	#include <iostream>
	#include <string>
	#include <vector>
	#include <map>
	#include <fstream>


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
		std::string	cgiExt;

		Location() : \
			path(""), \
			autoindex(false), \
			file("./www/default.html"), \
			root(""), \
			allowedGET(true), \
			allowedPOST(true), \
			allowedDELETE(true), \
			errorPage("./www/404.html"), \
			bufferSize(4096), \
			cgiExt("") {}

		Location(const Location & ref) : \
			path(ref.path), \
			autoindex(ref.autoindex), \
			file(ref.file), \
			root(ref.root), \
			allowedGET(ref.allowedGET), \
			allowedPOST(ref.allowedPOST), \
			allowedDELETE(ref.allowedDELETE), \
			errorPage(ref.errorPage), \
			bufferSize(ref.bufferSize), \
			cgiExt(ref.cgiExt) {}
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
			bool			isAutoIndex(size_t i, std::string req);
			std::string		getFile(size_t i, std::string req);
			bool			isGET(size_t i, std::string req);
			bool			isPOST(size_t i, std::string req);
			bool			isDELETE(size_t i, std::string req);
			std::string		getErrorPage(size_t i, std::string req);
			int				getBufferSize(size_t i, std::string req);

			std::string		getHandlePOST(size_t i, std::string req);
			std::string		getHandleDELETE(size_t i, std::string req);
			bool			isValidRequest(size_t i, std::string req);

			void			resizeVectors(size_t size);
			void			printConfig(void);

			void			setAIFile(size_t i, std::string url, std::string path, std::string file);
			void			setDeletePath(size_t i, std::string url, std::string task);

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
			std::vector < std::map < std::string, std::string > >	_cgi;
			std::vector < std::map < std::string, bool > >			_redir;
			std::vector < std::map < std::string, std::string > >	_errorPage;
			std::vector < std::map < std::string, int > >			_bufferSize;

			std::vector < std::map < std::string, std::string > >	_handlePOST;
			std::vector < std::map < std::string, std::string > >	_handleDELETE;
	};

#endif // CONFIG_HPP

// faltan las redirecciones
// error_page tiene que completarse con un código de error y un archivo
// pueden configurarse más de un error page con distintos codigos de error
