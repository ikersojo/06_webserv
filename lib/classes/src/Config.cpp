/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:33:58 by isojo-go          #+#    #+#             */
/*   Updated: 2023/09/26 23:22:20 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Config.hpp"

Config::Config(const std::string & configFile)
{
	std::cout << "Loading " << configFile << "..." << std::endl << std::endl;
	
	// to be replaced by an actual json/yml parser
	this->_configFileFD = 5;
	this->_maxPorts = 3;
	this->_ports.push_back(61000);
	this->_ports.push_back(61001);
	this->_ports.push_back(61002);

	this->_responseDirs.push_back("./www/site1");
	this->_responseDirs.push_back("./www/site2");
	this->_responseDirs.push_back("./www/site3");

	this->_responseFiles.push_back("index.html");
	this->_responseFiles.push_back("index.html");
	this->_responseFiles.push_back("index.html");


	// Actual code
	std::ifstream	inFile(configFile);
	std::string		line;

	if (!inFile.is_open())
	{
		error("Failed to open configuration file");
		exit(EXIT_FAILURE);
	}

	while (getline(inFile, line))
	{
		std::cout << "extracted line: " << line << std::endl;
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

std::vector < int >		Config::getPorts(void)
{
	return (this->_ports);
}

std::vector < std::string >		Config::getDirs(void)
{
	return (this->_responseDirs);
}

std::vector < std::string >		Config::getFiles(void)
{
	return (this->_responseFiles);
}
