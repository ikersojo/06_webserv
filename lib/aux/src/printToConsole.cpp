/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printToConsole.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 19:51:59 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/13 16:26:18 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/aux.hpp"

void	debug(const std::string & str)
{
	if (DEBUG == 1)
		std::cout << GREY << "[DEBUG: " << str << "]" << DEF_COL << std::endl;
}

void	error(const std::string & str)
{
	std::cerr << RED << now() << "  Error: " << str << DEF_COL << std::endl;
}

void	debugPrintFd(const fd_set * tmpRecvSet, const fd_set * tmpSendSet, int maxSocket)
{
	if (DEBUG)
	{
		std::cout << GREY << "    Printing file descriptors..." << std::endl;
		int fd = 3;
		while (fd <= maxSocket)
		{
			std::cout << "    fd: " << fd << "\ttmpRecvSet: ";
			if (FD_ISSET(fd, tmpRecvSet))
				std::cout << "[X]";
			else
				std::cout << "[ ]";
			std::cout << "\ttmpSendSet ";
			if (FD_ISSET(fd, tmpSendSet))
				std::cout << "[X]";
			else
				std::cout << "[ ]";
			std::cout << std::endl;
			fd++;
		}
		std::cout << std::endl << DEF_COL << std::endl;
	}
}

void	printListening(int * listenIndex, int * listenWrittenSize)
{
	std::string	listen;

	if (*listenIndex == 0)
	{
		std::cout.flush();
		std::cout << "\x1b[" << *listenWrittenSize << "D";
		*listenIndex = 1;
	}
	else
	{
		if (*listenIndex <= 20)
			listen = "Listening";
		else if (*listenIndex <= 40)
			listen = "Listening.";
		else if (*listenIndex <= 60)
			listen = "Listening..";
		else if (*listenIndex <= 80)
			listen = "Listening...";

		*listenWrittenSize = listen.size();
		std::cout << listen;
		std::cout.flush();
		std::cout << "\x1b[" << *listenWrittenSize << "D";

		*listenIndex += 1;
		if (*listenIndex == 80)
			*listenIndex = 1;
	}
}
