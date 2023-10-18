#ifndef AUX_HPP
	#define AUX_HPP

	#include <iostream>
	#include <string>
	#include <sstream>
	#include <ctime>
	#include <fcntl.h>
	#include <sys/socket.h>
	#include <arpa/inet.h>
	#include <unistd.h>
	#include <vector>
	#include <termios.h>
	#include <unistd.h>

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
	void		debugPrintFd(const fd_set * tmpRecvSet, const fd_set * tmpSendSet, int maxSocket);
	void		printListening(int * listenIndex, int * listenWrittenSize);

	// intToString.cpp
	std::string	intToString(int n);
	std::string	trimChars(const std::string& str, const std::string& chars);

	// now.cpp
	std::string	now(void);

	// locationInSocketVector.cpp
	int			locationInSocketVector(int fd, std::vector < int > socketList);

	// echo.cpp
	void	echoOff(void);
	void	echoOn(void);

	// vectorToJson.cpp
	std::string	vectorToJson(std::vector < std::string > vec);

	// countOccurrences.cpp
	size_t countOccurrences(const std::string& str, char ch);

#endif // AUX_HPP
