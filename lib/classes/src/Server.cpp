/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/20 21:16:52 by isojo-go          #+#    #+#             */
/*   Updated: 2023/09/22 11:43:54 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Server.hpp"

/* CLASS AUX FUNCTIONS AND INITIALIZING */

bool	Server::_shutdownRequested = false;

bool	SetSocketReuseAddr(int socket)
{
	int yes = 1;
	if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1)
		return false;
	return true;
}

void	Server::SignalHandler(int signal)
{
	if (signal == SIGINT || signal == SIGTERM)
	{
		debug("Requested shutdown on Server...");
		_shutdownRequested = true;
	}
}


/* MAIN CLASS METHODS */

Server::Server(Config * config)
{
	int temp;
	this->_config = config;

	this->_maxPorts = this->_config->getMaxPorts();
	size_t	i = 0;
	while (i < this->_maxPorts)
	{
		this->_serverSockets.push_back(-1);
		temp = this->_config->getPorts()[i];
		this->_ports.push_back(temp);
		i++;
	}

	signal(SIGINT, SignalHandler);
	signal(SIGTERM, SignalHandler);

	debug("Server Object Created");
}

Server::~Server()
{

	size_t i = 0;
	while (i < this->_maxPorts)
	{
		if (this->_serverSockets[i] != -1)
		{
			close(this->_serverSockets[i]);
			debug("...socket closed");
		}
		i++;
	}
	debug("Server Object Destroyed");
}

void	Server::init(void)
{
	debug("Initializing server...");

	size_t i = 0;
	while (i < this->_maxPorts)
	{
		// Set up server address
		sockaddr_in		serverAddr;

		bzero(&serverAddr, sizeof(serverAddr));
		serverAddr.sin_family = AF_INET;
		serverAddr.sin_port = htons(this->_ports[i]);
		serverAddr.sin_addr.s_addr = INADDR_ANY;

		// Create a Socket
		if ((this->_serverSockets[i]= socket(AF_INET, SOCK_STREAM, 0)) == -1)
		{
			error("Failed to create socket");
			exit (EXIT_FAILURE);
		}
		debug("...server socket defined");

		if (!SetSocketReuseAddr(this->_serverSockets[i]))
		{
			error("Failed to set socket as re-ussable");
			exit (EXIT_FAILURE);
		}
		debug("...server socket re-usage allowed");

		// Bind the server address to the socket
		if (bind(this->_serverSockets[i], (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1)
		{
			error("Failed to bind server address to socket");
			exit (EXIT_FAILURE);
		}
		debug("...server address binded to socket");

		// Start listening for connections
		if (listen(this->_serverSockets[i], BACKLOG) == -1)
		{
			error("Failed to listen for connections");
			exit (EXIT_FAILURE);
		}
		debug("...server socket listening");

		std::cout << "Server listening in http://localhost:" << this->_ports[i] << std::endl;
		i++;
	}
	debug("Server initialized");
	this->startListening();
}

void Server::startListening(void)
{
	debug("Setup select to iterate over the ports...");

	int maxSocket = -1;
	fd_set readSet;

	FD_ZERO(&readSet);

	size_t i = 0;
	while (i < this->_maxPorts)
	{
		FD_SET(this->_serverSockets[i], &readSet);
		if (this->_serverSockets[i] > maxSocket)
			maxSocket = this->_serverSockets[i];
		i++;
	}
	debug("...FD_SET ready with the listening ports");

	while (!_shutdownRequested)
	{	if (DEBUG)
			std::cout << std::endl;
		debug("Listening...");
		fd_set tmpSet = readSet;
		if (select(maxSocket + 1, &tmpSet, NULL, NULL, NULL) == -1)
		{
			if (!_shutdownRequested) // only print error if there is no shutdown request
				error("Select failed");
			continue; // continue running the server after a select error

		}
		debug("...select identified an event");

		i = 0;
		while (i < this->_maxPorts)
		{
			if (FD_ISSET(this->_serverSockets[i], &tmpSet))
			{
				debug("...event on listening socket identified");
				Connection conn(this->_serverSockets[i], this->_config);
			}
			i++;
		}
	}
}
