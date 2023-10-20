/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarrien- <aarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 15:54:15 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/20 15:20:34 by aarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
	#define WEBSERV_HPP

	// KEY macros:
	#define EXT		".yml"

	// Debug option on/off: check Makefile
	#ifndef DEBUG
		#define DEBUG 0
	#endif

	// Main includes:
	#include <iostream>
	#include <string>
	#include <csignal>

	#include "../lib/aux/inc/aux.hpp"

	// Custom Class Includes:
	#include "../lib/classes/inc/Config.hpp"
	#include "../lib/classes/inc/Server.hpp"
	#include "../lib/classes/inc/ResponseBuilder.hpp"

	// --- MAIN FUNCTIONS --- //

		// 01_syntaxCorrect.cpp
		bool syntaxCorrect(int argc, const char** argv);

		// 02_confFileCorrect.cpp
		bool confFileCorrect(const char **argv);

		// 03_utilsChecksConfig.cpp
		bool Empty(std::string &line);
		int checkAddress(std::string address);
		bool Checkport(std::vector<int> &Allport);
		int SpaceCounter(std::string &line);
		bool CheckLocation(std::ifstream &file, std::string &line);
		bool isOneOf(std::string &line);
		bool isInteger(std::string numb);
		bool isLocationConfigOf(std::string &line);

		// 04_Config_list.cpp
		bool Allow(std::string &line);
		bool Buffersize(std::string &line);
		bool AutoIndex(std::string &line);
		bool ErrorPage(std::string &line);

	// --- AUX FUNCTIONS --- //

		// 99_printLogo.cpp
		void	printLogo(void);

#endif // WEBSERV_HPP
