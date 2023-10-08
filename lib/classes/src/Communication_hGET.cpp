/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Communication_hGET.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/02 08:41:57 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/08 22:47:09 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Communication.hpp"

void	Communication::handleGetRequest(void)
{
	if (!this->_config->isGET(this->_location, this->_requestParams[1]))
	{
		error("GET is not allowed for this location");
		this->_ok = false;
		return ;
	}

	if (this->_config->isRedir(this->_location, this->_requestParams[1]))
	{
		debug("...redirection requested");
		this->buildRedirResponse();
	}
	else if (this->_config->isCgi(this->_location, this->_requestParams[1]))
	{
		debug("CGI run requested");
		error("not implemented yet"); // TODO
	}
	else if (this->_config->isAutoIndex(this->_location, this->_requestParams[1]))
	{
		debug("autoindex requested");
		this->buildAutoIndexResponse();
	}
	else
	{
		debug("...file requested");
		this->buildFileResponse();
	}
}


// reponse header example:
// 		"HTTP/1.1 200 OK\r\n"
// 		"Content-Type: text/plain\r\n"
// 		"Content-Length: 23\r\n"
// 		"\r\n"
// 		"Hello, Andoni and John!";

void	Communication::buildFileResponse(void)
{
	std::string			filepath;
	std::ifstream		inFile;
	std::ostringstream	fileContentStream;
	std::string			fileContent;
	int					fileSize;

	filepath = this->_config->getFile(this->_location, this->_requestParams[1]);
	if (DEBUG)
		std::cout << GREY << "[DEBUG: ...file: " << filepath << "]" << DEF_COL << std::endl;
	inFile.open(filepath);
	if (!inFile.is_open())
	{
		error("file not found"); // TO BE REPLACED BY 404
		this->_ok = false;
		return ;
	}
    fileContentStream << inFile.rdbuf();
    fileContent = fileContentStream.str();
	fileSize = fileContent.size();

	this->_responseStr = "HTTP/1.1 200 OK\r\n";
	if (this->_requestParams[1] == "/")
		this->_responseStr += "Content-Type: text/html\r\n";
	else if (this->_requestParams[1] == "/favicon.ico" || this->_requestParams[1] == "/image")
		this->_responseStr += "Content-Type: image/vnd.microsoft.icon\r\n";
	else
		this->_responseStr += "Content-Type: text/plain\r\n";
	this->_responseStr += "Content-Length: ";
	this->_responseStr += intToString(fileSize);
	this->_responseStr += "\r\n\r\n";
	this->_responseStr += fileContent;
	inFile.close();
	debug("...response built");
}


// HTTP/1.1 301 Moved Permanently
// Location: /new-url

void	Communication::buildRedirResponse(void)
{
	this->_responseStr = "HTTP/1.1 301 Moved Permanently\r\n";
	this->_responseStr += "Location: ";
	this->_responseStr += this->_config->getFile(this->_location, this->_requestParams[1]);
	this->_responseStr += "\r\n\r\n";
	debug("...response built");
}


void	Communication::buildAutoIndexResponse(void)
{
	std::string	requestedDir = this->_config->getFile(this->_location, this->_requestParams[1]);
	std::string	path = this->_requestParams[1];
	DIR* dir = opendir(requestedDir.c_str());
	if (dir != NULL)
	{
		this->_responseStr = "HTTP/1.1 200 OK\r\n";
		this->_responseStr += "Content-Type: text/html\r\n\r\n";
		this->_responseStr += "<html><head><title>Index of " + path + "</title></head><body>";
		this->_responseStr +=  "<h1>Index of " + path + "</h1><ul>";

		struct dirent *	entry;
		while ((entry = readdir(dir)) != NULL)
		{
			std::string url = this->_requestParams[1] + "/" + entry->d_name;
			this->_responseStr += "<li><a href=\"" + this->_requestParams[1] + "/" + entry->d_name + "\">" + entry->d_name + "</a></li>";
			this->_config->setAIFile(this->_location, url, requestedDir, entry->d_name);
		}
		this->_responseStr += "</ul></body></html>";
		closedir(dir);
	}
	else
		error("Directory not found");
	debug("...additional locations added to config");
	if (DEBUG)
		this->_config->printConfig();

	debug("...response built");
}
