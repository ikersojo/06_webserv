/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:17:18 by isojo-go          #+#    #+#             */
/*   Updated: 2023/09/26 23:10:30 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
	#define CONFIG_HPP

	#include <iostream>
	#include <string>
	#include <vector>
	#include <fstream>
	#include <sstream>


	#include "../../aux/inc/aux.hpp"

	class Config
	{
		public:
			Config(const std::string & configFile);
			~Config(void);

			size_t						getMaxPorts(void);
			std::vector < int >			getPorts(void);
			std::vector < std::string >	getDirs(void);
			std::vector < std::string >	getFiles(void);

		private:
			Config(void);

			int							_configFileFD;
			size_t						_maxPorts;
			std::vector < int >			_ports;

			std::vector < std::string >	_responseDirs;
			std::vector < std::string >	_responseFiles;
			std::vector < std::string >	_responseFullPathFiles;
	};

#endif // CONFIG_HPP
