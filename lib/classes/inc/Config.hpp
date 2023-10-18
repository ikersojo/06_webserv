/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarrien- <aarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:17:18 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/18 11:50:55 by aarrien-         ###   ########.fr       */
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

	#define DIRECTIVE_ROOT			"root:"
	#define DIRECTIVE_FILE			"file:"
	#define DIRECTIVE_AUTO_INDEX	"autoindex:"
	#define DIRECTIVE_ALLOW			"allow:"
	#define DIRECTIVE_ERROR_PAGE	"error_page:"
	#define DIRECTIVE_BUFFER_SIZE	"buffer_size:"
	#define DIRECTIVE_CGI			"cgi:"
	#define DIRECTIVE_REDIR			"redirect:"
	#define DIRECTIVE_HANDLE_POST	"handle_post:"
	#define DIRECTIVE_HANDLE_DELETE	"handle_delete:"

	struct Location {
		std::string					path;
		bool						autoindex;
		std::string					file;
		std::string					root;
		bool						allowedGET;
		bool						allowedPOST;
		bool						allowedDELETE;
		std::map<int, std::string>	errorPage;
		int							bufferSize;
		bool						cgi;
		bool						redir;
		std::string					handlePOST;
		std::string					handleDELETE;

		Location() : \
			autoindex(false), \
			allowedGET(true), \
			allowedPOST(true), \
			allowedDELETE(true), \
			bufferSize(4096), \
			cgi(false), \
			redir(false) {}

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
			cgi(ref.cgi), \
			redir(ref.redir), \
			handlePOST(ref.handlePOST), \
			handleDELETE(ref.handleDELETE) {}
	};

	class Config
	{
		public:
			Config(const std::string & configFile);
			~Config(void);

			size_t						getMaxPorts(void);
			std::string					getServerName(size_t i);
			int							getPort(size_t i);
			std::string					getAddress(size_t i);
			bool						isAutoIndex(size_t i, std::string req);
			std::string					getFile(size_t i, std::string req);
			bool						isGET(size_t i, std::string req);
			bool						isPOST(size_t i, std::string req);
			bool						isDELETE(size_t i, std::string req);
			std::map<int, std::string>	getErrorPage(size_t i, std::string req);
			int							getBufferSize(size_t i, std::string req);
			bool						isRedir(size_t i, std::string req);
			bool						isCgi(size_t i, std::string req);

			std::string					getHandlePOST(size_t i, std::string req);
			std::string					getHandleDELETE(size_t i, std::string req);
			bool						isValidRequest(size_t i, std::string req);

			void						resizeVectors(size_t size);
			void						printConfig(void);

			void						setAIFile(size_t i, std::string url, std::string path, std::string file);
			void						setDeletePath(size_t i, std::string url, std::string task);

		private:
			Config(void);

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
			std::vector < std::map < std::string, bool > >							_redir;
			std::vector < std::map < std::string, std::map < int, std::string > > >	_errorPage;
			std::vector < std::map < std::string, int > >							_bufferSize;

			std::vector < std::map < std::string, std::string > >					_handlePOST;
			std::vector < std::map < std::string, std::string > >					_handleDELETE;
	};

#endif // CONFIG_HPP
