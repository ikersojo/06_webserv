/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:16:58 by isojo-go          #+#    #+#             */
/*   Updated: 2023/09/23 11:36:34 by isojo-go         ###   ########.fr       */
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

Connection::Connection(int serverSocket, Config * config, int location)
{
	debug("Connection Object Created");
	this->_config = config;
	this->_location = location;
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
	this->manageRequest();
}

Connection::~Connection()
{
	debug("Connection destroyed");
}

void	Connection::manageRequest(void)
{
	this->_ok = true;

	if (this->_ok)
		this->readRequest();
	debug("...request received and saved");

	if (this->_ok)
		this->buildResponse();
	debug("...respose built");

	if (this->_ok)
		this->sendResponse();
	debug("...response sent");

	close(this->_clientSocket);
	debug("...conection closed to client");
}

void	Connection::readRequest(void)
{
	char	buffer[BUFFSIZE];
	ssize_t	bytesRead;

	bzero(&buffer, BUFFSIZE);

	if ((bytesRead = recv(this->_clientSocket, buffer, sizeof(buffer), 0)) <= 0)
	{
		this->_ok = false;
		return ;
	}
	if (DEBUG)
		std::cout << GREY << "[DEBUG: ...bytes read: " << bytesRead << "]"
				<< DEF_COL << std::endl;

	std::string requestString(buffer);
	this->_requestString = requestString;

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
	// if (DEBUG)
	// 	std::cout << GREY << "[DEBUG:Received from client:\n------------------------------------\n"
	// 			<< this->_requestString << "]" << DEF_COL << std::endl << std::endl;
}

void	Connection::buildResponse(void)
{

	// find the html file from config
	std::string	filepath;

	filepath = this->_config->getDirs()[this->_location];
	filepath.append("/");
	filepath.append(this->_config->getFiles()[this->_location]);
	if (DEBUG)
		std::cout << GREY << "[DEBUG: ...importing location " << this->_location << ": "
				<< filepath << "]" << DEF_COL << std::endl;

	// read the html file
	std::ifstream		inFile;
	std::string			line;
	std::ostringstream	oss;
	int					charCounter = 0;

	// reponse header
	this->_responseStr =	"HTTP/1.1 200 OK\r\n"
							"Content-Type: text/plain\r\n"
							"Content-Length: ";
	inFile.open(filepath);
	if (!inFile.is_open())
	{
		error("html file not found"); // TO BE REPLACED BY 404
		this->_ok = false;
		return ;
	}


	while (getline(inFile, line))
	{
		oss << line;
		charCounter += line.size();
	}

	oss << charCounter;

	oss << "\r\n\r\n";

	oss << "\r\n\r\n";

	std::string result = oss.str();

	inFile.close();

}

void	Connection::sendResponse(void)
{
	ssize_t bytesSent = send(this->_clientSocket, this->_responseStr.c_str(), this->_responseStr.size(), 0);
	if (bytesSent == -1)
		error("Failed to send response");
}
