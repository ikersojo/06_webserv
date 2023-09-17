#include "../inc/Server.hpp"

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

		// Bind the socket
		if (bind(this->_serverSockets[i], (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1)
		{
			error("Failed to bind socket");
			exit (EXIT_FAILURE);
		}
		debug("...server socket binded");

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
	this->startListeningOnAllPorts();
}

void Server::startListeningOnAllPorts(void)
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
				debug("Event on listening socket identified.");
				acceptConnections(i);
			}
			i++;
		}
	}
}

void	Server::acceptConnections(int threadIndex)
{
	sockaddr_in	clientAddr;
	socklen_t	clientAddrLen = sizeof(clientAddr);
	int			clientSocket;

	clientSocket = accept(this->_serverSockets[threadIndex], (struct sockaddr *) &clientAddr, &clientAddrLen);
	if (clientSocket == -1)
	{
		error("Failed to accept connections.");
		exit (EXIT_FAILURE);
	}
	debug("Client Connected");




	// -------  TO BE EXTRACTED------------

	// Read the HTTP request from the client
	char	buffer[4096];
	ssize_t	bytesRead;

	bzero(&buffer, 4096);
	if ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) <= 0)
	{
		close(clientSocket);
		return ;
	}

	std::string requestString(buffer);
	std::cout << "Received: " << requestString << std::endl;

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



	// -------  


	// Close the client socket
	close(clientSocket);
}
