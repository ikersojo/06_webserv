#include "../inc/ResponseBuilder.hpp"

std::string		ResponseBuilder::deChunk(void)
{
	size_t endOfHeaderPos = this->_requestStr.find("\r\n\r\n");
	std::string header = this->_requestStr.substr(0, endOfHeaderPos + 4);
	std::string body = this->_requestStr.substr(endOfHeaderPos + 4, this->_requestStr.length());

	this->_responseStr = "HTTP/1.1 200 OK\r\n";
	this->_responseStr += "Content-Type: text/plain\r\n\r\n";
	this->_responseStr += body;
	return (this->_responseStr);
}
