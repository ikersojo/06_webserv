#include "../inc/Server.hpp"

Server::Server(int port) : _port(port)
{
	debug("Server Object Created");

	// Set up server address
	sockaddr_in		serverAddr;

	bzero(&serverAddr, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	// Create a Socket
	if ((this->_serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		error("Failed to create socket.");
		exit (EXIT_FAILURE);
	}
	debug("Server socket defined");

	// Bind the socket
	if (bind(this->_serverSocket, (struct sockaddr *) &serverAddr, sizeof(serverAddr)) == -1)
	{
		error("Failed to bind socket.");
		exit (EXIT_FAILURE);
	}
	debug("Server socket binded");

	// Start listening for connections
	if (listen(this->_serverSocket, BACKLOG) == -1)
	{
		error("Failed to listen for connections.");
		exit (EXIT_FAILURE);
	}
	std::cout << "Server listening in http://localhost:" << this->_port << std::endl;
	while (true)
		acceptConnections();
}

Server::~Server()
{
	debug("Server Object Destroyed");

	if (this->_serverSocket != -1)
			close(this->_serverSocket);
}

void	Server::acceptConnections()
{
	sockaddr_in	clientAddr;
	socklen_t	clientAddrLen = sizeof(clientAddr);
	int			clientSocket;

	clientSocket = accept(this->_serverSocket, (struct sockaddr *) &clientAddr, &clientAddrLen);
	if (clientSocket == -1)
	{
		error("Failed to accept connections.");
		exit (EXIT_FAILURE);
	}
	debug("Client Connections defined");



	// -------  ESTO TENDREMOS QUE SACARLO DE AQUÍ ------------

	// Read the HTTP request from the client
	char	buffer[4096];
	ssize_t	bytesRead;

	bzero(&buffer, 4096);
	if ((bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0)) <= 0)
	{
		close(clientSocket);
		return ;
	}

	std::string requestString(buffer, static_cast<size_t>(bytesRead));

	// Parse the request (You'll need to implement the Request class for this)
	// Request request(requestString);
	std::cout << "Received: " << requestString << std::endl;

	// Handle the request and generate a response
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
