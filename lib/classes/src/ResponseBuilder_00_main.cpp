/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder_00_main.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 09:16:19 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/19 23:37:07 by isojo-go         ###   ########.fr       */
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

	this->_mime[".html"] = "text/html";
	this->_mime[".htm"] = "text/html";
	this->_mime[".css"] = "text/css";
	this->_mime[".ico"] = "image/x-icon";
	this->_mime[".avi"] = "video/x-msvideo";
	this->_mime[".bmp"] = "image/bmp";
	this->_mime[".doc"] = "application/msword";
	this->_mime[".gif"] = "image/gif";
	this->_mime[".gz"] = "application/x-gzip";
	this->_mime[".ico"] = "image/x-icon";
	this->_mime[".jpg"] = "image/jpeg";
	this->_mime[".jpeg"] = "image/jpeg";
	this->_mime[".png"] = "image/png";
	this->_mime[".txt"] = "text/plain";
	this->_mime[".mp3"] = "audio/mp3";
	this->_mime[".pdf"] = "application/pdf";
	this->_mime[".json"] = "application/json";
	this->_mime["default"] = "text/html";

	debug("ResponseBuilder Object Created");
}


ResponseBuilder::~ResponseBuilder(void)
{
	debug("ResponseBuilder Object Destroyed");
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


std::string		ResponseBuilder::computeResponse(void)
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
		this->checkLocation(this->_requestParams[1]);
		if (!this->_ok)
		{
			error("Not valid url");
			return (this->errorResponse(404));
		}
	}

	// Checks ok, build response based on the request type:
	if (this->_ok && this->_requestParams[0] == "GET")
		return (this->getResponse());
	else if (this->_ok && this->_requestParams[0] == "POST")
		return (this->postResponse());
	else if (this->_ok && this->_requestParams[0] == "DELETE")
		return (this->deleteResponse());
	else if (this->_ok)
		return (this->errorResponse(405));
	else
		return (this->errorResponse(500));
}


void	ResponseBuilder::checkLocation(std::string trimmedURL)
{
	std::string	requestedDir = "";
	std::string	remainder = "";
	bool		found = false;

	while (trimmedURL.find("/") != std::string::npos)
	{
		std::cout << "trimmedURL: " << trimmedURL << std::endl;
		std::cout << "requestedDir: " << requestedDir << "\n------\n" << std::endl;
		
		if (trimmedURL[trimmedURL.size() - 1] == '/')
			trimmedURL = trimmedURL.substr(0, trimmedURL.rfind("/"));
		remainder = trimmedURL.substr(trimmedURL.rfind("/") + 1, trimmedURL.size());
		trimmedURL = trimmedURL.substr(0, trimmedURL.rfind("/") + 1);
		if (this->_config->isValidRequest(this->_configIndex, trimmedURL))
			requestedDir = this->_config->getRootDir(this->_configIndex, trimmedURL);

		std::cout << "trimmedURL: " << trimmedURL << std::endl;
		std::cout << "requestedDir: " << requestedDir << "\n------\n" << std::endl;


		DIR* dir = opendir(requestedDir.c_str());
		if (dir != NULL)
		{
			struct dirent *	entry;
			while ((entry = readdir(dir)) != NULL)
			{
				if (entry->d_type == DT_DIR)
				{
					std::string url = trimmedURL + remainder;
					this->_config->setAIDir(this->_configIndex, url, requestedDir + remainder);
					found = true;
				}
				else if (entry->d_type == DT_REG)
				{
					std::string url = trimmedURL + entry->d_name;
					this->_config->setAIFile(this->_configIndex, url, requestedDir, entry->d_name);
					found = true;
				}
			}
			closedir(dir);
			if (DEBUG)
				this->_config->printConfig();
		}
		if (found)
			break;
	}
	if (found)
	{
		if (!this->_config->isValidRequest(this->_configIndex, this->_requestParams[1]))
			this->checkLocation(this->_requestParams[1]);
	}
	else
		this->_ok = false;
}



// std::string	ResponseBuilder::aiResponse(void)
// {
// 	std::string	requestedDir = this->_config->getRootDir(this->_configIndex, this->_requestParams[1]);
// 	std::string	path = this->_requestParams[1];

// 	DIR* dir = opendir(requestedDir.c_str());
// 	if (dir != NULL)
// 	{
// 		this->_responseStr = "HTTP/1.1 200 OK\r\n";
// 		this->_responseStr += "Content-Type: text/html\r\n\r\n";
// 		this->_responseStr += "<html><head><title>Index of " + path + "</title></head><body>";
// 		this->_responseStr +=  "<h1>Index of " + path + "</h1><ul>";

// 		struct dirent *	entry;
// 		while ((entry = readdir(dir)) != NULL)
// 		{
// 			std::string url = this->_requestParams[1] + "/" + entry->d_name;
// 			this->_responseStr += "<li><a href=\"" + this->_requestParams[1] + "/" + entry->d_name + "\">" + entry->d_name + "</a></li>";
// 			this->_config->setAIFile(this->_configIndex, url, requestedDir, entry->d_name);
// 		}
// 		this->_responseStr += "</ul></body></html>";
// 		closedir(dir);
// 		debug("...files in directory listed and added to config");
// 		if (DEBUG)
// 			this->_config->printConfig();

// 		if (this->_config->isValidRequest(this->_configIndex, this->_requestParams[1] + "/index.html"))
// 		{
// 			this->_requestParams[1] = this->_requestParams[1] + "/index.html";
// 			debug("...index file found");
// 			return (this->fileResponse());
// 		}
// 		debug("...auto index response built");
// 		return (this->_responseStr);
// 	}
// 	else
// 	{
// 		error("Directory not found");
// 		return (this->errorResponse(500));
// 	}
// }
