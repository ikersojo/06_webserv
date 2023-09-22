/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:17:18 by isojo-go          #+#    #+#             */
/*   Updated: 2023/09/22 11:34:33 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
	#define CONFIG_HPP

	#include <iostream>
	#include <string>
	#include <vector>

	#include "../../aux/inc/aux.hpp"

	class Config
	{
		public:
			Config(const std::string & configFile);
			~Config(void);

			size_t					getMaxPorts(void);
			std::vector < int >		getPorts(void);

		private:
			Config(void);

			int							_configFileFD;
			size_t						_maxPorts;
			std::vector < int >			_ports;

			std::vector < std::string >	_responseRootDir;
			std::vector < std::string >	_responseFile;
	};

#endif // CONFIG_HPP
