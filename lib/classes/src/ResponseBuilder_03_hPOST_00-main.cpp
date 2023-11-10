/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder_03_hPOST_00-main.cpp               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarrien- <aarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 09:16:19 by isojo-go          #+#    #+#             */
/*   Updated: 2023/11/10 17:03:08 by aarrien-         ###   ########.fr       */
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
	else if (this->_handlePOST == "setCookie")
	{
		debug("...setCookie Built-in Functionality Requested");
		return (this->setCookie());
	}
	// ... all additional functionality ...
	else
	{
		error("No POST functionality implemented");
		return(this->errorResponse(500));
	}
}


void	ResponseBuilder::decodeChuncked(void)
{
	if (this->_requestStr.find("Transfer-Encoding: chunked") != std::string::npos)
	{
		debug("...chunked body identified.");
		size_t endOfHeaderPos = this->_requestStr.find("\r\n\r\n");
		if (endOfHeaderPos != std::string::npos)
		{
			std::string body = this->_requestStr.substr(endOfHeaderPos + 4);
			////
			if (DEBUG)
				std::cout << GREY << "[DEBUG: ...chuncked content: " << body << " ]" << DEF_COL << std::endl;
		}
		else
			error("Body is empty");
		}
}
