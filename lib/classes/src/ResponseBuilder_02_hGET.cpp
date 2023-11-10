/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder_02_hGET.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarrien- <aarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 09:16:19 by isojo-go          #+#    #+#             */
/*   Updated: 2023/11/10 18:15:49 by aarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ResponseBuilder.hpp"

std::string	ResponseBuilder::searchCookie(void)
{
	std::ifstream inFile;
	std::ostringstream	fileContentStream;
	std::string	fileContent;
	size_t pos = _requestStr.find("Cookie:") + 8;
	std::string cookie = _requestStr.substr(pos, _requestStr.find("\r\n", pos) - pos);
	if (cookie.find(";")) {
		size_t nameStartPos = cookie.find("name=");
		size_t nameEndPos = cookie.find(nameStartPos, cookie.find(";", nameStartPos) - nameStartPos);
		cookie = cookie.substr(nameStartPos, nameEndPos);
	}

	if (DEBUG)
		std::cout << "Received Cookie [" << cookie << "]" << std::endl;

	inFile.open("www/register/cookie");
	if (!inFile.is_open())
		error("Cookie file not found");

	fileContentStream << inFile.rdbuf();
	fileContent = fileContentStream.str();

	if (fileContent.find(cookie + "\n") != std::string::npos) {
		debug("Cookie found");
		return (cookie);
	}
	return("");
}

std::string		ResponseBuilder::cookieResponse(std::string cookie) {
	_responseStr = "HTTP/1.1 200 OK\r\n";
	_responseStr += "Content-Type: text/html\r\n";
	_responseStr += "\r\n";
	_responseStr += "<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><title>HARL</title></head><body>";
	_responseStr += "<h1>Hola " + cookie.substr(cookie.find("=")+1) + "<h2>";
	_responseStr += "</body></html>";
	return (_responseStr);
}

std::string	ResponseBuilder::getResponse(void)
{
	if (!this->_config->isGET(this->_configIndex, this->_requestParams[1]))
	{
		error("GET is not allowed");
		return(this->errorResponse(405));
	}
	if (this->_config->isRedir(this->_configIndex, this->_requestParams[1]))
	{
		debug("...redirection requested");
		return (this->redirResponse());
	}
	else if (this->_config->isCgi(this->_configIndex, this->_requestParams[1]))
	{
		debug("...CGI requested");
		return (this->cgiGETResponse());
	}
	else if (this->_config->isAutoIndex(this->_configIndex, this->_requestParams[1]))
	{
		debug("...autoindex requested");
		return (this->aiResponse());
	}
	else
	{
		if (_requestStr.find("Cookie:") != std::string::npos && _config->getHandlePOST(_configIndex, _requestParams[1]) == "setCookie") {
			std::string	cookie = searchCookie();
			if (!cookie.empty())
				return (this->cookieResponse(cookie));
		}
		debug("...file requested");
		return (this->fileResponse());
	}
}


std::string	ResponseBuilder::redirResponse(void)
{
	this->_responseStr = "HTTP/1.1 308 Permanent Redirect\r\n";
	this->_responseStr += "Location: https://";
	this->_responseStr += this->_config->getFile(this->_configIndex, this->_requestParams[1]);
	this->_responseStr += "\r\n\r\n";
	debug("...redirect response built");
	return (this->_responseStr);
}

std::string	ResponseBuilder::aiResponse(void)
{
	std::string	path = this->_requestParams[1];
	std::string	actualPath = _config->getActualPath(_configIndex, path);

	if (!path.empty() && path[path.size()-1] != '/')
		path += "/";

	if (_config->isAutoIndex(_configIndex, path)) {
		DIR* dir = opendir(actualPath.c_str());
		if (dir != NULL) {
			this->_responseStr = "HTTP/1.1 200 OK\r\n";
			this->_responseStr += "Content-Type: text/html\r\n\r\n";
			this->_responseStr += "<html><head><title>Index of " + path + "</title></head><body>";
			this->_responseStr +=  "<h1>Index of " + path + "</h1><ul>";

			struct dirent *	entry;
			while ((entry = readdir(dir)) != NULL)
				this->_responseStr += "<li><a href=\"" + path + entry->d_name + "\">" + entry->d_name + "</a></li>";
			this->_responseStr += "</ul></body></html>";
			closedir(dir);
			return (this->_responseStr);
		}
	} else {
		error("Directory doesn't support autoindex");
		return (this->errorResponse(403));
	}
	return (this->fileResponse());
}


std::string	ResponseBuilder::fileResponse(void)
{
	std::string			filePath;
	std::ifstream		inFile;
	std::ostringstream	fileContentStream;
	std::string			fileContent;
	std::string			fileExtension;
	int					fileSize;

	filePath = _config->getActualPath(_configIndex, _requestParams[1]);

	if (DEBUG)
		std::cout << GREY << "[DEBUG: ...requested file: " << filePath << "]" << DEF_COL << std::endl;

	struct stat statbuf;

	if (stat(filePath.c_str(), &statbuf) == 0 && !S_ISREG(statbuf.st_mode)) {
		error("Requested file is a directory");
		return (this->errorResponse(404)); // <---- REVISAR: Deberiamos permitir acceder aqui si hay autoindex en el dir padre?
	}

	inFile.open(filePath);
	if (!inFile.is_open())
	{
		error("Requested file not found");
		return (this->errorResponse(404));
	}
	fileContentStream << inFile.rdbuf();
	fileContent = fileContentStream.str();
	fileSize = fileContent.size();
	inFile.close();

	this->_responseStr = "HTTP/1.1 200 OK\r\n";
	this->_responseStr += "Content-Type: ";
	if (filePath.rfind(".") != std::string::npos)
	{
		fileExtension = filePath.substr(filePath.rfind("."), filePath.size());
		if (DEBUG)
			std::cout << GREY << "[DEBUG: ...file extension: " << fileExtension << "]" << DEF_COL << std::endl;
		std::map<std::string, std::string>::iterator it = this->_mime.find(fileExtension);
		if (it != this->_mime.end())
		{
			//if (fileExtension == ".json")
			//	this->initJson(filePath);
			this->_responseStr += (this->_mime[fileExtension] + "\r\n");
		}
		else
			this->_responseStr += "text/plain\r\n";
	}
	else
		this->_responseStr += "text/plain\r\n";
	this->_responseStr += "Content-Length: ";
	this->_responseStr += intToString(fileSize);
	this->_responseStr += "\r\n\r\n";
	this->_responseStr += fileContent;
	debug("...file response built");
	return (this->_responseStr);
}
