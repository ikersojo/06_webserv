/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarrien- <aarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:33:58 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/06 14:15:30 by aarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Config.hpp"

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
		if ((line.find("port:") != std::string::npos))
			n++;
	}
	inFile.close();
	debug("ports to listen to identified");
	return (n);
}


void Config::resizeVectors(size_t size) {
	this->_maxPorts = size;
	(this->_autoindex.resize(size));
	this->_file.resize(size);
	this->_allowedGET.resize(size);
	this->_allowedPOST.resize(size);
	this->_allowedDELETE.resize(size);
	this->_errorPage.resize(size);
	this->_bufferSize.resize(size);
}

Location	createLocation(std::ifstream& inFile, std::string& line) {
	Location location = {
		.path = "",
		.autoindex = false,
		.file = "./www/default.html",
		.allowedGET = true,
		.allowedPOST = true,
		.allowedDELETE = true,
		.errorPage = "./www/404.html",
		.bufferSize = 4096,
	};
	if (line.find("location:") != std::string::npos) {
		// configuración específica por location
		location.path = trimChars(line.substr(line.find(":")+1), " \"");
		while (getline(inFile, line) && line.find("location:") == std::string::npos) {
			if (line.find("root:") != std::string::npos)
				location.file = trimChars(line.substr(line.find(":")+1), " \"") + location.path;
			if (line.find("file:") != std::string::npos) {
				location.file += ( "/" + trimChars(line.substr(line.find(":")+1), " \""));
			}
			if (line.find("autoindex:") != std::string::npos)
				if (line.find("on") != std::string::npos) location.autoindex = true;
			if (line.find("allow:") != std::string::npos) {
				if (line.find("GET") == std::string::npos) location.allowedGET = false;
				if (line.find("POST") == std::string::npos) location.allowedPOST = false;
				if (line.find("DELETE") == std::string::npos) location.allowedDELETE = false;
			}
			if (line.find("error_page") != std::string::npos)
				location.errorPage = trimChars(line.substr(line.find(":")+1), " \"");
			if (line.find("buffer_size") != std::string::npos)
				location.bufferSize = std::atoi((trimChars(line.substr(line.find(":")+1), " \"")).c_str());
		}
	}
	return (location);
}

void	printLocations(std::vector<Location>& locations) {
	std::cout << "\nLOCATIONS:\n";
	for (std::vector<Location>::iterator it = locations.begin(); it != locations.end();it++) {
		std::cout << "location " << it->path << ":" << std::endl;
		std::cout << "  file:        " << it->file << std::endl;
		std::cout << "  autoindex:   " << (it->autoindex ? "on" : "off") << std::endl;
		std::cout << "  allow:       ";
		if (it->allowedGET) std::cout << "GET ";
		if (it->allowedPOST) std::cout << "POST ";
		if (it->allowedDELETE) std::cout << "DELETE ";
		std::cout << std::endl;
		std::cout << "  error_page:  " << it->errorPage << std::endl;
		std::cout << "  buffer_size: " << it->bufferSize << std::endl;
	}
}

Config::Config(const std::string & configFile)
{
	std::cout << "Loading " << configFile << "..." << std::endl << std::endl;

	std::string line;
	std::ifstream inFile(configFile);
	if (!inFile.is_open())
	{
		error("Failed to open configuration file");
		exit(EXIT_FAILURE);
	}

	resizeVectors(getNumberOfPorts(configFile));

	std::vector<Location> locations;
	while (line.find("location:") != std::string::npos || getline(inFile, line)) {
		if (line.find("location:") == std::string::npos)
			continue;
		Location location = createLocation(inFile, line);
		locations.push_back(location);
	}
	printLocations(locations);
	inFile.close();
	debug("Config Object Created");
}


Config::~Config(void) {
	debug("Config Object Destroyed");
}


size_t			Config::getMaxPorts(void) { return (this->_maxPorts); }

std::string		Config::getServerName(size_t i) { return (this->_servername[i]); }

int				Config::getPort(size_t i) { return (this->_port[i]); }

std::string		Config::getAddress(size_t i) { return (this->_address[i]); }

bool			Config::getAutoIndex(size_t i, std::string req) { return (this->_autoindex[i][req]); }

std::string		Config::getFile(size_t i, std::string req) { return (this->_file[i][req]); }

bool			Config::getAllowedGET(size_t i, std::string req) { return (this->_allowedGET[i][req]); }

bool			Config::getAllowedPOST(size_t i, std::string req) { return (this->_allowedPOST[i][req]); }

bool			Config::getAllowedDELETE(size_t i, std::string req) { return (this->_allowedDELETE[i][req]); }

std::string		Config::getErrorPage(size_t i, std::string req) { return (this->_errorPage[i][req]); }

int				Config::getBufferSize(size_t i, std::string req) { return (this->_bufferSize[i][req]); }


void	Config::printConfig(void) {
	debug("Printing config...");
	size_t	i = 0;
	std::cout << GREY << std::endl << "------ CONFIG FILE INPUT (DEBUG) ------" << std::endl;
	while (i < this->_maxPorts)
	{
		std::cout << std::endl << "  LISTENING LOCATION " << i << ":" << std::endl;
		//std::cout << "    Server name :  " << this->_servername[i] << std::endl;
		std::cout << "    Listening on : " << this->_address[i] << ":" << this->_port[i] << std::endl;

		std::map<std::string, std::string>& tempMap = this->_file[i];
		std::map<std::string, std::string>::iterator it = tempMap.begin();
		while ( it != tempMap.end())
		{
			std::cout << "    Request to route " << it->first << " :" << std::endl;
			//std::cout << "      autoindex : " << _autoindex[i][it->first] << std::endl;
			std::cout << "      file :      " << _file[i][it->first] << std::endl;
			it++;
		}
		i++;
	}
	std::cout << DEF_COL << std::endl;
}



/*for (std::vector<Location>::iterator it = locations.begin(); it != locations.end();i++) {
	std::cout << "LOCATION (" << &it << ")\n";
	std::cout << "location " << it->path << ":" << std::endl;
	std::cout << "  file:        " << it->file << std::endl;
	std::cout << "  autoindex:   " << it->autoindex << std::endl;
	std::cout << "  allow:       ";
	if (it->allowedGET) std::cout << "GET ";
	if (it->allowedPOST) std::cout << "POST ";
	if (it->allowedDELETE) std::cout << "DELETE ";
	std::cout << std::endl;
	std::cout << "  error_page:  " << it->errorPage << std::endl;
	std::cout << "  buffer_size: " << it->bufferSize << std::endl;
} */
