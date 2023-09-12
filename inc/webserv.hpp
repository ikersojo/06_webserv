/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 15:54:15 by isojo-go          #+#    #+#             */
/*   Updated: 2023/09/12 20:42:07 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
	#define WEBSERV_HPP

	// KEY macros:
	#define EXT		".conf"

	// AUX macros
	#define RED		"\033[0;31m"
	#define GREEN	"\033[0;92m"
	#define YELLOW	"\033[0;93m"
	#define GREY	"\033[0;90m"
	#define DEF_COL	"\033[0;39m"

	// Debug option on/off: check Makefile
	#ifndef DEBUG
		#define DEBUG 0
	#endif

	// Main includes:
	#include <iostream>
	#include <string>

	// Custom Class Includes:
	// #include "../classes/inc/template.hpp"


	// Functions across files:
	// --------------------------------------------------
	
	// 01_syntaxCorrect.cpp
	bool syntaxCorrect(int argc, const char** argv);

	// 99_debug.cpp
	void	debug(const std::string & str);

#endif // WEBSERV_HPP

