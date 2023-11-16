/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarrien- <aarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:17:18 by isojo-go          #+#    #+#             */
/*   Updated: 2023/11/16 14:35:14 by aarrien-         ###   ########.fr       */
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

	#define D_SERVER				"server:"
	#define D_SERVER_NAME			"servername:"
	#define D_LISTEN				"listen:"
	#define D_LOCATION				"location:"
	#define D_ROOT					"root:"
	#define D_FILE					"file:"
	#define D_AUTO_INDEX			"autoindex:"
	#define D_ALLOW					"allow:"
	#define D_ERROR_PAGE			"error_page:"
	#define D_BUFFER_SIZE			"buffer_size:"
	#define D_UPLOAD				"upload:"
	#define D_CGI					"cgi:"
	#define D_REDIR					"redirect:"
	#define D_HANDLE_POST			"handle_post:"
	#define D_HANDLE_DELETE			"handle_delete:"

	#define GENERAL_LOCATION_ROUTE	"GENERAL"

	#define EXTRACT_CLEAN_VALUE(line) trimChars((line).substr((line).find(":") + 1), " \"")
	#define STRING_CONTAINS(str, target) ((str).find(target) != std::string::npos)

	struct Location {
		std::string					path;
		bool						autoindex;
		std::string					file;
		std::string					root;
		bool						allowedGET;
		bool						allowedPOST;
		bool						allowedDELETE;
		std::map<int, std::string>	errorPage;
		std::string					upload;
		int							bufferSize;
		bool						cgi;
		std::string					cgiExt;
		bool						redir;
		std::string					handlePOST;
		std::string					handleDELETE;

		Location() : \
			autoindex(false), \
			allowedGET(true), \
			allowedPOST(true), \
			allowedDELETE(true), \
			upload("upload"), \
			bufferSize(-1), \
			cgi(false), \
			redir(false) {}
	};

	class Config
	{
		public:
			Config(const std::string & configFile, char ** envp);
			~Config(void);

			char **						getEnvp(void);
			size_t						getMaxPorts(void);
			std::string					getServerName(size_t i);
			int							getPort(size_t i);
			std::string					getAddress(size_t i);
			std::string					getFile(size_t i, std::string req);
			std::string					getRoot(size_t i, std::string req);
			bool						isAutoIndex(size_t i, std::string req);
			bool						isGET(size_t i, std::string req);
			bool						isPOST(size_t i, std::string req);
			bool						isDELETE(size_t i, std::string req);
			std::map<int, std::string>	getErrorPage(size_t i, std::string req);
			std::string					getUpload(size_t i, std::string req);
			int							getBufferSize(size_t i, std::string req);
			bool						isRedir(size_t i, std::string req);
			bool						isCgi(size_t i, std::string req);
			std::string					getCgi(size_t i, std::string req);

			std::string					getHandlePOST(size_t i, std::string req);
			std::string					getHandleDELETE(size_t i, std::string req);

			void						printConfig(void);
			std::string					getActualPath(size_t i, std::string req);
			std::string					getNearestLocation(size_t i, std::string uri);

		private:
			Config(void);

			char **																	_envp;
			size_t																	_maxPorts;
			std::vector < std::string >												_servername;
			std::vector < int >														_port;
			std::vector < std::string >												_address;
			std::vector < std::map < std::string, bool > >							_autoindex;
			std::vector < std::map < std::string, std::string > >					_file;
			std::vector < std::map < std::string, std::string > >					_root;
			std::vector < std::map < std::string, bool > >							_allowedGET;
			std::vector < std::map < std::string, bool > >							_allowedPOST;
			std::vector < std::map < std::string, bool > >							_allowedDELETE;
			std::vector < std::map < std::string, bool > >							_cgi;
			std::vector < std::map < std::string, std::string > >					_cgiExt;
			std::vector < std::map < std::string, bool > >							_redir;
			std::vector < std::map < std::string, std::map < int, std::string > > >	_errorPage;
			std::vector < std::map < std::string, int > >							_bufferSize;
			std::vector < std::map < std::string, std::string > >					_upload;

			std::vector < std::map < std::string, std::string > >					_handlePOST;
			std::vector < std::map < std::string, std::string > >					_handleDELETE;
	};

#endif // CONFIG_HPP
