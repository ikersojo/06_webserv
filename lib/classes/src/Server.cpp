/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:16:52 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/27 16:13:41 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

bool	Server::_shutdownRequested = false;


void	Server::SignalHandler(int signal)
{
	if (signal == SIGINT || signal == SIGTERM)
		_shutdownRequested = true;
}


Server::Server(Config * config)
{
	this->_config = config;
	this->_maxPorts = this->_config->getMaxPorts();
	FD_ZERO(&this->_recvSet);
	FD_ZERO(&this->_sendSet);
	this->_maxSocket = -1;
	this->_timeout.tv_sec = 0;
	this->_timeout.tv_usec = TIMEOUT;
	this->_maxSocket = -1;

	int	i = -1;
	while (++i < this->_maxPorts)
		this->_serverSocket.push_back(-1);

	signal(SIGINT, SignalHandler);
	signal(SIGTERM, SignalHandler);

	debug("Server Object Created");
}


Server::~Server()
{
	std::vector < int >::iterator	it;

	it = this->_clientSocket.begin();
	while (it != this->_clientSocket.end())
	{
		if (*it != -1)
		{
			if (close(*it) == -1)
				error("Client Socket could not be closed");
			else
				debug("... client socket closed");
		}
		it++;
	}

	it = this->_serverSocket.begin();
	while (it != this->_serverSocket.end())
	{
		if (*it != -1)
		{
			if (close(*it) == -1)
				error("Server Socket could not be closed");
			else
				debug("... server socket closed");
		}
		it++;
	}
	debug("Server Object Destroyed");
}


void	Server::init(void)
{
	std::cout << now() << "  Initializing server..." << std::endl;

	int i = 0;
	while (i < this->_maxPorts)
	{
		// Set up server address
		sockaddr_in		serverAddr;
		memset(&serverAddr, 0, sizeof(serverAddr));
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(this->_config->getPort(i));
		serverAddr.sin_addr.s_addr = INADDR_ANY;

		// Create a Socket
		if ((this->_serverSocket[i]= socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			error("Failed to create socket");
			exit (EXIT_FAILURE);
		}
		debug("...server socket defined");

		// Set the Socket as re-usable
		int		opt = 1;
		if (setsockopt(this->_serverSocket[i], SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1)
		{
			error("Failed to set server socket as re-ussable");
			exit (EXIT_FAILURE);
		}
		debug("...server socket options defined");

		// Bind the server address to the socket
		if (bind(this->_serverSocket[i], (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1)
		{
			error("Failed to bind server address to socket");
			exit (EXIT_FAILURE);
		}
		debug("...server address binded to socket");

		// Start listening for Communications
		if (listen(this->_serverSocket[i], BACKLOG) == -1)
		{
			error("Failed to listen for Communications");
			exit (EXIT_FAILURE);
		}
		debug("...server socket listening");

		// Set the socket as non-blocking
		if (fcntl(this->_serverSocket[i], F_SETFL, O_NONBLOCK) == -1)
		{
			error("Server socket cannot be set as non-blocked");
			return ;
		}
		debug("...server socket set as non-blocking");

		// Add the socket to the recv fd_set
		FD_SET(this->_serverSocket[i], &this->_recvSet);
		if (this->_serverSocket[i] > this->_maxSocket)
			this->_maxSocket = this->_serverSocket[i];
		debug("...server socket added to the recv fd_set");

		// Log on terminal
		std::cout << now() << "  Server listening in socket " << this->_serverSocket[i] << " to http://" << this->_config->getAddress(i) << ":" << this->_config->getPort(i) << std::endl;

		i++;
	}
	debug("Server initialized");
	std::cout << std::endl << "Press crtl + c to terminate the server" << std::endl << std::endl;
	this->startListeningSelect();
}

void Server::startListeningSelect(void) // MAIN LOOP
{
	int		select_res;

	this->_listenIndex = 0;
	this->_listenWrittenSize = 0;

	while (!Server::_shutdownRequested)
	{
		// Select for I/O multiplexing
		fd_set tmpRecvSet = this->_recvSet;
		fd_set tmpSendSet = this->_sendSet;
		select_res = select(this->_maxSocket + 1, &tmpRecvSet, &tmpSendSet, NULL, &this->_timeout);

		// Select: no event identified: print listening animation
		if (select_res == 0)
			printListening(&this->_listenIndex, &this->_listenWrittenSize);

		// Select error: only print error if there is no shutdown request. continue running the server after a select error
		if (select_res == -1)
		{
			this->_listenIndex = 0;
			if (!Server::_shutdownRequested)
				error("Select failed");
			continue;
		}

		// Select identified an event:
		else if (select_res > 0)
		{
			this->_listenIndex = 0; // reset listening animation
			debug("Select identified a file descriptor ready...");
			debugPrintFd(&tmpRecvSet, &tmpSendSet, this->_maxSocket);

			int fd = 3;
			while (fd <= this->_maxSocket)
			{
				int locInServer = locationInSocketVector(fd, this->_serverSocket);
				int locInClient = locationInSocketVector(fd, this->_clientSocket);

				if (FD_ISSET(fd, &tmpRecvSet))
				{
					debug("...fd ready to receive data");
					if (locInServer != -1) //the fd ready to recieve data is a server socket
						acceptConnection(locInServer);
					else if (locInClient != -1) //the fd ready to recieve data is a client socket
						readRequest(locInClient);
				}
				else if (FD_ISSET(fd, &tmpSendSet))
				{
					debug("...fd ready to send data");
					ResponseBuilder reponse(this->_config, this->_configIndex[locInClient], this->_requestStr[locInClient]);
					this->_responseStr[locInClient] = reponse.computeResponse();
					this->sendResponse(locInClient);
				}
				fd++;
			}
		}
	}
	// Log on terminal
	std::cout.flush();
	std::cout << "\x1b[" << this->_listenWrittenSize << "D";
	std::cout << now() << "  Server Terminated" << std::endl;
}

void	Server::acceptConnection(int locInServer)
{
	sockaddr_in	clientAddr;
	socklen_t	clientAddrLen = sizeof(clientAddr);
	int			clientSocket;

	debug("...server accepting client communication");
	if ((clientSocket = accept(this->_serverSocket[locInServer], (struct sockaddr *) &clientAddr, &clientAddrLen)) == -1)
	{
		error("Failed to accept Communications");
		return ;
	}
	debug("...client socket created for communication");

	if (fcntl(clientSocket, F_SETFL, O_NONBLOCK) == -1)
	{
		error("Client socket cannot be set as non-blocked");
		close(clientSocket);
		return ;
	}
	debug("...client socket set as non-blocking");

	// Print the new connection to terminal
	char	ipAddress[INET_ADDRSTRLEN];
	inet_ntop(AF_INET, &(clientAddr.sin_addr), ipAddress, INET_ADDRSTRLEN);
	std::cout << now() << "  " << "Connection of client " << ipAddress << " in socket " << clientSocket
				<< " to http://" << this->_config->getAddress(locInServer) << ":" << this->_config->getPort(locInServer) << std::endl;

	// Add the socket and setup the request and response strings, aligned in the same index of their vectors
	this->_clientSocket.push_back(clientSocket);
	this->_configIndex.push_back(locInServer);
	this->_requestStr.push_back("");
	this->_responseStr.push_back("");

	// Add the client socket to the recv fd_set
	FD_SET(clientSocket, &this->_recvSet);
	if (clientSocket > this->_maxSocket)
		this->_maxSocket = clientSocket;

	debug("Client Connected");
}


void	Server::readRequest(int locInClient)
{
	ssize_t	bytesRead = BUFFSIZE;
	char	buffer[BUFFSIZE];
	memset(buffer, 0, BUFFSIZE);

	while (bytesRead == BUFFSIZE) // multiple reads if BUFFSIZE is exceeded
	{
		debug("...reading request from client");
		bytesRead = recv(this->_clientSocket[locInClient], buffer, sizeof(buffer), 0);
		if (DEBUG)
			std::cout << GREY << "[DEBUG: ...bytes read: " << bytesRead << "]"
					<< DEF_COL << std::endl;
		if (bytesRead == 0)
			debug("Client terminated conection");
		else if (bytesRead == -1)
		{
			error("Request could not be read from client");
			this->closeClient(locInClient, &this->_recvSet);
		}
		else if (bytesRead > 0)
		{
			std::string	requestString(buffer, bytesRead);
			this->_requestStr[locInClient] += requestString;
		}
	}
	FD_CLR(this->_clientSocket[locInClient], &this->_recvSet);
	FD_SET(this->_clientSocket[locInClient], &this->_sendSet);

	debug("...request received and saved");
	if (DEBUG) // remove for prod
		std::cout << YELLOW << "\n---- Received from client ----\n\n" <<
		this->_requestStr[locInClient].substr(0, this->_requestStr[locInClient].find("\n"))
		<< DEF_COL << std::endl;
}


void	Server::sendResponse(int locInClient)
{
	debug("Sending response...");
	 if (DEBUG) // remove for prod
	 	std::cout << YELLOW << "\n---- Response to client ----\n\n" <<
		this->_responseStr[locInClient].substr(0, this->_responseStr[locInClient].find("\n"))
		<< DEF_COL << std::endl;

	std::string	pendingString = this->_responseStr[locInClient];
	ssize_t totalBytesSent = 0;
	while (totalBytesSent != (ssize_t)this->_responseStr[locInClient].size())
	{
		ssize_t bytesSent = send(this->_clientSocket[locInClient], pendingString.c_str(), this->_responseStr[locInClient].size(), 0);
		if (bytesSent == 0)
		{
			debug("Client terminated conection");
			break ;
		}
		else if (bytesSent == -1)
		{
			error("Failed to send response");
			break ;
		}
		else if (bytesSent > 0)
		{
			if (DEBUG)
				std::cout << GREY << "[DEBUG: ..." << bytesSent << " bytes succesfully sent]" << DEF_COL << std::endl;
			totalBytesSent += bytesSent;
			pendingString = pendingString.substr(bytesSent + 1, pendingString.size());
		}
	}
	this->closeClient(locInClient, &this->_sendSet);
}

// Close socket and remove all reference of the data which has been transferred
void	Server::closeClient(int locInClient, fd_set * set)
{
	close(this->_clientSocket[locInClient]);
	if (this->_clientSocket[locInClient] == this->_maxSocket)
		this->_maxSocket--;
	FD_CLR(this->_clientSocket[locInClient], set);
	this->_clientSocket.erase(this->_clientSocket.begin() + locInClient);
	this->_requestStr.erase(this->_requestStr.begin() + locInClient);
	this->_configIndex.erase(this->_configIndex.begin() + locInClient);
	this->_responseStr.erase(this->_responseStr.begin() + locInClient);
}
