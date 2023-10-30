/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder_03_hPOST_00-main.cpp               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 09:16:19 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/30 10:39:46 by isojo-go         ###   ########.fr       */
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
	// ... all additional functionality ...
	else
	{
		error("No POST functionality implemented");
		return(this->errorResponse(500));
	}
}
