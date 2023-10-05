/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:33:58 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/05 22:02:08 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Config.hpp"

size_t	getNumberOfPorts(const std::string & configFile)
{
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
	if (DEBUG == 1)
		std::cout << GREY << "[DEBUG: " << n << " ports to listen to identified]"
					<< DEF_COL << std::endl;
	return (n);
}

void	Config::resizeVectors(size_t size)
{
	this->_servername.resize(size);
	this->_port.resize(size);
	this->_address.resize(size);
	this->_autoindex.resize(size);
	this->_file.resize(size);
	this->_allowedGET.resize(size);
	this->_allowedPOST.resize(size);
	this->_allowedDELETE.resize(size);
	this->_errorPage.resize(size);
	this->_bufferSize.resize(size);
	this->_redir.resize(size);
	this->_cgi.resize(size);
}

Config::Config(const std::string & configFile)
{
	std::cout << "Loading " << configFile << "..." << std::endl << std::endl;

	this->_maxPorts = 1;						// this->_maxPorts = getNumberOfPorts(configFile);
	this->resizeVectors(this->_maxPorts);


	size_t	i = 0;
	while (i < this->_maxPorts)
	{
		this->_servername[i] = "myserver.com";
		this->_port[i] = 61001;
		this->_address[i] = "localhost";

		this->_autoindex[i]["/"] = false;
		this->_redir[i]["/"] = false;
		this->_cgi[i]["/"] = false;
		this->_file[i]["/"] = "./www/site1/index.html";
		this->_allowedGET[i]["/"] = true;
		this->_allowedPOST[i]["/"] = true;
		this->_allowedDELETE[i]["/"] = true;
		this->_errorPage[i]["/"] = "./www/def404.html";
		this->_bufferSize[i]["/"] = 4096;

		this->_autoindex[i]["/favicon.ico"] = false;
		this->_redir[i]["/favicon.ico"] = false;
		this->_cgi[i]["/favicon.ico"] = false;
		this->_file[i]["/favicon.ico"] = "./www/site1/icon1.png";
		this->_allowedGET[i]["/favicon.ico"] = true;
		this->_allowedPOST[i]["/favicon.ico"] = true;
		this->_allowedDELETE[i]["/favicon.ico"] = true;
		this->_errorPage[i]["/favicon.ico"] = "./www/def404.html";
		this->_bufferSize[i]["/favicon.ico"] = 4096;

		this->_autoindex[i]["/images/sample"] = false;
		this->_redir[i]["/images/sample"] = false;
		this->_cgi[i]["/images/sample"] = false;
		this->_file[i]["/images/sample"] = "./www/site1/images/sample_image1.jpg";
		this->_allowedGET[i]["/images/sample"] = true;
		this->_allowedPOST[i]["/images/sample"] = true;
		this->_allowedDELETE[i]["/images/sample"] = true;
		this->_errorPage[i]["/images/sample"] = "./www/def404.html";
		this->_bufferSize[i]["/images/sample"] = 4096;

		this->_autoindex[i]["/images"] = true;
		this->_redir[i]["/images"] = false;
		this->_cgi[i]["/images"] = false;
		this->_file[i]["/images"] = "./www/site1/images/";
		this->_allowedGET[i]["/images"] = true;
		this->_allowedPOST[i]["/images"] = true;
		this->_allowedDELETE[i]["/images"] = true;
		this->_errorPage[i]["/images"] = "./www/def404.html";
		this->_bufferSize[i]["/images"] = 4096;

		this->_autoindex[i]["/redir"] = false;
		this->_redir[i]["/redir"] = true;
		this->_cgi[i]["/redir"] = false;
		this->_file[i]["/redir"] = "https://www.google.com";
		this->_allowedGET[i]["/redir"] = true;
		this->_allowedPOST[i]["/redir"] = true;
		this->_allowedDELETE[i]["/redir"] = true;
		this->_errorPage[i]["/redir"] = "./www/def404.html";
		this->_bufferSize[i]["/redir"] = 4096;

		i++;
	}
	debug("Config Object Created");
}


Config::~Config(void)
{
	debug("Config Object Destroyed");
}


size_t	Config::getMaxPorts(void)
{
	return (this->_maxPorts);
}


int		Config::getPort(size_t i)
{
	return (this->_port[i]);
}


std::string		Config::getAddress(size_t i)
{
	return (this->_address[i]);
}


std::string		Config::getFile(size_t i, std::string req)
{
	return (this->_file[i][req]);
}

bool	Config::isAutoIndex(size_t i, std::string req)
{
	return (this->_autoindex[i][req]);
}

bool	Config::isRedir(size_t i, std::string req)
{
	return (this->_redir[i][req]);
}

bool	Config::isGET(size_t i, std::string req)
{
	return (this->_allowedGET[i][req]);
}

bool	Config::isPOST(size_t i, std::string req)
{
	return (this->_allowedPOST[i][req]);
}

bool	Config::isDELETE(size_t i, std::string req)
{
	return (this->_allowedDELETE[i][req]);
}

bool	Config::isCgi(size_t i, std::string req)
{
	return (this->_cgi[i][req]);
}

void	Config::printConfig(void)
{
	debug("Printing config...");
	size_t	i = 0;
	std::cout << GREY << std::endl << "------ CONFIG FILE INPUT (DEBUG) ------" << std::endl;
	while (i < this->_maxPorts)
	{
		std::cout << std::endl << "  LISTENING LOCATION " << i << ":" << std::endl;
		std::cout << "    http://" << this->_address[i] << ":" << this->_port[i] << std::endl;

		std::map<std::string, std::string>& currentMap = this->_file[i];
		std::map<std::string, std::string>::iterator it = currentMap.begin();
		while ( it != currentMap.end())
		{
			std::cout << "    Requests to " << it->first;

			if (this->isGET(i, it->first))
				std::cout << " (G";
			else
				std::cout << "(-";
			if (this->isPOST(i, it->first))
				std::cout << "P";
			else
				std::cout << "-";
			if (this->isDELETE(i, it->first))
				std::cout << "D)";
			else
				std::cout << "-)";

			std::cout << " will be replied by ";

			if (this->isRedir(i, it->first))
				std::cout << "a redirection to ";
			if (this->isCgi(i, it->first))
				std::cout << "a cgi script run of file ";
			std::cout << it->second << std::endl;
			it++;
		}
		i++;
	}
	std::cout << DEF_COL << std::endl;
}
















// Config::Config(const std::string & configFile)
// {
// 	std::cout << "Loading " << configFile << "..." << std::endl << std::endl;

// 	std::string line, item1, item2, item3, rootDir, path, fullPath;
// 	int	nPortsPerServer;
// 	int	nPortsPopulated = 0;
// 	std::ifstream inFile(configFile);
// 	if (!inFile.is_open())
// 	{
// 		error("Failed to open configuration file");
// 		exit(EXIT_FAILURE);
// 	}

// 	this->_maxPorts = getNumberOfPorts(configFile);
// 	this->_files.resize(this->_maxPorts);

// 	while (getline(inFile, line))
// 	{
// 		std::istringstream iss(line);
// 		if (iss >> item1 && item1 == "server:")
// 		{

// 			while (getline(inFile, line))
// 			{
// 				if (line.find("listen:") != std::string::npos)
// 				{
// 					nPortsPerServer = 0;
// 					while (getline(inFile, line) && 
// 							((line.find("port:") != std::string::npos) || (line.find("address:") != std::string::npos)))
// 					{
// 						std::istringstream issPort(line);
// 						issPort >> item1 >> item2 >> item3;
// 						if (item1 == "-" && item2 == "port:")
// 						{
// 							// std::cout << "PORT: " << item3 << std::endl; // do things with port
// 							nPortsPerServer++;
// 							this->_ports.push_back(atoi(item3.c_str()));

// 							if (getline(inFile, line) && line.find("address:") != std::string::npos)
// 							{
// 								std::istringstream issAddress(line);
// 								issAddress >> item2 >> item3;
// 								// std::cout << "ADDRESS: " << item3 << std::endl; // do things with address
// 								this->_addresses.push_back(trimDoubleQuotes(item3));
// 							}
// 						}
// 					}
// 				}
// 				if (line.find("root_directory:") != std::string::npos)
// 				{
// 					std::istringstream issRootDir(line);
// 					issRootDir >> item1 >> item2;
// 					rootDir = trimDoubleQuotes(item2);
// 					rootDir+= "/";
// 					// std::cout << "ROOT DIR: " << rootDir << std::endl; // do things with root dir
// 				}
// 				if (line.find("locations:") != std::string::npos)
// 				{
// 					while (getline(inFile, line) && 
// 							((line.find("path:") != std::string::npos) || (line.find("file:") != std::string::npos)))
// 					{
// 						std::istringstream issPath(line);
// 						issPath >> item1 >> item2 >> item3;
// 						if (item1 == "-" && item2 == "path:")
// 						{
// 							path = trimDoubleQuotes(item3);
// 							// std::cout << "PATH: " << path << std::endl; // do things with path

// 							if (getline(inFile, line) && line.find("file:") != std::string::npos)
// 							{
// 								std::istringstream issFile(line);
// 								issFile >> item2 >> item3;
// 								fullPath = rootDir;
// 								fullPath+= trimDoubleQuotes(item3);
// 								// std::cout << "FILE: " << fullPath << std::endl; // do things with file
// 							}
// 						}
// 						int	i = -1;
// 						while (++i < nPortsPerServer)
// 							this->_files[nPortsPopulated + i].insert(std::make_pair(path, fullPath));
// 					}
// 					nPortsPopulated += nPortsPerServer;
// 				}
// 			}
// 		}
// 	}
// 	inFile.close();
// 	debug("Config Object Created");
// }
