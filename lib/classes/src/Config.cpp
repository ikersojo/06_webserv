/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarrien- <aarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:33:58 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/10 16:26:42 by aarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Config.hpp"

std::string	extractCleanValue(std::string & line) {
	return (trimChars(line.substr(line.find(":")+1), " \""));
}

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

std::vector< std::vector< std::pair<std::string, std::string> > >	getListenPoints(const std::string & configFile) {
	std::ifstream														inFile(configFile);
	std::vector< std::vector< std::pair<std::string, std::string> > >	res;
	std::vector< std::pair<std::string, std::string> >					serv;
	std::string															line;
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
			std::string address = trimChars(line.substr(line.find(":")+1, line.rfind(":")-line.find(":")-1), " ");
			std::string port = trimChars(line.substr(line.rfind(":")+1), " ");
			serv.push_back(make_pair(address, port));
		}
	}
	res.push_back(serv);
	inFile.close();
	return (res);
}

Location	createLocation(std::ifstream& inFile, std::string& line) {
	Location location;

	if (line.find("location:") != std::string::npos) {
		location.path = extractCleanValue(line);
		while (getline(inFile, line) && !line.empty() && line.find("location:") == std::string::npos) {
			if (line.find("root:") != std::string::npos)
				location.root = extractCleanValue(line);
			if (line.find("file:") != std::string::npos)
				location.file = extractCleanValue(line);
			if (line.find("autoindex:") != std::string::npos)
				if (line.find("on") != std::string::npos) location.autoindex = true;
			if (line.find("allow:") != std::string::npos) {
				if (line.find("GET") == std::string::npos) location.allowedGET = false;
				if (line.find("POST") == std::string::npos) location.allowedPOST = false;
				if (line.find("DELETE") == std::string::npos) location.allowedDELETE = false;
			}
			if (line.find("error_page") != std::string::npos)
				location.errorPage = extractCleanValue(line);
			if (line.find("buffer_size") != std::string::npos)
				location.bufferSize = std::atoi(extractCleanValue(line).c_str());
		}
	}
	return (location);
}

void	printAllServerLocs(std::vector< std::vector<Location> >& allServerLocs) {
	size_t	nServer = 0;

	std::cout << "\n-- ALL SERVER LOCATIONS --\n";
	for (std::vector< std::vector<Location> >::const_iterator it1 = allServerLocs.begin(); it1 != allServerLocs.end(); it1++) {
		std::cout << (nServer+1) << "º SERVER ---------------\n";
		nServer++;
		for (std::vector<Location>::const_iterator it2 = it1->begin(); it2 != it1->end(); it2++) {
			std::cout << "    location " << it2->path << ":" << std::endl;
			std::cout << "      file:        " << it2->file << std::endl;
			std::cout << "      root:        " << it2->root << std::endl;
			std::cout << "      autoindex:   " << (it2->autoindex ? "on" : "off") << std::endl;
			std::cout << "      allow:       ";
			if (it2->allowedGET) std::cout << "GET ";
			if (it2->allowedPOST) std::cout << "POST ";
			if (it2->allowedDELETE) std::cout << "DELETE ";
			std::cout << std::endl;
			std::cout << "      error_page:  " << it2->errorPage << std::endl;
			std::cout << "      buffer_size: " << it2->bufferSize << std::endl;
		}
	}
	std::cout << "\n------------------------\n";
}

std::map<std::string, std::string> createMapString(std::vector<Location> allServerLocs, std::string directive) {
	std::map<std::string, std::string> Map;

	for (size_t i = 0; i < allServerLocs.size(); i++) {
		if (directive == "file") Map[allServerLocs[i].path] = allServerLocs[i].file;
		else if (directive == "errorPage") Map[allServerLocs[i].path] = allServerLocs[i].errorPage;
		else if (directive == "root") Map[allServerLocs[i].path] = allServerLocs[i].root;
	}

	return (Map);
}

std::map<std::string, bool> createMapBool(std::vector<Location> allServerLocs, std::string directive) {
	std::map<std::string, bool> Map;

	for (size_t i = 0; i < allServerLocs.size(); i++) {
		if (directive == "autoindex") Map[allServerLocs[i].path] = allServerLocs[i].autoindex;
		else if (directive == "allowedGET") Map[allServerLocs[i].path] = allServerLocs[i].allowedGET;
		else if (directive == "allowedPOST") Map[allServerLocs[i].path] = allServerLocs[i].allowedPOST;
		else if (directive == "allowedDELETE") Map[allServerLocs[i].path] = allServerLocs[i].allowedDELETE;
	}

	return (Map);
}

std::map<std::string, int> createMapInt(std::vector<Location> allServerLocs, std::string directive) {
	std::map<std::string, int> Map;

	for (size_t i = 0; i < allServerLocs.size(); i++) {
		if (directive == "bufferSize") Map[allServerLocs[i].path] = allServerLocs[i].bufferSize;
	}

	return (Map);
}

std::vector< std::vector<Location> >	getAllServerLocs(const std::string & configFile) {

	std::string line;
	std::ifstream inFile(configFile);
	if (!inFile.is_open())
	{
		error("Failed to open configuration file");
		exit(EXIT_FAILURE);
	}

	std::vector< std::vector<Location> > allServerLocs; // allServerLocs[server][location]
	std::vector<Location> serverLocs;
	while (getline(inFile, line)) {
		while (line.find("location:") != std::string::npos || getline(inFile, line)) {
			if (line.find("server:") != std::string::npos) break;
			if (line.find("location:") == std::string::npos) continue;
			Location location = createLocation(inFile, line);
			serverLocs.push_back(location);
		}
		allServerLocs.push_back(serverLocs);
		serverLocs.clear();

	}
	inFile.close();
	return (allServerLocs);
}

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

	printAllServerLocs(allServerLocs);
	size_t	nServer = 0;
	for (std::vector< std::vector<Location> >::const_iterator it = allServerLocs.begin(); it < allServerLocs.end(); it++) {
		for (std::vector< std::pair<std::string, std::string> >::const_iterator it2 = listenPoints[nServer].begin(); it2 < listenPoints[nServer].end(); it2++) {
			_autoindex.push_back(createMapBool(*it, "autoindex"));
			_file.push_back(createMapString(*it, "file"));
			_root.push_back(createMapString(*it, "root"));
			_allowedGET.push_back(createMapBool(*it, "allowedGET"));
			_allowedPOST.push_back(createMapBool(*it, "allowedPOST"));
			_allowedDELETE.push_back(createMapBool(*it, "allowedDELETE"));
			_errorPage.push_back(createMapString(*it, "errorPage"));
			_bufferSize.push_back(createMapInt(*it, "bufferSize"));
		}
		nServer++;
	}
	debug("Config Object Created");
}


Config::~Config(void) {
	debug("Config Object Destroyed");
}


size_t			Config::getMaxPorts(void) { return (_maxPorts); }

std::string		Config::getServerName(size_t i) { return (_servername[i]); }

int				Config::getPort(size_t i) { return (_port[i]); }

std::string		Config::getAddress(size_t i) { return (_address[i]); }

bool			Config::getAutoIndex(size_t i, std::string req) { return (_autoindex[i][req]); }

std::string		Config::getFile(size_t i, std::string req) { return (_file[i][req]); }

bool			Config::getAllowedGET(size_t i, std::string req) { return (_allowedGET[i][req]); }

bool			Config::getAllowedPOST(size_t i, std::string req) { return (_allowedPOST[i][req]); }

bool			Config::getAllowedDELETE(size_t i, std::string req) { return (_allowedDELETE[i][req]); }

std::string		Config::getErrorPage(size_t i, std::string req) { return (_errorPage[i][req]); }

int				Config::getBufferSize(size_t i, std::string req) { return (_bufferSize[i][req]); }


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
			std::cout << "      errorPage :  " << _errorPage[i][it->first] << std::endl;
			std::cout << "      bufferSize : " << _bufferSize[i][it->first] << std::endl;
			it++;
		}
		i++;
	}
	std::cout << DEF_COL << std::endl;
}
