/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder_03_hPOST_00-main.cpp               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 09:16:19 by isojo-go          #+#    #+#             */
/*   Updated: 2023/11/10 15:50:29 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ResponseBuilder.hpp"

std::string	ResponseBuilder::postResponse(void)
{
	if (!this->_config->isPOST(this->_configIndex, this->_requestParams[1]))
	{
		error("POST is not allowed");
		return(this->errorResponse(405));
	}
	std::cout << this->_requestStr << std::endl;
	this->decodeChuncked();

	if (this->_config->isCgi(this->_configIndex, this->_requestParams[1]))
	{
		debug("...CGI requested");
		return (this->cgiPOSTResponse());
	}

	this->_handlePOST = this->_config->getHandlePOST(this->_configIndex, this->_requestParams[1]);
	if (this->_handlePOST == "addToList")
	{
		debug("...addToList Built-in Functionality Requested");
		return (this->addToList());
	}
	else if (this->_handlePOST == "uploadPhoto")
	{
		debug("...uploadPhoto Built-in Functionality Requested");
		return (this->uploadPhoto());
	}
	else if (this->_handlePOST == "dechunk")
	{
		debug("...deChunck Built-in Functionality Requested");
		return (this->deChunk());
	}
	// ... all additional functionality ...
	else
	{
		error("No POST functionality implemented");
		return(this->errorResponse(500));
	}
}


std::string	trimBody(std::string body)
{
	// pending...
	return (body);
}

void	ResponseBuilder::decodeChuncked(void)
{
	if (this->_requestStr.find("Transfer-Encoding: chunked") != std::string::npos)
	{
		debug("...chunked body identified.");
		size_t endOfHeaderPos = this->_requestStr.find("\r\n\r\n");
		std::string header = this->_requestStr.substr(0, endOfHeaderPos + 4);
		std::string body = this->_requestStr.substr(endOfHeaderPos + 4);
		body = trimBody(body);

		if (endOfHeaderPos != std::string::npos)
		{
			if (DEBUG)
				std::cout << GREY << "[DEBUG: ...chuncked content: \n" << this->_requestStr << "\n]" << DEF_COL << std::endl;
			this->_requestStr = header + body;
			if (DEBUG)
				std::cout << GREY << "[DEBUG: ...de-chuncked content: \n" << this->_requestStr << "\n]" << DEF_COL << std::endl;
		}
		else
			error("Body is empty");
		}
}
