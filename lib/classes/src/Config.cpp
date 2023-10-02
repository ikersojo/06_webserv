/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:33:58 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/02 08:51:34 by isojo-go         ###   ########.fr       */
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


Config::Config(const std::string & configFile)
{
	std::cout << "Loading " << configFile << "..." << std::endl << std::endl;

	std::string line, item1, item2, item3, rootDir, path, fullPath;
	int	nPortsPerServer;
	int	nPortsPopulated = 0;
	std::ifstream inFile(configFile);
	if (!inFile.is_open())
	{
		error("Failed to open configuration file");
		exit(EXIT_FAILURE);
	}

	this->_maxPorts = getNumberOfPorts(configFile);
	this->_files.resize(this->_maxPorts);

	while (getline(inFile, line))
	{
		std::istringstream iss(line);
		if (iss >> item1 && item1 == "server:")
		{

			while (getline(inFile, line))
			{
				if (line.find("listen:") != std::string::npos)
				{
					nPortsPerServer = 0;
					while (getline(inFile, line) && 
							((line.find("port:") != std::string::npos) || (line.find("address:") != std::string::npos)))
					{
						std::istringstream issPort(line);
						issPort >> item1 >> item2 >> item3;
						if (item1 == "-" && item2 == "port:")
						{
							// std::cout << "PORT: " << item3 << std::endl; // do things with port
							nPortsPerServer++;
							this->_ports.push_back(atoi(item3.c_str()));

							if (getline(inFile, line) && line.find("address:") != std::string::npos)
							{
								std::istringstream issAddress(line);
								issAddress >> item2 >> item3;
								// std::cout << "ADDRESS: " << item3 << std::endl; // do things with address
								this->_addresses.push_back(trimDoubleQuotes(item3));
							}
						}
					}
				}
				if (line.find("root_directory:") != std::string::npos)
				{
					std::istringstream issRootDir(line);
					issRootDir >> item1 >> item2;
					rootDir = trimDoubleQuotes(item2);
					rootDir+= "/";
					// std::cout << "ROOT DIR: " << rootDir << std::endl; // do things with root dir
				}
				if (line.find("locations:") != std::string::npos)
				{
					while (getline(inFile, line) && 
							((line.find("path:") != std::string::npos) || (line.find("file:") != std::string::npos)))
					{
						std::istringstream issPath(line);
						issPath >> item1 >> item2 >> item3;
						if (item1 == "-" && item2 == "path:")
						{
							path = trimDoubleQuotes(item3);
							// std::cout << "PATH: " << path << std::endl; // do things with path

							if (getline(inFile, line) && line.find("file:") != std::string::npos)
							{
								std::istringstream issFile(line);
								issFile >> item2 >> item3;
								fullPath = rootDir;
								fullPath+= trimDoubleQuotes(item3);
								// std::cout << "FILE: " << fullPath << std::endl; // do things with file
							}
						}
						int	i = -1;
						while (++i < nPortsPerServer)
							this->_files[nPortsPopulated + i].insert(std::make_pair(path, fullPath));
					}
					nPortsPopulated += nPortsPerServer;
				}
			}
		}
	}
	inFile.close();
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
	return (this->_ports[i]);
}


std::string		Config::getAddress(size_t i)
{
	return (this->_addresses[i]);
}


std::string		Config::getFile(size_t i, std::string req)
{
	return (this->_files[i][req]);
}


void	Config::printConfig(void)
{
	debug("Printing config...");
	size_t	i = 0;
	std::cout << GREY << std::endl << "------ CONFIG FILE INPUT (DEBUG) ------" << std::endl;
	while (i < this->_maxPorts)
	{
		std::cout << std::endl << "  LISTENING LOCATION " << i << ":" << std::endl;
		std::cout << "    http://" << this->_addresses[i] << ":" << this->_ports[i] << std::endl;

		std::map<std::string, std::string>& currentMap = this->_files[i];
		std::map<std::string, std::string>::iterator it = currentMap.begin();
		while ( it != currentMap.end())
		{
			std::cout << "    Requests to " << it->first << " will be replied by " << it->second << std::endl;
			it++;
		}
		i++;
	}
	std::cout << DEF_COL << std::endl;
}
