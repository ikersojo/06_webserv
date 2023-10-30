/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder_04_hDELETE_00-main.cpp             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdasilva <jdasilva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 09:16:19 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/30 18:08:52 by jdasilva         ###   ########.fr       */
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
	else if (this->_handleDELETE == "deletePhoto")
	{
		debug("... deletePhoto  Built-in Functionality Requested");
		return (this->deletePhoto());
	}
	// ... all additional functionality ...
	else
	{
		error("No DELETE functionality implemented");
		return(this->errorResponse(500));
	}
}
