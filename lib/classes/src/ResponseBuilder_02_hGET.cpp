/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder_02_hGET.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarrien- <aarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 09:16:19 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/24 17:56:56 by aarrien-         ###   ########.fr       */
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
	std::string	path = this->_requestParams[1];

	if (_config->isAutoIndex(_configIndex, path)) {
		std::cout << (_config->getRoot(_configIndex, path) + (path.substr(_config->getNearestLocation(_configIndex, path).size()))).c_str() << std::endl;
		DIR* dir = opendir((_config->getRoot(_configIndex, path) + (path.substr(_config->getNearestLocation(_configIndex, path).size()))).c_str());
		if (dir != NULL) {
			this->_responseStr = "HTTP/1.1 200 OK\r\n";
			this->_responseStr += "Content-Type: text/html\r\n\r\n";
			this->_responseStr += "<html><head><title>Index of " + path + "</title></head><body>";
			this->_responseStr +=  "<h1>Index of " + path + "</h1><ul>";

			struct dirent *	entry;
			while ((entry = readdir(dir)) != NULL)
				this->_responseStr += "<li><a href=\"" + path + "/" + entry->d_name + "\">" + entry->d_name + "</a></li>";
			this->_responseStr += "</ul></body></html>";
			closedir(dir);
			return (this->_responseStr);
		} else {
			return (this->fileResponse());
		}
	} else {
		error("Directory doesn't support autoindex");
		return (this->errorResponse(403));
	}
}


std::string	ResponseBuilder::fileResponse(void)
{
	std::string			filePath;
	std::ifstream		inFile;
	std::ostringstream	fileContentStream;
	std::string			fileContent;
	std::string			fileExtension;
	int					fileSize;

	std::string nl = _config->getNearestLocation(_configIndex, _requestParams[1]);
	std::string actualFile = _requestParams[1].substr(nl.size());
	std::string file = _config->getFile(_configIndex, nl);

	if (actualFile.size() == 0 && file.size() != 0)
		actualFile = "/" + file;
	filePath = _config->getRoot(_configIndex, nl) + actualFile;

	if (DEBUG)
		std::cout << GREY << "[DEBUG: ...requested file: " << filePath << "]" << DEF_COL << std::endl;

	struct stat statbuf;

	if (stat(filePath.c_str(), &statbuf) == 0 && !S_ISREG(statbuf.st_mode)) {
		error("Requested file is a directory");
		return (this->errorResponse(404));
	}

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

	std::cout << "file content [" << fileContent << "]\n"; //

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
