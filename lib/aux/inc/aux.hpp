#ifndef AUX_HPP
	#define AUX_HPP
	
	#include <iostream>
	#include <string>
	#include <sstream>

	// Debug option on/off: check Makefile
	#ifndef DEBUG
		#define DEBUG 0
	#endif

	// Colors:
	#define RED		"\033[0;31m"
	#define GREEN	"\033[0;92m"
	#define YELLOW	"\033[0;93m"
	#define GREY	"\033[0;90m"
	#define DEF_COL	"\033[0;39m"

	// printToConsole.cpp
	void		debug(const std::string & str);
	void		error(const std::string & str);
	std::string	intToString(int n);
	std::string	trimDoubleQuotes(const std::string& str);

#endif // AUX_HPP
