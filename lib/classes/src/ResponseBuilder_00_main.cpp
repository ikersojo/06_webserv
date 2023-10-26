/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder_00_main.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarrien- <aarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 09:16:19 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/26 15:10:00 by aarrien-         ###   ########.fr       */
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
	while (this->_ok && ++i < 3)
	{
		if (this->_requestParams[i].empty())
		{
			error("HTTP request header content is not correct (empty arguments)");
			this->_ok = false;
			return ;
		}
		else if (i == 1 && this->_requestParams[i].size() > 1 && this->_requestParams[i][this->_requestParams[i].size() - 1] == '/')
			this->_requestParams[i] = this->_requestParams[i].substr(0, this->_requestParams[i].size() - 2);
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

	// Checks ok, build response based on the request type:
	if (this->_ok && this->_requestParams[0] == "GET")
	{
		debug("...GET request identified");
		return (this->getResponse());
	}
	else if (this->_ok && this->_requestParams[0] == "POST")
	{
		debug("...POST request identified");
		return (this->postResponse());
	}
	else if (this->_ok && this->_requestParams[0] == "DELETE")
	{
		debug("...DELETE request identified");
		return (this->deleteResponse());
	}
	else if (this->_ok)
	{
		error("Request OK, but method not implemented");
		return (this->errorResponse(405));
	}
	else
	{
		error("Request is not OK");
		return (this->errorResponse(500));
	}
}
