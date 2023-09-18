#include "../inc/Connection.hpp"

Connection::Connection(int serverSocket)
{
	debug("Connection Object Created");
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
	this->receiveRequest();
}

Connection::~Connection()
{
	debug("Connection destroyed");
}

void	Connection::receiveRequest(void)
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
	debug("...request received");

	std::string requestString(buffer);
	std::cout << "Received from client:\n------------------------------------\n" << requestString << std::endl;

	// Generate a response
	std::string responseStr =	"HTTP/1.1 200 OK\r\n"
								"Content-Type: text/plain\r\n"
								"Content-Length: 23\r\n"
								"\r\n"
								"Hello, Andoni and John!";

	// Send the response back to the client
	ssize_t bytesSent = send(this->_clientSocket, responseStr.c_str(), responseStr.size(), 0);
	if (bytesSent == -1)
		error("Failed to send response");
	debug("...response sent");


	// -------  


	// Close the client socket
	close(this->_clientSocket);
	debug("...conection closed to client");
}
