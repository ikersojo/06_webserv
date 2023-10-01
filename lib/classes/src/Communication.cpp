/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Communication.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:16:58 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/01 15:58:44 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Communication.hpp"

std::string	intToString(int n)
{
	std::string	str;
	std::stringstream ss;

	ss << n;
	str = ss.str();
	return (str);
}


/* CLASS AUX FUNCTIONS AND INITIALIZING */

bool	Communication::_shutdownRequested = false;

void	Communication::SignalHandler(int signal)
{
	if (signal == SIGINT || signal == SIGTERM)
	{
		debug("Requested shutdown on Communication...");
		_shutdownRequested = true;
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
	if (this->_clientSocket == -1)
	{
		error("Failed to accept Communications");
		exit (EXIT_FAILURE);
	}
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

	if (this->_ok)
		this->readRequest();
	debug("...request received and saved");

	if (this->_ok && this->_requestParams[0] == "GET")
	{
		if (this->_ok)
			this->buildResponse();
		debug("...response built");

		if (this->_ok)
			this->sendResponse();
		debug("...response succesfully sent");
	}
	else if (this->_ok && this->_requestParams[0] == "POST")
	{
		error("POST not defined yet"); // TO BE MODIFIED <----------------------------fcntl(fd, F_SETFL, O_NONBLOCK);
	}
	else if (this->_ok && this->_requestParams[0] == "DELETE")
	{
		error("DELETE not defined yet"); // TO BE MODIFIED <----------------------------fcntl(fd, F_SETFL, O_NONBLOCK);
	}

	close(this->_clientSocket);
	debug("...conection closed to client");
}

void	Communication::readRequest(void)
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

	std::string	requestString(buffer);
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
	if (DEBUG)
		std::cout << YELLOW << "[DEBUG: ---- Received from client ----\n\n"
				<< this->_requestString << "]" << DEF_COL << std::endl << std::endl;
}

void	Communication::buildResponse(void)
{
	std::string			filepath;
	std::ifstream		inFile;
	std::ostringstream	fileContentStream;
	std::string			fileContent;
	int					fileSize;

	std::string			line, temp;

    // Read the requested file content into a string
	filepath = this->_config->getFile(this->_location, this->_requestParams[1]);
	if (DEBUG)
		std::cout << GREY << "[DEBUG: file requested: " << filepath << "]" << DEF_COL << std::endl;
	inFile.open(filepath);
	if (!inFile.is_open())
	{
		error("html file not found"); // TO BE REPLACED BY 404
		this->_ok = false;
		return ;
	}
    fileContentStream << inFile.rdbuf();
    fileContent = fileContentStream.str();
	fileSize = fileContent.size();

	// reponse header example:
	// 		"HTTP/1.1 200 OK\r\n"
	// 		"Content-Type: text/plain\r\n"
	// 		"Content-Length: 23\r\n"
	// 		"\r\n"
	// 		"Hello, Andoni and John!";

	this->_responseStr = "HTTP/1.1 200 OK\r\n";
	if (this->_requestParams[1] == "/")
		this->_responseStr += "Content-Type: text/html\r\n";
	else if (this->_requestParams[1] == "/favicon.ico" || this->_requestParams[1] == "/image")
		this->_responseStr += "Content-Type: image/vnd.microsoft.icon\r\n";
	else
		this->_responseStr += "Content-Type: text/plain\r\n";
	this->_responseStr += "Content-Length: ";
	this->_responseStr += intToString(fileSize);
	this->_responseStr += "\r\n\r\n";
	this->_responseStr += fileContent;
	inFile.close();
	// if (DEBUG)
	// 	std::cout << GREY << "[DEBUG: ...response: \n" << this->_responseStr << "\n]" << DEF_COL << std::endl;
}

void	Communication::sendResponse(void)
{
	ssize_t bytesSent = send(this->_clientSocket, this->_responseStr.c_str(), this->_responseStr.size(), 0);
	if (bytesSent == -1)
		error("Failed to send response");
	else if (DEBUG)
		std::cout << GREY << "[DEBUG: ..." << bytesSent << " bytes sent]" << DEF_COL << std::endl;
}
