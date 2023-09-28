/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:33:58 by isojo-go          #+#    #+#             */
/*   Updated: 2023/09/28 11:11:20 by isojo-go         ###   ########.fr       */
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
	
	// to be replaced by an actual json/yml parser
		this->_maxPorts = 3;

		this->_ports.push_back(61000);
		this->_ports.push_back(61001);
		this->_ports.push_back(61002);

		this->_addresses.push_back("localhost");
		this->_addresses.push_back("localhost");
		this->_addresses.push_back("localhost");

		this->_files.resize(3);
		this->_files[0].insert(std::make_pair("/", "./www/site1/index.html"));
		this->_files[1].insert(std::make_pair("/", "./www/site2/index.html"));
		this->_files[2].insert(std::make_pair("/", "./www/site3/index.html"));

		this->_files[0].insert(std::make_pair("/favicon.ico", "./www/site1/icon1.png"));
		this->_files[1].insert(std::make_pair("/favicon.ico", "./www/site2/icon2.png"));
		this->_files[2].insert(std::make_pair("/favicon.ico", "./www/site3/icon3.png"));

	// Actual code
	getNumberOfPorts(configFile);

	std::ifstream inFile(configFile);
	std::string line, item1, item2, item3;

	if (!inFile.is_open())
	{
		error("Failed to open configuration file");
		exit(EXIT_FAILURE);
	}

	while (getline(inFile, line))
	{
		std::istringstream iss(line);
		if (iss >> item1 && item1 == "server:")
		{
			while (getline(inFile, line))
			{
				if (line.find("listen:") != std::string::npos)
				{
					while (getline(inFile, line) && 
							((line.find("port:") != std::string::npos) || (line.find("address:") != std::string::npos)))
					{
						std::istringstream issPort(line);
						issPort >> item1 >> item2 >> item3;
						if (item1 == "-" && item2 == "port:")
						{
							std::cout << "PORT: " << item3 << std::endl; // do things with port

							if (getline(inFile, line) && line.find("address:") != std::string::npos)
							{
								std::istringstream issAddress(line);
								issAddress >> item2 >> item3;
								std::cout << "ADDRESS: " << item3 << std::endl; // do things with address
							}
						}
					}
				}
				if (line.find("root_directory:") != std::string::npos)
				{
					std::istringstream issRootDir(line);
					issRootDir >> item1 >> item2;
					std::cout << "ROOT DIR: " << item2 << std::endl; // do things with root dir
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
							std::cout << "PATH: " << item3 << std::endl; // do things with path

							if (getline(inFile, line) && line.find("file:") != std::string::npos)
							{
								std::istringstream issFile(line);
								issFile >> item2 >> item3;
								std::cout << "FILE: " << item3 << std::endl; // do things with file
							}
						}
					}
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
}
