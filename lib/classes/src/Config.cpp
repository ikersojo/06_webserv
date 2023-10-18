/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarrien- <aarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:33:58 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/18 11:52:10 by aarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Config.hpp"

// Returns the clean value of a certain directive contained in line (input parameter).
std::string	extractCleanValue(std::string & line) {
	return (trimChars(line.substr(line.find(":")+1), " \""));
}

// Returns the amount of listen (actual ports) directives found in the config file
size_t	getNumberOfPorts(const std::string & configFile) {
	std::ifstream	inFile(configFile);
	std::string		line;
	size_t			n;

	if (!inFile.is_open())
	{
		error("Failed to open configuration file");
		exit(EXIT_FAILURE);
	}
	n = 0;
	while (getline(inFile, line))
	{
		if ((line.find("listen:") != std::string::npos))
			n++;
	}
	inFile.close();
	debug("ports to listen to identified");
	return (n);
}

void	Config::setAIFile(size_t i, std::string url, std::string path, std::string file) {
	std::string	fullPath;

	fullPath = path + file;

	this->_autoindex[i][url] = false;
	this->_redir[i][url] = false;
	this->_cgi[i][url] = false;
	this->_file[i][url] = fullPath;
	this->_allowedGET[i][url] = true;
	this->_allowedPOST[i][url] = false;
	this->_allowedDELETE[i][url] = false;
	//this->_errorPage[i][url] = "./www/def404.html";
	this->_bufferSize[i][url] = 4096;
}

void	Config::setDeletePath(size_t i, std::string url, std::string task) {
	this->_autoindex[i][url] = false;
	this->_redir[i][url] = false;
	this->_cgi[i][url] = false;
	this->_file[i][url] = task;
	this->_allowedGET[i][url] = false;
	this->_allowedPOST[i][url] = false;
	this->_allowedDELETE[i][url] = true;
	//this->_errorPage[i][url] = "./www/def404.html";
	this->_bufferSize[i][url] = 4096;
	this->_handleDELETE[i][url] = "removeFromList";
}

// Returns all address:port pairs in each server.
// The first vector would be the server and the second each pair.
std::vector< std::vector< std::pair<std::string, std::string> > >	getListenPoints(const std::string & configFile) {
	std::ifstream														inFile(configFile);
	std::vector< std::vector< std::pair<std::string, std::string> > >	res;
	std::vector< std::pair<std::string, std::string> >					serv;
	std::string															line, address, port;
	size_t																nServer = 0;

	if (!inFile.is_open())
	{
		error("Failed to open configuration file");
		exit(EXIT_FAILURE);
	}

	while (getline(inFile, line)) {
		if (line.find("server:") != std::string::npos) {
			if (nServer != 0) {
				res.push_back(serv);
				serv.clear();
			}
			nServer++;
		}
		if (line.find("listen:") != std::string::npos) {
			line = line.substr(line.find(":")+1);
			if (line.find(":") == std::string::npos) {
				if (line.find(".") != std::string::npos) {
					address = trimChars(line.substr(line.find(":")+1, line.rfind(":")-line.find(":")-1), " ");
					port = "80";
				} else {
					address = "localhost";
					port = trimChars(line.substr(line.rfind(":")+1), " ");
				}
			} else {
				address = trimChars(line.substr(0, line.find(":")), " ");
				port = trimChars(line.substr(line.rfind(":")+1), " ");
			}
			serv.push_back(make_pair(address, port));
		}
	}
	res.push_back(serv);
	inFile.close();
	return (res);
}

// Generates a map with each location's path as the key and the specified directive's value as the value.
// Each CreateMap function creates a different map depending on the directive value type.

std::map<std::string, std::string>	createMapString(std::vector<Location> allServerLocs, std::string directive) {
	std::map<std::string, std::string> Map;

	for (size_t i = 0; i < allServerLocs.size(); i++) {
		if (directive == "file") Map[allServerLocs[i].path] = allServerLocs[i].file;
		else if (directive == "root") Map[allServerLocs[i].path] = allServerLocs[i].root;
		else if (directive == "handlePOST") Map[allServerLocs[i].path] = allServerLocs[i].handlePOST;
		else if (directive == "handleDELETE") Map[allServerLocs[i].path] = allServerLocs[i].handleDELETE;
	}

	return (Map);
}

std::map<std::string, bool>	createMapBool(std::vector<Location> allServerLocs, std::string directive) {
	std::map<std::string, bool> Map;

	for (size_t i = 0; i < allServerLocs.size(); i++) {
		if (directive == "autoindex") Map[allServerLocs[i].path] = allServerLocs[i].autoindex;
		else if (directive == "allowedGET") Map[allServerLocs[i].path] = allServerLocs[i].allowedGET;
		else if (directive == "allowedPOST") Map[allServerLocs[i].path] = allServerLocs[i].allowedPOST;
		else if (directive == "allowedDELETE") Map[allServerLocs[i].path] = allServerLocs[i].allowedDELETE;
		else if (directive == "cgi") Map[allServerLocs[i].path] = allServerLocs[i].cgi;
		else if (directive == "redir") Map[allServerLocs[i].path] = allServerLocs[i].redir;
	}

	return (Map);
}

std::map<std::string, int>	createMapInt(std::vector<Location> allServerLocs, std::string directive) {
	std::map<std::string, int> Map;

	for (size_t i = 0; i < allServerLocs.size(); i++) {
		if (directive == "bufferSize") Map[allServerLocs[i].path] = allServerLocs[i].bufferSize;
	}

	return (Map);
}

std::map<std::string, std::map<int, std::string> >	createMapMap(std::vector<Location> allServerLocs, std::string directive) {
	std::map<std::string, std::map<int, std::string> > Map;

	for (size_t i = 0; i < allServerLocs.size(); i++) {
		if (directive == "errorPage") Map[allServerLocs[i].path] = allServerLocs[i].errorPage;
	}

	return (Map);
}

// Modifies an existing Location structure based on the following lines it reads
// until it finds an "location:" directive.
void	setLocation(std::ifstream& inFile, std::string& line, Location& location) {
	location.path = extractCleanValue(line);
	while (getline(inFile, line) && !line.empty() && line.find("location:") == std::string::npos) {
		if (line.find(DIRECTIVE_ROOT) != std::string::npos)
			location.root = extractCleanValue(line);
		if (line.find(DIRECTIVE_FILE) != std::string::npos) {
			if (!location.redir)
				location.file = extractCleanValue(line);
		}
		if (line.find(DIRECTIVE_AUTO_INDEX) != std::string::npos)
			if (line.find("on") != std::string::npos) location.autoindex = true;
		if (line.find(DIRECTIVE_ALLOW) != std::string::npos) {
			(line.find("GET") != std::string::npos) ? location.allowedGET = true : location.allowedGET = false;
			(line.find("POST") != std::string::npos) ? location.allowedPOST = true : location.allowedPOST = false;
			(line.find("DELETE") != std::string::npos) ? location.allowedDELETE = true : location.allowedDELETE = false;
		}
		if (line.find(DIRECTIVE_ERROR_PAGE) != std::string::npos) {
			line = trimChars(line.substr(line.find(":")+1), " ");
			int code = std::atoi(trimChars(line.substr(0, line.find(" ")), " \"").c_str());
			std::string page = trimChars(line.substr(line.find(" ")+1), " \"");
			location.errorPage[code] = page;
		}
		if (line.find(DIRECTIVE_BUFFER_SIZE) != std::string::npos)
			location.bufferSize = std::atoi(extractCleanValue(line).c_str());
		if (line.find(DIRECTIVE_CGI) != std::string::npos)
			(line.find("on") != std::string::npos) ? location.cgi = true : location.cgi = false;
		if (line.find(DIRECTIVE_REDIR) != std::string::npos) {
			location.file = extractCleanValue(line);
			location.redir = true;
		}
		if (line.find(DIRECTIVE_HANDLE_POST) != std::string::npos)
			location.handlePOST = extractCleanValue(line);
		if (line.find(DIRECTIVE_HANDLE_DELETE) != std::string::npos)
			location.handleDELETE = extractCleanValue(line);
	}
}

// Returns all servers locations saved in a Location struct,
// divided by server (first vector index) and location path (second vector index).
std::vector< std::vector<Location> >	getAllServerLocs(const std::string & configFile) {

	std::string line;
	std::ifstream inFile(configFile);
	if (!inFile.is_open())
	{
		error("Failed to open configuration file");
		exit(EXIT_FAILURE);
	}

	std::vector< std::vector<Location> > allServerLocs;
	std::vector<Location> serverLocs;
	Location general;
	setLocation(inFile, line, general);
	while (line.find("location:") != std::string::npos || getline(inFile, line)) {
		while (line.find("location:") != std::string::npos || getline(inFile, line)) {
			if (line.find("server:") != std::string::npos) break;
			if (line.find("location:") == std::string::npos) continue;
			else {
				Location location(general);
				setLocation(inFile, line, location);
				serverLocs.push_back(location);
			}
		}
		if (line.find("server:") != std::string::npos) {
			general = Location();
			setLocation(inFile, line, general);
		}
		allServerLocs.push_back(serverLocs);
		serverLocs.clear();
	}
	inFile.close();
	return (allServerLocs);
}

// Creates the config object step by step:
//   - Saves number of ports
//   - Gets all listen points and saves them in corresponding vector position
//   - Gets and saves all individual location directive values
Config::Config(const std::string & configFile)
{
	std::cout << "Loading " << configFile << "..." << std::endl << std::endl;

	_maxPorts = getNumberOfPorts(configFile);
	std::vector< std::vector< std::pair<std::string, std::string> > > listenPoints = getListenPoints(configFile);
	for (size_t j = 0; j < listenPoints.size(); j++) {
		for (size_t i = 0; i < listenPoints[j].size(); i++) {
			_port.push_back(std::atoi(listenPoints[j][i].second.c_str()));
			_address.push_back(listenPoints[j][i].first);
		}
	}

	std::vector< std::vector<Location> > allServerLocs = getAllServerLocs(configFile);
	size_t	nServer = 0;
	for (std::vector< std::vector<Location> >::const_iterator it = allServerLocs.begin(); it < allServerLocs.end(); it++) {
		for (std::vector< std::pair<std::string, std::string> >::const_iterator it2 = listenPoints[nServer].begin(); it2 < listenPoints[nServer].end(); it2++) {
			_autoindex.push_back(createMapBool(*it, "autoindex"));
			_file.push_back(createMapString(*it, "file"));
			_root.push_back(createMapString(*it, "root"));
			_allowedGET.push_back(createMapBool(*it, "allowedGET"));
			_allowedPOST.push_back(createMapBool(*it, "allowedPOST"));
			_allowedDELETE.push_back(createMapBool(*it, "allowedDELETE"));
			_cgi.push_back(createMapBool(*it, "cgi"));
			_redir.push_back(createMapBool(*it, "redir"));
			_errorPage.push_back(createMapMap(*it, "errorPage"));
			_bufferSize.push_back(createMapInt(*it, "bufferSize"));
			_handlePOST.push_back(createMapString(*it, "handlePOST"));
			_handleDELETE.push_back(createMapString(*it, "handleDELETE"));
		}
		nServer++;
	}
	debug("Config Object Created");
}

Config::~Config(void) {
	debug("Config Object Destroyed");
}

bool						Config::isAutoIndex(size_t i, std::string req) { return (this->_autoindex[i][req]); }

bool						Config::isRedir(size_t i, std::string req) { return (this->_redir[i][req]); }

bool						Config::isGET(size_t i, std::string req) { return (this->_allowedGET[i][req]); }

bool						Config::isPOST(size_t i, std::string req) { return (this->_allowedPOST[i][req]); }

bool						Config::isDELETE(size_t i, std::string req) { return (this->_allowedDELETE[i][req]); }

bool						Config::isCgi(size_t i, std::string req) { return (this->_cgi[i][req]); }

size_t						Config::getMaxPorts(void) { return (_maxPorts); }

std::string					Config::getServerName(size_t i) { return (_servername[i]); }

int							Config::getPort(size_t i) { return (_port[i]); }

std::string					Config::getAddress(size_t i) { return (_address[i]); }

std::string					Config::getFile(size_t i, std::string req) { return (_file[i][req]); }

std::map<int, std::string>	Config::getErrorPage(size_t i, std::string req) { return (_errorPage[i][req]); }

int							Config::getBufferSize(size_t i, std::string req) { return (_bufferSize[i][req]); }

std::string					Config::getHandlePOST(size_t i, std::string req) { return (this->_handlePOST[i][req]); }

std::string					Config::getHandleDELETE(size_t i, std::string req) { return (this->_handleDELETE[i][req]); }

void	Config::printConfig(void) {
	debug("Printing config...");
	size_t	i = 0;
	std::cout << GREY << std::endl << "------ CONFIG FILE INPUT (DEBUG) ------" << std::endl;
	while (i < _maxPorts)
	{
		std::cout << std::endl << "  LISTENING LOCATION " << i << ":" << std::endl;
		//std::cout << "    Server name :  " << _servername[i] << std::endl;
		std::cout << "    Listening on : " << _address[i] << ":" << _port[i] << std::endl;

		std::map<std::string, std::string>& tempMap = _file[i];
		std::map<std::string, std::string>::iterator it = tempMap.begin();
		(void)it;
		while ( it != tempMap.end())
		{
			std::cout << "    Request to route " << it->first << " :" << std::endl;
			std::cout << "      autoindex : " << (_autoindex[i][it->first] ? " on" : " off") << std::endl;
			std::cout << "      file :       " << _file[i][it->first] << std::endl;
			std::cout << "      root :       " << _root[i][it->first] << std::endl;
			std::cout << "      allow :      ";
			if (_allowedGET[i][it->first]) std::cout << "GET ";
			if (_allowedPOST[i][it->first]) std::cout << "POST ";
			if (_allowedDELETE[i][it->first]) std::cout << "DELETE ";
			std::cout << std::endl;
			std::cout << "      errorPage :\n";
			for (std::map<int, std::string>::iterator ite = _errorPage[i][it->first].begin(); ite != _errorPage[i][it->first].end(); ite++)
				std::cout << "        - " << ite->first << " => (" << ite->second << ")" << std::endl;
			std::cout << "      bufferSize : " << _bufferSize[i][it->first] << std::endl;
			std::cout << "      cgi :        " << (_cgi[i][it->first] ? " on" : " off") << std::endl;
			std::cout << "      redir :      " << (_redir[i][it->first] ? " on" : " off") << std::endl;
			std::cout << "      handlePOST :   " << _handlePOST[i][it->first] << std::endl;
			std::cout << "      handleDELETE : " << _handleDELETE[i][it->first] << std::endl;
			it++;
		}
		i++;
	}
	std::cout << DEF_COL << std::endl;
}


bool	Config::isValidRequest(size_t i, std::string req) {
	std::map < std::string, std::string >::iterator it;

	it = this->_file[i].find(req);
	if (it != this->_file[i].end())
		return (true);
	return (false);
}


