/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder_02_hGET.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 09:16:19 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/22 23:17:07 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ResponseBuilder.hpp"


std::string	ResponseBuilder::getResponse(void)
{
	if (!this->_config->isGET(this->_configIndex, this->_requestParams[1]))
	{
		error("GET is not allowed");
		return(this->errorResponse(405));
	}
	if (this->_config->isRedir(this->_configIndex, this->_requestParams[1]))
	{
		debug("...redirection requested");
		return (this->redirResponse());
	}
	else if (this->_config->isCgi(this->_configIndex, this->_requestParams[1]))
	{
		debug("..CGI requested");
		return (this->cgiGETResponse());
	}
	else if (this->_config->isAutoIndex(this->_configIndex, this->_requestParams[1]))
	{
		debug("...autoindex requested");
		return (this->aiResponse());
	}
	else
	{
		debug("...file requested");
		return (this->fileResponse());
	}
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
	std::string	requestedDir;
	int i = -1;
	while (++i < 2)
	{
		if (i == 0)
			requestedDir = this->_config->getFullPath(this->_configIndex, this->_requestParams[1]);
		else
			requestedDir = this->_config->getRoot(this->_configIndex, this->_requestParams[1]);
		std::string	path = this->_requestParams[1];

		if (DEBUG)
			std::cout << GREY << "[DEBUG: ...openning directory: " << requestedDir << "]" << DEF_COL << std::endl;

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
				std::string name(entry->d_name);
				if (name != "." && name != "..")
				{
					if (entry->d_type == DT_REG)
					{
						if (DEBUG)
							std::cout << GREY << "[DEBUG: ...file found: " << name << "]" << DEF_COL << std::endl;
						std::string url = "http://" + this->_config->getAddress(this->_configIndex) + ":" + intToString(this->_config->getPort(this->_configIndex)) + this->_requestParams[1] + name;
						this->_responseStr += "<li><a href=\"" + url + "\">" + name + "</a></li>";
						this->_config->setAIFile(this->_configIndex, this->_requestParams[1] + name, requestedDir, name);
					}
				}
			}
			this->_responseStr += "</ul></body></html>";
			closedir(dir);
			debug("...files in directory listed and added to config");
		}
		else
			continue;;


		std::string root = this->_requestParams[1];
		if (root[root.size()] != '/')
			root += "/";
		if (this->_config->isValidRequest(this->_configIndex, this->_requestParams[1] + "index.html"))
		{
			this->_requestParams[1] = this->_requestParams[1] + "index.html";
			debug("...index file found");
			return (this->fileResponse());
		}
		debug("...auto index response built");
		return (this->_responseStr);
	}
	error("Directory not found");
	return (this->errorResponse(500));
}


std::string	ResponseBuilder::fileResponse(void)
{
	std::string			filePath;
	std::ifstream		inFile;
	std::ostringstream	fileContentStream;
	std::string			fileContent;
	std::string			fileExtension;
	int					fileSize;

	filePath = this->_config->getFullPath(this->_configIndex, this->_requestParams[1]);
	if (DEBUG)
		std::cout << GREY << "[DEBUG: ...requested file: " << filePath << "]" << DEF_COL << std::endl;

	inFile.open(filePath);
	if (!inFile.is_open())
	{
		error("Requested file not found");
		return (this->errorResponse(404));
	}
	fileContentStream << inFile.rdbuf();
	fileContent = fileContentStream.str();
	fileSize = fileContent.size();
	inFile.close();

	this->_responseStr = "HTTP/1.1 200 OK\r\n";

	this->_responseStr += "Content-Type: ";
	if (filePath.rfind("."))
	{
		fileExtension = filePath.substr(filePath.rfind("."), filePath.size());
		if (DEBUG)
			std::cout << GREY << "[DEBUG: ...file extension: " << fileExtension << "]" << DEF_COL << std::endl;

		std::map<std::string, std::string>::iterator it = this->_mime.find(fileExtension);
		if (it != this->_mime.end())
		{
			if (fileExtension == ".json")
				this->initJson(filePath);
			this->_responseStr += this->_mime[fileExtension];
			this->_responseStr += "\r\n";
		}
		else
			this->_responseStr += "text/plain\r\n";
	}
	else
		this->_responseStr += "text/plain\r\n";

	this->_responseStr += "Content-Length: ";
	this->_responseStr += intToString(fileSize);
	this->_responseStr += "\r\n\r\n";
	this->_responseStr += fileContent;
	debug("...file response built");
	return (this->_responseStr);
}
