/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarrien- <aarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:33:58 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/25 15:21:29 by aarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Config.hpp"

// Returns the amount of listen (actual ports) directives found in the config file
size_t	getNumberOfPorts(const std::string & configFile) {
	std::ifstream	inFile(configFile);
	std::string		line;
	size_t			n;

	if (!inFile.is_open()) {
		error("Failed to open configuration file");
		exit(EXIT_FAILURE);
	}
	n = 0;
	while (getline(inFile, line)) {
		if (line.find(D_LISTEN) != std::string::npos)
			n++;
	}
	inFile.close();
	debug("ports to listen to identified");
	return (n);
}

std::vector<std::string>	getServerNames(const std::string & configFile) {
	std::ifstream	inFile(configFile);
	std::string		line, serverName;
	std::vector<std::string> serverNames;

	if (!inFile.is_open()) {
		error("Failed to open configuration file");
		exit(EXIT_FAILURE);
	}
	while (getline(inFile, line)) {
		if (line.find(D_SERVER) != std::string::npos)
			serverName.clear();
		if (line.find(D_SERVER_NAME) != std::string::npos)
			serverName = EXTRACT_CLEAN_VALUE(line);
		if (line.find(D_LISTEN) != std::string::npos)
			serverNames.push_back(serverName);
	}
	inFile.close();
	debug("server names identified");
	return (serverNames);
}

void    Config::setAIFile(size_t i, std::string url, std::string dir, std::string file)
{
	this->_autoindex[i][url] = false;
	this->_redir[i][url] = false;
	this->_cgi[i][url] = false;
	this->_root[i][url] = dir;
	this->_file[i][url] = file;
	this->_allowedGET[i][url] = true;
	this->_allowedPOST[i][url] = false;
	this->_allowedDELETE[i][url] = false;
	//this->_errorPage[i][url] = “./www/def404.html”;
	this->_bufferSize[i][url] = 4096;
}
void    Config::setAIDir(size_t i, std::string url, std::string dir)
{
	this->_autoindex[i][url] = true;
	this->_redir[i][url] = false;
	this->_cgi[i][url] = false;
	this->_root[i][url] = dir;
	this->_file[i][url] = dir;
	this->_allowedGET[i][url] = true;
	this->_allowedPOST[i][url] = false;
	this->_allowedDELETE[i][url] = false;
	//this->_errorPage[i][url] = “./www/def404.html”;
	this->_bufferSize[i][url] = 4096;
}
void    Config::setDeletePath(size_t i, std::string url, std::string task)
{
	this->_autoindex[i][url] = false;
	this->_redir[i][url] = false;
	this->_cgi[i][url] = false;
	this->_file[i][url] = task;
	this->_allowedGET[i][url] = false;
	this->_allowedPOST[i][url] = false;
	this->_allowedDELETE[i][url] = true;
	//this->_errorPage[i][url] = “./www/def404.html”;
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
		if (line.find(D_SERVER) != std::string::npos) {
			if (nServer != 0) {
				res.push_back(serv);
				serv.clear();
			}
			nServer++;
		}
		if (line.find(D_LISTEN) != std::string::npos) {
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
		if (directive == D_FILE) Map[allServerLocs[i].path] = allServerLocs[i].file;
		else if (directive == D_ROOT) Map[allServerLocs[i].path] = allServerLocs[i].root;
		else if (directive == D_CGI) Map[allServerLocs[i].path] = allServerLocs[i].cgiExt;
		else if (directive == D_HANDLE_POST) Map[allServerLocs[i].path] = allServerLocs[i].handlePOST;
		else if (directive == D_HANDLE_DELETE) Map[allServerLocs[i].path] = allServerLocs[i].handleDELETE;
	}

	return (Map);
}

std::map<std::string, bool>	createMapBool(std::vector<Location> allServerLocs, std::string directive) {
	std::map<std::string, bool> Map;

	for (size_t i = 0; i < allServerLocs.size(); i++) {
		if (directive == D_AUTO_INDEX) Map[allServerLocs[i].path] = allServerLocs[i].autoindex;
		else if (directive == "allowedGET") Map[allServerLocs[i].path] = allServerLocs[i].allowedGET;
		else if (directive == "allowedPOST") Map[allServerLocs[i].path] = allServerLocs[i].allowedPOST;
		else if (directive == "allowedDELETE") Map[allServerLocs[i].path] = allServerLocs[i].allowedDELETE;
		else if (directive == D_CGI) Map[allServerLocs[i].path] = allServerLocs[i].cgi;
		else if (directive == D_REDIR) Map[allServerLocs[i].path] = allServerLocs[i].redir;
	}

	return (Map);
}

std::map<std::string, int>	createMapInt(std::vector<Location> allServerLocs, std::string directive) {
	std::map<std::string, int> Map;

	for (size_t i = 0; i < allServerLocs.size(); i++) {
		if (directive == D_BUFFER_SIZE) Map[allServerLocs[i].path] = allServerLocs[i].bufferSize;
	}

	return (Map);
}

std::map<std::string, std::map<int, std::string> >	createMapMap(std::vector<Location> allServerLocs, std::string directive) {
	std::map<std::string, std::map<int, std::string> > Map;

	for (size_t i = 0; i < allServerLocs.size(); i++) {
		if (directive == D_ERROR_PAGE) Map[allServerLocs[i].path] = allServerLocs[i].errorPage;
	}

	return (Map);
}

// Modifies an existing Location structure based on the following lines it reads
// until it finds an "location:" directive.
void	setLocation(std::ifstream& inFile, std::string& line, Location& location) {
	location.path = EXTRACT_CLEAN_VALUE(line);
	while (getline(inFile, line) && !line.empty() && line.find(D_LOCATION) == std::string::npos) {
		if (STRING_CONTAINS(line, D_ROOT))
			location.root = EXTRACT_CLEAN_VALUE(line);
		if (STRING_CONTAINS(line, D_FILE)) {
			if (!location.redir)
				location.file = EXTRACT_CLEAN_VALUE(line);
		}
		if (STRING_CONTAINS(line, D_AUTO_INDEX))
			if (STRING_CONTAINS(line, "on")) location.autoindex = true;
		if (STRING_CONTAINS(line, D_ALLOW)) {
			(STRING_CONTAINS(line, "GET")) ?    location.allowedGET = true : location.allowedGET = false;
			(STRING_CONTAINS(line, "POST")) ?   location.allowedPOST = true : location.allowedPOST = false;
			(STRING_CONTAINS(line, "DELETE")) ? location.allowedDELETE = true : location.allowedDELETE = false;
		}
		if (STRING_CONTAINS(line, D_ERROR_PAGE)) {
			line = trimChars(line.substr(line.find(":")+1), " ");
			int code = std::atoi(trimChars(line.substr(0, line.find(" ")), " \"").c_str());
			std::string page = trimChars(line.substr(line.find(" ")+1), " \"");
			location.errorPage[code] = page;
		}
		if (STRING_CONTAINS(line, D_BUFFER_SIZE))
			location.bufferSize = std::atoi(EXTRACT_CLEAN_VALUE(line).c_str());
		if (STRING_CONTAINS(line, D_CGI)) {
			location.cgiExt = EXTRACT_CLEAN_VALUE(line);
			(!location.cgiExt.empty()) ? location.cgi = true : location.cgi = false;
		}
		if (STRING_CONTAINS(line, D_REDIR)) {
			location.file = EXTRACT_CLEAN_VALUE(line);
			location.redir = true;
		}
		if (STRING_CONTAINS(line, D_HANDLE_POST))
			location.handlePOST = EXTRACT_CLEAN_VALUE(line);
		if (STRING_CONTAINS(line, D_HANDLE_DELETE))
			location.handleDELETE = EXTRACT_CLEAN_VALUE(line);
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
	Location general, tmpGeneral;

	setLocation(inFile, line, general);
	serverLocs.push_back((tmpGeneral = general, tmpGeneral.path = GENERAL_LOCATION_ROUTE, tmpGeneral));
	while (line.find(D_LOCATION) != std::string::npos || getline(inFile, line)) {
		while (line.find(D_LOCATION) != std::string::npos || getline(inFile, line)) {
			if (line.find(D_SERVER) != std::string::npos) break;
			if (line.find(D_LOCATION) == std::string::npos) continue;
			else {
				Location location(general);
				setLocation(inFile, line, location);
				serverLocs.push_back(location);
			}
		}
		if (line.find(D_SERVER) != std::string::npos) {
			general = Location();
			setLocation(inFile, line, general);
		}
		allServerLocs.push_back(serverLocs);
		serverLocs.clear();
		serverLocs.push_back((tmpGeneral = general, tmpGeneral.path = GENERAL_LOCATION_ROUTE, tmpGeneral));
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
	_servername = getServerNames(configFile);
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
			_autoindex.push_back(createMapBool(*it, D_AUTO_INDEX));
			_file.push_back(createMapString(*it, D_FILE));
			_root.push_back(createMapString(*it, D_ROOT));
			_allowedGET.push_back(createMapBool(*it, "allowedGET"));
			_allowedPOST.push_back(createMapBool(*it, "allowedPOST"));
			_allowedDELETE.push_back(createMapBool(*it, "allowedDELETE"));
			_cgi.push_back(createMapBool(*it, D_CGI));
			_cgiExt.push_back(createMapString(*it, D_CGI));
			_redir.push_back(createMapBool(*it, D_REDIR));
			_errorPage.push_back(createMapMap(*it, D_ERROR_PAGE));
			_bufferSize.push_back(createMapInt(*it, D_BUFFER_SIZE));
			_handlePOST.push_back(createMapString(*it, D_HANDLE_POST));
			_handleDELETE.push_back(createMapString(*it, D_HANDLE_DELETE));
		}
		nServer++;
	}
	debug("Config Object Created");
}

Config::~Config(void) {
	debug("Config Object Destroyed");
}
size_t						Config::getMaxPorts(void) { return (_maxPorts); }

std::string					Config::getServerName(size_t i) { return (_servername[i]); }

int							Config::getPort(size_t i) { return (_port[i]); }

std::string					Config::getAddress(size_t i) { return (_address[i]); }

std::string					Config::getFile(size_t i, std::string req) {
	if (_file.size() > i && _file[i].find(getNearestLocation(i, req)) != _file[i].end())
		return (_file[i][getNearestLocation(i, req)]);
	return (std::string());
}

std::string					Config::getRoot(size_t i, std::string req) { return (_root[i][getNearestLocation(i, req)]); }

bool						Config::isAutoIndex(size_t i, std::string req) { return (this->_autoindex[i][getNearestLocation(i, req)]); }

bool						Config::isGET(size_t i, std::string req) { return (this->_allowedGET[i][getNearestLocation(i, req)]); }

bool						Config::isPOST(size_t i, std::string req) { return (this->_allowedPOST[i][getNearestLocation(i, req)]); }

bool						Config::isDELETE(size_t i, std::string req) { return (this->_allowedDELETE[i][getNearestLocation(i, req)]); }

std::map<int, std::string>	Config::getErrorPage(size_t i, std::string req) {
	//if (_errorPage.size() > i && _errorPage[i].find(getNearestLocation(i, req)) != _errorPage[i].end())
	return (_errorPage[i][getNearestLocation(i, req)]);
	//return (std::map<int, std::string>());
}

int							Config::getBufferSize(size_t i, std::string req) { return (_bufferSize[i][getNearestLocation(i, req)]); }

bool						Config::isRedir(size_t i, std::string req) { return (this->_redir[i][getNearestLocation(i, req)]); }

bool						Config::isCgi(size_t i, std::string req) { return (this->_cgi[i][getNearestLocation(i, req)]); }

std::string					Config::getCgi(size_t i, std::string req) { return (_cgiExt[i][getNearestLocation(i, req)]); }

std::string					Config::getHandlePOST(size_t i, std::string req) { return (this->_handlePOST[i][getNearestLocation(i, req)]); }

std::string					Config::getHandleDELETE(size_t i, std::string req) { return (this->_handleDELETE[i][getNearestLocation(i, req)]); }

std::string					Config::getActualPath(size_t i, std::string req) {
	std::string nl = getNearestLocation(i, req);
	std::string actualFile = req.substr(nl.size());
	std::string file = getFile(i, nl);

	if (actualFile.size() == 0 && file.size() != 0)
		actualFile = "/" + file;
	if (!actualFile.empty() && actualFile[0] != '/')
		actualFile = "/" + actualFile;
	return(getRoot(i, nl) + actualFile);
}

std::string					Config::getFullPath(size_t i, std::string req) {
	std::string	fullPath, root, file;

	root = this->getRoot(i, req);
	if (root[root.size() - 1] == '/')
		root = root.substr(0, root.rfind("/"));
	file = this->getFile(i, req);
	if (file[file[0]] == '.')
		file = file.substr(2, file.size());
	fullPath = root + "/" + file;
	return (fullPath);
}


bool	Config::isValidRequest(size_t i, std::string req) {
	std::map < std::string, std::string >::iterator it;

	it = this->_file[i].find(req);
	if (it != this->_file[i].end())
		return (true);
	return (false);
}

void	Config::printConfig(void) {
	debug("Printing config...");
	size_t	i = 0;
	std::cout << GREY << std::endl << "------ CONFIG FILE INPUT (DEBUG) ------" << std::endl;
	while (i < _maxPorts)
	{
		std::cout << std::endl << "  LISTENING LOCATION " << i << ":" << std::endl;
		std::cout << "    Server name :      " << _servername[i] << std::endl;
		std::cout << "    Listening on :     " << _address[i] << ":" << _port[i] << std::endl;

		std::map<std::string, std::string>& uriMap = _file[i];
		std::map<std::string, std::string>::iterator it = uriMap.begin();
		(void)it;
		while ( it != uriMap.end())
		{
			std::cout << "    Request to route : [" << it->first << "]" << std::endl;
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
			std::cout << "      cgi :        " << (_cgi[i][it->first] ? (" on => " + _cgiExt[i][it->first]) : " off") << std::endl;
			std::cout << "      redir :      " << (_redir[i][it->first] ? " on" : " off") << std::endl;
			std::cout << "      handlePOST :   " << _handlePOST[i][it->first] << std::endl;
			std::cout << "      handleDELETE : " << _handleDELETE[i][it->first] << std::endl;
			it++;
		}
		i++;
	}
	std::cout << DEF_COL << std::endl;
}

std::string	Config::getNearestLocation(size_t i, std::string uri) {
	std::vector<std::string> avaliableLocations;
	bool catchAll = false;
	size_t pos;

	if (_file.size() <= i)
		return ("");
	for (std::map<std::string, std::string>::const_iterator it = _file[i].begin(); it != _file[i].end(); it++) {
		if (it->first == "/")
			catchAll = true;
		avaliableLocations.push_back(it->first);
	}

	if (uri.empty() && catchAll)
		return ("/");
	while (!uri.empty()) {
		for (std::vector<std::string>::iterator it = avaliableLocations.begin(); it != avaliableLocations.end(); it++) {
			if (uri[0] != '/' && uri[uri.size()-1] != '/' && *it == ('/' + uri + '/'))
				return ('/' + uri + '/');
			if (uri[0] != '/' && *it == ('/' + uri))
				return ('/' + uri);
			if (uri[uri.size()-1] != '/' && *it == (uri + '/'))
				return (uri + '/');
			if (*it == uri)
				return (uri);
		}
		pos = uri.rfind('/');
		if (pos != std::string::npos) {
			if (pos == 0 && catchAll)
				return ("/");
			else if (pos == 0 && !catchAll)
				return (GENERAL_LOCATION_ROUTE);
			else
				uri = uri.substr(0, pos);
		} else if (catchAll)
			return ("/");
		else
			uri.clear();
	}
	return (uri);
}
