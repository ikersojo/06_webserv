#include "../inc/ResponseBuilder.hpp"

std::string		ResponseBuilder::deChunk(void)
{
	std::string body = "testing...";

	this->_responseStr = "HTTP/1.1 200 OK\r\n";
	this->_responseStr += "Content-Type: text/html\r\n\r\n";
	this->_responseStr += "<html><head><title>De-Chuncked content</title></head><body>";
	this->_responseStr += "<h1>De-Chuncked content:</h1><p>";
	this->_responseStr += body;
	this->_responseStr +=  "</p>";
	return (this->_responseStr);
}
