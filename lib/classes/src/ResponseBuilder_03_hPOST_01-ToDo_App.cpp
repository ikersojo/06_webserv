/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder_03_hPOST_01-ToDo_App.cpp           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 11:18:03 by isojo-go          #+#    #+#             */
/*   Updated: 2023/11/03 13:01:43 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ResponseBuilder.hpp"

std::string	extractTask(const std::string& str)
{
	size_t quoteStart = 0;
	size_t quoteEnd = str.rfind("\"");
	size_t count = 0;

	while (count < 3)
	{
		quoteStart = str.find("\"", quoteStart);
		++count;
		if (count < 3)
			quoteStart++;
	}
	std::string extractedContent = str.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
	size_t i = 0;
	while(i < extractedContent.size())
	{
		if (extractedContent[i] == ' ')
			extractedContent[i] = '_';
		i++;
	}
	return (extractedContent);
}

std::string	ResponseBuilder::addToList(void)
{
	// Locate DB:
	std::string filePath = _config->getActualPath(_configIndex, this->_requestParams[1]);
	if (DEBUG)
		std::cout << GREY << "[DEBUG: ...db json file identified: " << filePath << "]" <<DEF_COL << std::endl;

	// Add task to DB:
	size_t lastBlankLinePos = this->_requestStr.rfind("\r\n\r\n");
	if (lastBlankLinePos != std::string::npos)
	{
		std::string lastLine = this->_requestStr.substr(lastBlankLinePos + 4);
		std::string	task = extractTask(lastLine);
		if (DEBUG)
			std::cout << GREY << "[DEBUG: ...extracted task: " << task << " ]" << DEF_COL << std::endl;
		if (task != "")
			this->writeToJsonFile("\"" + task + "\"", filePath);
	}
	else
		error("Task to be added not found");

	// Get Json File Content
	std::string fileContent = this->readFromJsonFile(filePath);
	if (fileContent == "")
		return (this->errorResponse(404));

	// Build response
	this->_responseStr  = "HTTP/1.1 200 OK\r\n";
	this->_responseStr += "Content-Type: application/json\r\n";
	this->_responseStr += "Content-Length: ";
	this->_responseStr += intToString(fileContent.size());
	this->_responseStr += "\r\n\r\n";
	this->_responseStr += fileContent;

	debug("...post addToList completed and response built");
	return (this->_responseStr);
}

std::string	ResponseBuilder::readFromJsonFile(std::string filePath)
{
	std::ifstream		inFile;
	std::ostringstream	fileContentStream;
	std::string			fileContent;

	// Read the existing content into a string
	inFile.open(filePath);
	if (!inFile.is_open())
	{
		error("JSON file not found");
		return ("");
	}
	fileContentStream << inFile.rdbuf();
	inFile.close();
	fileContent = fileContentStream.str();
	return (fileContent);
}

void	ResponseBuilder::writeToJsonFile(std::string task, std::string filePath)
{
	// Get Json File Content
	std::string fileContent = this->readFromJsonFile(filePath);
	if (fileContent == "")
		return ;

	// Find the position of the first "[" character
	size_t position = fileContent.find_first_of('[');
	if (position == std::string::npos)
	{
		error("Invalid JSON file format");
		return;
	}

	// Insert the new task at the position after the first "[" character
	if (countOccurrences(fileContent, '\"') > 0)
		fileContent.insert(position + 1,  task + ", ");
	else
		fileContent.insert(position + 1, task);

	// Open the file in output mode to write the updated content
	std::ofstream outFile(filePath);
	if (!outFile.is_open())
	{
		error("Unable to open the JSON file for writing");
		return;
	}
	outFile << fileContent;
	outFile.close();

	debug("...task included in json file");
}
