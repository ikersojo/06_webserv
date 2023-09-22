/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:16:58 by isojo-go          #+#    #+#             */
/*   Updated: 2023/09/22 08:39:06 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Connection.hpp"

/* CLASS AUX FUNCTIONS AND INITIALIZING */

bool	Connection::_shutdownRequested = false;

void	Connection::SignalHandler(int signal)
{
	if (signal == SIGINT || signal == SIGTERM)
	{
		debug("Requested shutdown on Connection...");
		_shutdownRequested = true;
	}
}


/* MAIN CLASS METHODS */

Connection::Connection(int serverSocket, Config * config)
{
	debug("Connection Object Created");
	this->_config = config;
	this->_clientAddrLen = sizeof(this->_clientAddr);
	this->_serverSocket = serverSocket;
	this->_clientAddrLen = sizeof(this->_clientAddrLen);
	this->_clientSocket = accept(serverSocket, (struct sockaddr *) &this->_clientAddr, &this->_clientAddrLen);
	if (this->_clientSocket == -1)
	{
		error("Failed to accept connections");
		exit (EXIT_FAILURE);
	}
	debug("Client Connected. Awaiting request...");
	this->assessRequest();
}

Connection::~Connection()
{
	debug("Connection destroyed");
}

void	Connection::assessRequest(void)
{
	char	buffer[BUFFSIZE];
	ssize_t	bytesRead;

	bzero(&buffer, BUFFSIZE);

	// Read the HTTP request from the client
	if ((bytesRead = recv(this->_clientSocket, buffer, sizeof(buffer), 0)) <= 0)
	{
		close(this->_clientSocket);
		return ;
	}
	std::cout << YELLOW << "bytes read: " << bytesRead << DEF_COL << std::endl;
	std::string requestString(buffer);
	this->_requestString = requestString;
	debug("...request received and saved");

	// ------- 

	std::cout << YELLOW << "Received from client:\n------------------------------------\n" << requestString << DEF_COL << std::endl;


	// -------  

	// Generate a response
	this->buildResponse();

	// Send the response
	this->sendResponse();

	// Close the client socket
	close(this->_clientSocket);
	debug("...conection closed to client");
}

void	Connection::buildResponse(void)
{
	this->_responseStr =		"HTTP/1.1 200 OK\r\n"
								"Content-Type: text/plain\r\n"
								"Content-Length: 23\r\n"
								"\r\n"
								"Hello, Andoni and John!";
}

void	Connection::sendResponse(void)
{
	ssize_t bytesSent = send(this->_clientSocket, this->_responseStr.c_str(), this->_responseStr.size(), 0);
	if (bytesSent == -1)
		error("Failed to send response");
	debug("...response sent");
}
