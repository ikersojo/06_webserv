/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 15:54:15 by isojo-go          #+#    #+#             */
/*   Updated: 2023/09/20 22:20:17 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
	#define WEBSERV_HPP

	// KEY macros:
	#define EXT		".conf"

	// Debug option on/off: check Makefile
	#ifndef DEBUG
		#define DEBUG 0
	#endif

	// Main includes:
	#include <iostream>
	#include <string>

	#include "../lib/aux/inc/aux.hpp"

	// Custom Class Includes:
	#include "../lib/classes/inc/Server.hpp"
	#include "../lib/classes/inc/Connection.hpp"
	#include "../lib/classes/inc/Config.hpp"

	// --- MAIN FUNCTIONS --- //

		// 01_syntaxCorrect.cpp
		bool syntaxCorrect(int argc, const char** argv);

		// 02_confFileCorrect.cpp 
		bool confFileCorrect(const char **argv);


	// --- AUX FUNCTIONS --- //

		// 99_printLogo.cpp
		void	printLogo(void);

#endif // WEBSERV_HPP
