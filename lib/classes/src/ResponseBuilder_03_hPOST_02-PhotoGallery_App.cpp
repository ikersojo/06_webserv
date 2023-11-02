/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder_03_hPOST_02-PhotoGallery_Ap        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdasilva <jdasilva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 16:22:46 by jdasilva          #+#    #+#             */
/*   Updated: 2023/11/02 16:25:47 by jdasilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ResponseBuilder.hpp"

std::string		extractFileName(std::string request)
{
	std::string	res;

	res = request.substr(request.find("filename=") + 10, request.size());
	res = res.substr(0, res.find("\""));
	return (res);
}

std::string		ResponseBuilder::uploadPhoto(void)
{
	// Locate DB:
	std::string dbFilePath = _config->getActualPath(_configIndex, this->_requestParams[1]);
	if (DEBUG)
		std::cout << GREY << "[DEBUG: ...db json file identified: " << dbFilePath << "]" <<DEF_COL << std::endl;

	std::string root = _config->getRoot(_configIndex, this->_requestParams[1]);
	if (DEBUG)
		std::cout << GREY << "[DEBUG: ...root dir: " << root << "]" <<DEF_COL << std::endl;

	std::string name = extractFileName(this->_requestStr);
	std::string fileName = root + "/photos/" + name;
	if (DEBUG)
		std::cout << GREY << "[DEBUG: ...file name: " << fileName << "]" <<DEF_COL << std::endl;

	// Add task to DB:
	size_t lastBlankLinePos = this->_requestStr.rfind("\r\n\r\n");
	if (lastBlankLinePos != std::string::npos)
	{
		std::string lastLine = this->_requestStr.substr(lastBlankLinePos + 4);
		if (DEBUG)
			std::cout << GREY << "[DEBUG: ...extracted info: " << this->_requestStr << " ]" << DEF_COL << std::endl;
		std::string	photo = "{\"url\": \"gallerySite/photos/" + name + "\", \"name\": \"" + name + "\"}";
		this->writeToJsonFile(photo, dbFilePath);

		// Open the file in output mode to write the updated content
		std::ofstream outFile(fileName);
		if (!outFile.is_open())
		{
			error("Unable to open the file");
			return(this->errorResponse(500));
		}
		outFile.write(lastLine.c_str(), lastLine.size());
		outFile.close();
	}
	else
		error("Info not found");

	// Get Json File Content
	std::string fileContent = this->readFromJsonFile(dbFilePath);
	if (fileContent == "")
		return (this->errorResponse(404));

	// Build response
	this->_responseStr  = "HTTP/1.1 200 OK\r\n";
	this->_responseStr += "Content-Type: application/json\r\n";
	this->_responseStr += "Content-Length: ";
	this->_responseStr += intToString(fileContent.size());
	this->_responseStr += "\r\n\r\n";
	this->_responseStr += fileContent;

	debug("...post uploadPhoto completed and response built");
	return (this->_responseStr);
}
