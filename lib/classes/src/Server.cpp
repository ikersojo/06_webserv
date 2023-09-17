#include "../inc/Server.hpp"

Server::Server(void)
{
	debug("Server Object Created");
}

Server::~Server()
{
	debug("Server Object Destroyed");

	size_t i = 0;
	while (i < this->_maxPorts)
	{
		if (this->_serverSockets[i] != -1)
			close(this->_serverSockets[i]);
		i++;
	}
}

//dummy function, to be replaced by one loading all private variables from file TODO
void	Server::loadConfig(const std::string & configFile)
{

	std::cout << "Loading " << configFile << "..." << std::endl;

	this->_maxPorts = 3;
	this->_ports.push_back(61000);
	this->_ports.push_back(61001);
	this->_ports.push_back(61002);
	
	size_t	i = 0;
	while (i < this->_maxPorts)
	{
		this->_serverSockets.push_back(-1);
		i++;
	}
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

	while (true)
	{
		debug("Listening...");
		fd_set tmpSet = readSet;
		if (select(maxSocket + 1, &tmpSet, nullptr, nullptr, nullptr) == -1)
		{
			error("Select failed");
			exit(EXIT_FAILURE);
		}
		debug("...select identified an event");

		i = 0;
		while (i < this->_maxPorts)
		{
			if (FD_ISSET(this->_serverSockets[i], &tmpSet))
			{
				debug("...event on listening socket identified.");
				acceptConnections(this->_serverSockets[i]);
			}
			i++;
		}
	}
}

void	Server::acceptConnections(int serverSocket)
{
	sockaddr_in	clientAddr;
	socklen_t	clientAddrLen = sizeof(clientAddr);
	int			clientSocket;

	clientSocket = accept(serverSocket, (struct sockaddr *) &clientAddr, &clientAddrLen);
	if (clientSocket == -1)
	{
		error("Failed to accept connections.");
		exit (EXIT_FAILURE);
	}
	debug("Client Connected. Awaiting request...");




	// -------  TO BE EXTRACTED------------

	// Read the HTTP request from the client
	char	buffer[BUFFSIZE];
	ssize_t	bytesRead;

	bzero(&buffer, BUFFSIZE);
	if ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) <= 0)
	{
		close(clientSocket);
		return ;
	}
	debug("...request received.");

	std::string requestString(buffer);
	std::cout << "Received from client:\n------------------------------------\n" << requestString << std::endl;

	// Generate a response
	std::string responseStr =	"HTTP/1.1 200 OK\r\n"
								"Content-Type: text/plain\r\n"
								"Content-Length: 23\r\n"
								"\r\n"
								"Hello, Andoni and John!";

	// Send the response back to the client
	ssize_t bytesSent = send(clientSocket, responseStr.c_str(), responseStr.size(), 0);
	if (bytesSent == -1)
		error("Failed to send response.");
	debug("...response sent");


	// -------  


	// Close the client socket
	close(clientSocket);
	debug("...conection closed to client");
}
