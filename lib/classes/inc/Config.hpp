/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:17:18 by isojo-go          #+#    #+#             */
/*   Updated: 2023/09/27 22:39:29 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
	#define CONFIG_HPP

	#include <iostream>
	#include <string>
	#include <vector>
	#include <map>
	#include <fstream>
	#include <sstream>


	#include "../../aux/inc/aux.hpp"

	class Config
	{
		public:
			Config(const std::string & configFile);
			~Config(void);

			size_t			getMaxPorts(void);
			int				getPort(size_t i);
			std::string		getAddress(size_t i);
			std::string		getFile(size_t i, std::string req);
			void			printConfig(void);

		private:
			Config(void);

			size_t							_maxPorts;
			std::vector < int >				_ports;
			std::vector < std::string >		_addresses;
			std::vector < std::map < std::string, std::string > >	_files;
	};

#endif // CONFIG_HPP
