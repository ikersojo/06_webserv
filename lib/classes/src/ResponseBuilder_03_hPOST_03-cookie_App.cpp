#include "../inc/ResponseBuilder.hpp"

std::string	ResponseBuilder::setCookie(void)
{
	std::string	cookie, key, value;
	std::ofstream outFile;

	cookie = _requestStr.substr(_requestStr.find("\r\n\r\n")+4);

	outFile.open("www/register/cookie", std::ios_base::app);
	outFile << cookie + "\n";

	_responseStr += "HTTP/1.1 200 OK\r\n";
	_responseStr += "Set-Cookie: " + cookie + "; Max-Age=30;\r\n";
	_responseStr += "\r\n";
	return(_responseStr);
}
