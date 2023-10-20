/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder_01_hERROR.cpp                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 09:16:19 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/19 18:36:09 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ResponseBuilder.hpp"

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
