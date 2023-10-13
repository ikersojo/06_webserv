/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 09:16:19 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/13 16:33:00 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ResponseBuilder.hpp"

bool	ResponseBuilder::_shutdownRequested = false;


void	ResponseBuilder::SignalHandler(int signal)
{
	if (signal == SIGINT || signal == SIGTERM)
		_shutdownRequested = true;
}


ResponseBuilder::ResponseBuilder(Config * config, int configIndex, std::string requestStr)
{
	this->_config = config;
	this->_configIndex = configIndex;
	this->_ok = true;
	this->_requestStr = requestStr;
	this->_responseStr = "";
	debug("ResponseBuilder Object Created");
}


ResponseBuilder::~ResponseBuilder(void)
{
	debug("ResponseBuilder Object Destroyed");
}


std::string		ResponseBuilder::computeRegularResponse(void)
{
	this->assessRequest();
	if (!this->_ok)
		return (this->errorResponse(400));
	if (this->_requestParams[2] != "HTTP/1.1")
	{
		error("webserv only work with HTTP/1.1 requests");
		return (this->errorResponse(505));
	}
	if (this->_ok && !this->_config->isValidRequest(this->_configIndex, this->_requestParams[1]))
	{
		error("Not valid url");
		return (this->errorResponse(404));
	}

	if (this->_ok && this->_requestParams[0] == "GET" && this->_config->isGET(this->_configIndex, this->_requestParams[1]))
		return (this->getResponse());
	// else if (this->_ok && this->_requestParams[0] == "POST")
	// 	return (this->postResponse());
	// else if (this->_ok && this->_requestParams[0] == "DELETE")
	// 	return (this->deleteResponse());
	else if (this->_ok)
		return (this->errorResponse(405));
	else
		return (this->errorResponse(500));
}


void	ResponseBuilder::assessRequest(void)
{
	debug("Postprocessing request...");
	std::istringstream	iss(this->_requestStr);
	std::string			item;

	while (iss >> item)
		this->_requestParams.push_back(item);
	if (this->_requestParams.size() < 3)
	{
		error("HTTP request header is not correct (too few arguments)");
		this->_ok = false;
		return ;
	}
	int i = -1;
	while (++i < 3)
	{
		if (this->_requestParams[i].empty())
		{
			error("HTTP request header content is not correct (empty arguments)");
			this->_ok = false;
			return ;
		}
		if (DEBUG)
			std::cout << GREY << "[DEBUG: ...param " << i << ": "
					<< this->_requestParams[i] << "]" << DEF_COL << std::endl;
	}
	debug("...request assessed");
}


std::string	ResponseBuilder::getResponse(void)
{
	if (this->_config->isRedir(this->_configIndex, this->_requestParams[1]))
	{
		debug("...redirection requested");
		return (this->redirResponse());
	}
	else if (this->_config->isAutoIndex(this->_configIndex, this->_requestParams[1]))
	{
		debug("autoindex requested");
		return (this->aiResponse());
	}
	else
	{
		debug("...file requested");
		return (this->fileResponse());
	}
}


std::string	ResponseBuilder::errorResponse(int code) // TODO: Add files
{
	if (code == 400)
		this->_responseStr = "HTTP/1.1 400 Bad Request\r\n";
	else if (code == 404)
		this->_responseStr = "HTTP/1.1 404 Not Found\r\n";
	else if (code == 405)
		this->_responseStr = "HTTP/1.1 405 Method Not Allowed\r\n";
	else if (code == 500)
		this->_responseStr = "HTTP/1.1 500 Internal Server Error\r\n";
	else if (code == 505)
		this->_responseStr = "HTTP/1.1 505 HTTP Version Not Supported\r\n";
	else
		this->_responseStr = "HTTP/1.1 XXX Unkown Error\r\n";

	this->_responseStr += "Content-Type: text/html\r\n\r\n";
	this->_responseStr += "<html><head><title>Error " + intToString(code) + "</title></head><body>";
	this->_responseStr +=  "<h1>Error " + intToString(code) + "</h1>";
	debug("...error response built");
	return (this->_responseStr);
}


std::string	ResponseBuilder::redirResponse(void)
{
	this->_responseStr = "HTTP/1.1 308 Permanent Redirect\r\n";
	this->_responseStr += "Location: ";
	this->_responseStr += this->_config->getFile(this->_configIndex, this->_requestParams[1]);
	this->_responseStr += "\r\n\r\n";
	debug("...redirect response built");
	return (this->_responseStr);
}


std::string	ResponseBuilder::aiResponse(void)
{
	std::string	requestedDir = this->_config->getFile(this->_configIndex, this->_requestParams[1]);
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
			this->_config->setAIFile(this->_configIndex, url, requestedDir, entry->d_name);
		}
		this->_responseStr += "</ul></body></html>";
		closedir(dir);
		debug("...files in directory listed and added to config");
		if (DEBUG)
			this->_config->printConfig();

		if (this->_config->isValidRequest(this->_configIndex, this->_requestParams[1] + "/index.html"))
		{
			this->_requestParams[1] = this->_requestParams[1] + "/index.html";
			debug("...index file found");
			return (this->fileResponse());
		}
		debug("...auto index response built");
		return (this->_responseStr);
	}
	else
	{
		error("Directory not found");
		return (this->errorResponse(500));
	}
}


// 		"HTTP/1.1 200 OK\r\n"
// 		"Content-Type: text/plain\r\n"
// 		"Content-Length: 23\r\n"
// 		"\r\n"
// 		"Hello, Andoni and John!";

std::string	ResponseBuilder::fileResponse(void)
{
	std::string			filepath;
	std::ifstream		inFile;
	std::ostringstream	fileContentStream;
	std::string			fileContent;
	int					fileSize;

	filepath = this->_config->getFile(this->_configIndex, this->_requestParams[1]);
	if (DEBUG)
		std::cout << GREY << "[DEBUG: ...requested file: " << filepath << "]" << DEF_COL << std::endl;

	inFile.open(filepath);
	if (!inFile.is_open())
	{
		error("Requested file not found");
		return (this->errorResponse(404));
	}
    fileContentStream << inFile.rdbuf();
    fileContent = fileContentStream.str();
	fileSize = fileContent.size();

	this->_responseStr = "HTTP/1.1 200 OK\r\n";
	if (filepath.find(".html") != std::string::npos)
		this->_responseStr += "Content-Type: text/html\r\n";
	else if (this->_requestParams[1] == "/favicon.ico" || this->_requestParams[1] == "/image" || this->_requestParams[1].find(".jpg") != std::string::npos || this->_requestParams[1].find(".png") != std::string::npos)
		this->_responseStr += "Content-Type: image/vnd.microsoft.icon\r\n";
	else
		this->_responseStr += "Content-Type: text/plain\r\n";
	this->_responseStr += "Content-Length: ";
	this->_responseStr += intToString(fileSize);
	this->_responseStr += "\r\n\r\n";
	this->_responseStr += fileContent;
	inFile.close();
	debug("...response built");
	return (this->_responseStr);
}
