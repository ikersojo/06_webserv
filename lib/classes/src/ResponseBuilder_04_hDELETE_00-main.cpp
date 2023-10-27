/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder_04_hDELETE_00-main.cpp             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 09:16:19 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/27 15:04:11 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ResponseBuilder.hpp"

std::string	ResponseBuilder::deleteResponse(void)
{
	if (!this->_config->isDELETE(this->_configIndex, this->_requestParams[1]))
	{
		error("DELETE is not allowed");
		return(this->errorResponse(405));
	}

	this->_handleDELETE = this->_config->getHandleDELETE(this->_configIndex, this->_requestParams[1]);
	if (this->_handleDELETE == "removeFromList")
	{
		debug("...removeFromList Built-in Functionality Requested");
		return (this->removeFromList());
	}
	// ... all additional functionality ...
	else
	{
		error("No DELETE functionality implemented");
		return(this->errorResponse(500));
	}
}
