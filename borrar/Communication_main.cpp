/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Communication_main.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:16:58 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/12 22:11:29 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Communication.hpp"

/* CLASS AUX FUNCTIONS AND INITIALIZING */

bool	Communication::_shutdownRequested = false;


void	Communication::SignalHandler(int signal)
{
		if (signal == SIGINT || signal == SIGTERM)
		{
			debug("Requested shutdown on Communication...");
			Communication::_shutdownRequested = true;
		}
}


/* MAIN CLASS METHODS */

Communication::Communication(int serverSocket, Config * config, int location)
{
	debug("Communication Object Created");
	this->_config = config;
	this->_location = location;
	this->_clientAddrLen = sizeof(this->_clientAddr);
	this->_serverSocket = serverSocket;
	this->_clientAddrLen = sizeof(this->_clientAddrLen);
	this->_clientSocket = accept(serverSocket, (struct sockaddr *) &this->_clientAddr, &this->_clientAddrLen);
	debug("...client socket accepted communication");
	if (this->_clientSocket == -1)
	{
		error("Failed to accept Communications");
		exit (EXIT_FAILURE);
	}
	if (fcntl(this->_clientSocket, F_SETFL, O_NONBLOCK) == -1)
	{
		error("Client socket cannot be set as non-blocked");
		this->_ok = false;
		return ;
	}
	debug("...client socket set as non-blocking");

	char	ipAddress[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(this->_clientAddr.sin_addr), ipAddress, INET_ADDRSTRLEN);
	std::cout << now() << "  " << "Client " << ipAddress << " assigned to socket " << this->_clientSocket << std::endl;
	debug("Client Connected. Awaiting request...");

	this->manageRequest();
}


Communication::~Communication()
{
	debug("Communication destroyed");
}


void	Communication::manageRequest(void)
{
	this->_ok = true;

	this->readRequest();
	if (this->_ok && !this->_config->isValidRequest(this->_location, this->_requestParams[1]))
	{
		error("Not valid url");
		build404errorResponse();
	}
	else if (this->_ok && this->_requestParams[0] == "GET")
		this->handleGetRequest();
	else if (this->_ok && this->_requestParams[0] == "POST")
		this->handlePostRequest();
	else if (this->_ok && this->_requestParams[0] == "DELETE")
		this->handleDeleteRequest();
	if (this->_ok)
		this->sendResponse();
	if (close(this->_clientSocket) == -1)
		error("Client Socket could not be closed");
	else
		debug("...conection closed to client");
}

void	Communication::readRequest(void)
{
	char	buffer[BUFFSIZE];
	ssize_t	bytesRead = 0;
	this->_requestString = "";
	size_t	it = 0;

	memset(buffer, 0, BUFFSIZE);
	debug("...reading request");
	while (!Communication::_shutdownRequested && it < MAXREADIT)
	{
		bytesRead = recv(this->_clientSocket, buffer, sizeof(buffer), 0);
		if (bytesRead >= 0)
			break ;
		it++;
	}
	if (DEBUG)
		std::cout << GREY << "[DEBUG: ...bytes read: " << bytesRead << " in " << it << " attempts]"
				<< DEF_COL << std::endl;
	
	if (bytesRead == -1)
	{
		this->_ok = false;
		return ;
	}

	std::string	requestString(buffer);
	this->_requestString += requestString;
	// if (DEBUG) // remove for prod
	// 	std::cout << YELLOW << "[DEBUG: ---- Received from client ----\n\n"
	// 			<< this->_requestString << "]" << DEF_COL << std::endl << std::endl;

	std::istringstream	iss(buffer);
	std::string			item;
	while (iss >> item)
		this->_requestParams.push_back(item);
	if (this->_requestParams.size() < 3)
	{
		error("HTTP request header is not correct");
		this->_ok = false;
		return ;
	}
	int i = -1;
	while (++i < 3)
	{
		if (this->_requestParams[i].empty())
		{
			error("HTTP request header content is not correct");
			this->_ok = false;
			return ;
		}
		if (DEBUG)
			std::cout << GREY << "[DEBUG: ...param " << i << ": "
					<< this->_requestParams[i] << "]" << DEF_COL << std::endl;
	}
	if (this->_requestParams[2] != "HTTP/1.1")
	{
		error("webserv only work with HTTP/1.1 requests");
		this->_ok = false;
		return ;
	}
	debug("...request received and saved");
}

