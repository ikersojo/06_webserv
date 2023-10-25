/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder_01_hERROR.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarrien- <aarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 09:16:19 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/25 16:55:15 by aarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ResponseBuilder.hpp"

std::string	resolveErrorCode(int code)
{
	switch (code) {
		case 400: return ("Bad Request");
		case 404: return ("Not Found");
		case 405: return ("Method Not Allowed");
		case 500: return ("Internal Server Error");
		case 505: return ("HTTP Version Not Supported");
		default:  return ("Unknown Error");
	}
}

std::string	ResponseBuilder::errorResponse(int code)
{
	std::map<int, std::string> errorPages;
	std::ifstream		inFile;
	std::ostringstream	fileContentStream;
	std::string			fileContent;

	if (!_requestStr.empty())
		errorPages = this->_config->getErrorPage(this->_configIndex, _config->getNearestLocation(_configIndex, this->_requestParams[1]));
	if (!errorPages.empty() && errorPages[code] != "") {
		inFile.open(errorPages[code]);
		if (!inFile.is_open())
			error("Requested file not found");
		else {
			fileContentStream << inFile.rdbuf();
			fileContent = fileContentStream.str();

			this->_responseStr = "HTTP/1.1 " + std::to_string(code) + " " + resolveErrorCode(code) + "\r\n";
			this->_responseStr += "Content-Type: text/html\r\n\r\n";
			this->_responseStr += fileContent;
			return (this->_responseStr);
		}
	}

	this->_responseStr = "HTTP/1.1 " + std::to_string(code) + " " + resolveErrorCode(code) + "\r\n";
	this->_responseStr += "Content-Type: text/html\r\n\r\n";
	this->_responseStr += "<html><head><title>Error " + intToString(code) + "</title></head><body>";
	this->_responseStr +=  "<h1>Error " + intToString(code) + "</h1>";
	debug("...error response built");
	std::cout << "error response:\n" << _responseStr;
	return (this->_responseStr);
}
