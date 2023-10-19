/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:33:58 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/19 23:16:12 by isojo-go         ###   ########.fr       */
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
	this->_rootDir.resize(size);
	this->_file.resize(size);
	this->_allowedGET.resize(size);
	this->_allowedPOST.resize(size);
	this->_allowedDELETE.resize(size);
	this->_errorPage.resize(size);
	this->_bufferSize.resize(size);
	this->_redir.resize(size);
	this->_cgi.resize(size);
	this->_handlePOST.resize(size);
	this->_handleDELETE.resize(size);
}


void	Config::setAIFile(size_t i, std::string url, std::string path, std::string file)
{
	std::string	fullPath;

	fullPath = path + file;

	this->_autoindex[i][url] = false;
	this->_redir[i][url] = false;
	this->_cgi[i][url] = false;
	this->_rootDir[i][url] = path;
	this->_file[i][url] = fullPath;
	this->_allowedGET[i][url] = true;
	this->_allowedPOST[i][url] = false;
	this->_allowedDELETE[i][url] = false;
	this->_errorPage[i][url] = "./www/def404.html";
	this->_bufferSize[i][url] = 4096;
}


void	Config::setAIDir(size_t i, std::string url, std::string path)
{
	std::cout << "url: " << url << ", path: " << path << std::endl;
	this->_autoindex[i][url] = true;
	this->_redir[i][url] = false;
	this->_cgi[i][url] = false;
	this->_rootDir[i][url] = path;
	this->_file[i][url] = path;
	this->_allowedGET[i][url] = true;
	this->_allowedPOST[i][url] = false;
	this->_allowedDELETE[i][url] = false;
	this->_errorPage[i][url] = "./www/def404.html";
	this->_bufferSize[i][url] = 4096;
}


void	Config::setDeletePath(size_t i, std::string url, std::string task)
{
	this->_autoindex[i][url] = false;
	this->_redir[i][url] = false;
	this->_cgi[i][url] = false;
	this->_file[i][url] = task;
	this->_allowedGET[i][url] = false;
	this->_allowedPOST[i][url] = false;
	this->_allowedDELETE[i][url] = true;
	this->_errorPage[i][url] = "./www/def404.html";
	this->_bufferSize[i][url] = 4096;
	this->_handleDELETE[i][url] = "removeFromList";
}


Config::Config(const std::string & configFile)
{
	std::cout << now() << "  Loading " << configFile << "..." << std::endl;

	this->_maxPorts = 5;						// this->_maxPorts = getNumberOfPorts(configFile);
	this->resizeVectors(this->_maxPorts);


	size_t	i = 0;
		this->_servername[i] = "myserver.com";
		this->_port[i] = 61001;
		this->_address[i] = "localhost";

		this->_autoindex[i]["/"] = false;
		this->_redir[i]["/"] = false;
		this->_cgi[i]["/"] = false;
		this->_file[i]["/"] = "./www/site1/index.html";
		this->_rootDir[i]["/"] = "./www/site1/";
		this->_allowedGET[i]["/"] = true;
		this->_allowedPOST[i]["/"] = false;
		this->_allowedDELETE[i]["/"] = false;
		this->_errorPage[i]["/"] = "./www/def404.html";
		this->_bufferSize[i]["/"] = 4096;
		this->_handlePOST[i]["/"] = "";
		this->_handleDELETE[i]["/"] = "";

		this->_autoindex[i]["/favicon.ico"] = false;
		this->_redir[i]["/favicon.ico"] = false;
		this->_cgi[i]["/favicon.ico"] = false;
		this->_file[i]["/favicon.ico"] = "./www/site1/icon1.png";
		this->_rootDir[i]["/favicon.ico"] = "./www/site1/";
		this->_allowedGET[i]["/favicon.ico"] = true;
		this->_allowedPOST[i]["/favicon.ico"] = false;
		this->_allowedDELETE[i]["/favicon.ico"] = false;
		this->_errorPage[i]["/favicon.ico"] = "./www/def404.html";
		this->_bufferSize[i]["/favicon.ico"] = 4096;
		this->_handlePOST[i]["/favicon.ico"] = "";
		this->_handleDELETE[i]["/favicon.ico"] = "";

		this->_autoindex[i]["/images/sample"] = false;
		this->_redir[i]["/images/sample"] = false;
		this->_cgi[i]["/images/sample"] = false;
		this->_file[i]["/images/sample"] = "./www/site1/images/sample_image1.jpg";
		this->_rootDir[i]["/images/sample"] = "./www/site1/images/";
		this->_allowedGET[i]["/images/sample"] = true;
		this->_allowedPOST[i]["/images/sample"] = false;
		this->_allowedDELETE[i]["/images/sample"] = false;
		this->_errorPage[i]["/images/sample"] = "./www/def404.html";
		this->_bufferSize[i]["/images/sample"] = 4096;
		this->_handlePOST[i]["/images/sample"] = "";
		this->_handleDELETE[i]["/images/sample"] = "";

		this->_autoindex[i]["/images"] = true;
		this->_redir[i]["/images"] = false;
		this->_cgi[i]["/images"] = false;
		this->_file[i]["/images"] = "./www/site1/images/";
		this->_rootDir[i]["/images"] = "./www/site1/images/";
		this->_allowedGET[i]["/images"] = true;
		this->_allowedPOST[i]["/images"] = false;
		this->_allowedDELETE[i]["/images"] = false;
		this->_errorPage[i]["/images"] = "./www/def404.html";
		this->_bufferSize[i]["/images"] = 4096;
		this->_handlePOST[i]["/images"] = "";
		this->_handleDELETE[i]["/images"] = "";

		this->_autoindex[i]["/redirect"] = false;
		this->_redir[i]["/redirect"] = true;
		this->_cgi[i]["/redirect"] = false;
		this->_file[i]["/redirect"] = "https://www.42urduliz.com";
		this->_rootDir[i]["/redirect"] = "/";
		this->_allowedGET[i]["/redirect"] = true;
		this->_allowedPOST[i]["/redirect"] = false;
		this->_allowedDELETE[i]["/redirect"] = false;
		this->_errorPage[i]["/redirect"] = "./www/def404.html";
		this->_bufferSize[i]["/redirect"] = 4096;
		this->_handlePOST[i]["/redirect"] = "";
		this->_handleDELETE[i]["/redirect"] = "";

	i = 1;
		this->_servername[i] = "myserver.com";
		this->_port[i] = 61002;
		this->_address[i] = "localhost";

		this->_autoindex[i]["/"] = false;
		this->_redir[i]["/"] = false;
		this->_cgi[i]["/"] = false;
		this->_file[i]["/"] = "./www/stressTestSite/index.html";
		this->_rootDir[i]["/"] = "./www/stressTestSite/";
		this->_allowedGET[i]["/"] = true;
		this->_allowedPOST[i]["/"] = false;
		this->_allowedDELETE[i]["/"] = false;
		this->_errorPage[i]["/"] = "./www/def404.html";
		this->_bufferSize[i]["/"] = 4096;
		this->_handlePOST[i]["/"] = "";
		this->_handleDELETE[i]["/"] = "";

	i = 2;
		this->_servername[i] = "myserver.com";
		this->_port[i] = 61003;
		this->_address[i] = "localhost";

		this->_autoindex[i]["/"] = false;
		this->_redir[i]["/"] = false;
		this->_cgi[i]["/"] = false;
		this->_file[i]["/"] = "./www/todoSite/index.html";
		this->_rootDir[i]["/"] = "./www/todoSite/index.html/";
		this->_allowedGET[i]["/"] = true;
		this->_allowedPOST[i]["/"] = false;
		this->_handlePOST[i]["/"] = "";
		this->_allowedDELETE[i]["/"] = false;
		this->_handleDELETE[i]["/"] = "";
		this->_errorPage[i]["/"] = "./www/def404.html";
		this->_bufferSize[i]["/"] = 4096;

		this->_autoindex[i]["/api"] = false;
		this->_redir[i]["/api"] = false;
		this->_cgi[i]["/api"] = false;
		this->_file[i]["/api"] = "./www/todoSite/tasks.json";
		this->_rootDir[i]["/api"] = "./www/todoSite/";
		this->_allowedGET[i]["/api"] = true;
		this->_allowedPOST[i]["/api"] = true;
		this->_handlePOST[i]["/api"] = "addToList";
		this->_allowedDELETE[i]["/api"] = false;
		this->_handleDELETE[i]["/api"] = "";
		this->_errorPage[i]["/api"] = "./www/def404.html";
		this->_bufferSize[i]["/api"] = 4096;

	i = 3;
		this->_servername[i] = "myserver.com";
		this->_port[i] = 61004;
		this->_address[i] = "localhost";

		this->_autoindex[i]["/"] = false;
		this->_redir[i]["/"] = false;
		this->_cgi[i]["/"] = false;
		this->_file[i]["/"] = "./www/gallerySite/index.html";
		this->_rootDir[i]["/"] = "./www/gallerySite/";
		this->_allowedGET[i]["/"] = true;
		this->_allowedPOST[i]["/"] = false;
		this->_handlePOST[i]["/"] = "";
		this->_allowedDELETE[i]["/"] = false;
		this->_handleDELETE[i]["/"] = "";
		this->_errorPage[i]["/"] = "./www/def404.html";
		this->_bufferSize[i]["/"] = 4096;

		this->_autoindex[i]["/photos"] = false;
		this->_redir[i]["/photos"] = false;
		this->_cgi[i]["/photos"] = false;
		this->_file[i]["/photos"] = "./www/gallerySite/photos.json";
		this->_rootDir[i]["/photos"] = "./www/gallerySite/";
		this->_allowedGET[i]["/photos"] = true;
		this->_allowedPOST[i]["/photos"] = false;
		this->_handlePOST[i]["/photos"] = "";
		this->_allowedDELETE[i]["/photos"] = false;
		this->_handleDELETE[i]["/photos"] = "";
		this->_errorPage[i]["/photos"] = "./www/def404.html";
		this->_bufferSize[i]["/photos"] = 4096;

		this->_autoindex[i]["/upload"] = false;
		this->_redir[i]["/upload"] = false;
		this->_cgi[i]["/upload"] = false;
		this->_file[i]["/upload"] = "./www/gallerySite/upload.json";
		this->_rootDir[i]["/upload"] = "./www/gallerySite/";
		this->_allowedGET[i]["/upload"] = true;
		this->_allowedPOST[i]["/upload"] = true;
		this->_handlePOST[i]["/upload"] = "addPhoto";
		this->_allowedDELETE[i]["/upload"] = false;
		this->_handleDELETE[i]["/upload"] = "";
		this->_errorPage[i]["/upload"] = "./www/def404.html";
		this->_bufferSize[i]["/upload"] = 4096;


	i = 4;
		this->_servername[i] = "myserver.com";
		this->_port[i] = 61005;
		this->_address[i] = "localhost";

		this->_autoindex[i]["/"] = false;
		this->_redir[i]["/"] = false;
		this->_cgi[i]["/"] = true;
		this->_file[i]["/"] = "/Users/ikersojo/01_PROJECTS/06_webserver/www/cgi-bin/print.out";
		this->_rootDir[i]["/"] = "/Users/ikersojo/01_PROJECTS/06_webserver/www/cgi-bin/";
		this->_allowedGET[i]["/"] = true;
		this->_allowedPOST[i]["/"] = false;
		this->_handlePOST[i]["/"] = "";
		this->_allowedDELETE[i]["/"] = false;
		this->_handleDELETE[i]["/"] = "";
		this->_errorPage[i]["/"] = "./www/def404.html";
		this->_bufferSize[i]["/"] = 4096;

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

std::string		Config::getErrorPage(size_t i, std::string req)
{
	return (this->_errorPage[i][req]);
}

std::string		Config::getFile(size_t i, std::string req)
{
	return (this->_file[i][req]);
}

std::string		Config::getRootDir(size_t i, std::string req)
{
	return (this->_rootDir[i][req]);
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


bool	Config::isValidRequest(size_t i, std::string req)
{
	std::map < std::string, std::string >::iterator it;

	it = this->_file[i].find(req);
	if (it != this->_file[i].end())
		return (true);
	return (false);
}


std::string		Config::getHandlePOST(size_t i, std::string req)
{
	return (this->_handlePOST[i][req]);
}


std::string		Config::getHandleDELETE(size_t i, std::string req)
{
	return (this->_handleDELETE[i][req]);
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
