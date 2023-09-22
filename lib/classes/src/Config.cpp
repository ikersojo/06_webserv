/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:33:58 by isojo-go          #+#    #+#             */
/*   Updated: 2023/09/22 09:48:32 by isojo-go         ###   ########.fr       */
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
