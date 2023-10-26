/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder_03_hPOST.cpp                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 09:16:19 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/26 13:52:37 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ResponseBuilder.hpp"

void	ResponseBuilder::writeToJsonFile(std::string task, std::string filePath)
{
	std::ifstream		inFile;

	if (DEBUG)
		std::cout << GREY << "[DEBUG: ...JSON file to include task: " << filePath << "]" << DEF_COL << std::endl;

	// Open the file in input mode to read the existing content
	inFile.open(filePath);
	if (!inFile.is_open())
	{
		error("JSON file not found");
		return ;
	}

	// Read the existing content into a string
	std::ostringstream	fileContentStream;
	fileContentStream << inFile.rdbuf();
	std::string fileContent = fileContentStream.str();

	// Close the input file
	inFile.close();

	// Find the position of the first "[" character
	size_t position = fileContent.find_first_of('[');
	if (position == std::string::npos)
	{
		error("Invalid JSON file format");
		return;
	}

	// Insert the new task at the position after the first "[" character
	if (countOccurrences(fileContent, '\"') > 0)
		fileContent.insert(position + 1, "\"" + task + "\", ");
	else
		fileContent.insert(position + 1, "\"" + task + "\"");

	// Open the file in output mode to write the updated content
	std::ofstream outFile(filePath);
	if (!outFile.is_open())
	{
		error("Unable to open the JSON file for writing");
		return;
	}

	// Write the updated content back to the file
	outFile << fileContent;

	// Close the output file
	outFile.close();

	debug("...task included in json file");
}


void	ResponseBuilder::addToList(std::string filePath)
{
	// Find the last line (after a blank line)
	size_t lastBlankLinePos = this->_requestStr.rfind("\r\n\r\n");
	if (lastBlankLinePos != std::string::npos)
	{
		std::string lastLine = this->_requestStr.substr(lastBlankLinePos + 4);
		std::string	task = extractTask(lastLine);
		if (DEBUG)
			std::cout << GREY << "[DEBUG: ...extracted task: " << task << " ]" << DEF_COL << std::endl;

		this->writeToJsonFile(task, filePath);

		//std::string url = this->_requestParams[1] + "/" + task;
		//this->_config->setDeletePath(this->_configIndex, url, task);
	}
}


std::string	ResponseBuilder::postResponse(void)
{
	if (!this->_config->isPOST(this->_configIndex, this->_requestParams[1]))
	{
		error("POST is not allowed");
		return(this->errorResponse(405));
	}

	std::string			filePath;
	std::ifstream		inFile;
	std::ostringstream	fileContentStream;
	std::string			fileContent;
	int					fileSize;

	filePath = _config->getActualPath(_configIndex, this->_requestParams[1]);
	if (DEBUG)
		std::cout << GREY << "[DEBUG: ...requested file: " << filePath << "]" <<DEF_COL << std::endl;

	this->addToList(filePath);
	//this->initJson(filePath);

	inFile.open(filePath);
	if (!inFile.is_open())
	{
		error("Requested file not found");
		return (this->errorResponse(404));
	}

	fileContentStream << inFile.rdbuf();
	fileContent = fileContentStream.str();
	fileSize = fileContent.size();

	this->_responseStr  = "HTTP/1.1 200 OK\r\n";
	this->_responseStr += "Content-Type: application/json\r\n";
	this->_responseStr += "Content-Length: ";
	this->_responseStr += intToString(fileSize);
	this->_responseStr += "\r\n\r\n";
	this->_responseStr += fileContent;
	inFile.close();

	debug("...post response built");
	return (this->_responseStr);
}


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





void	ResponseBuilder::clearJsonFile(std::string filePath)
{
	std::ofstream outFile(filePath);
	if (!outFile.is_open())
	{
		error("Unable to open the JSON file for writing");
		return;
	}
	outFile << "[]";
	outFile.close();
}


void	ResponseBuilder::initJson(std::string filePath)
{
	if (DEBUG)
		std::cout << GREY << "[DEBUG: ...Initializing JSON file: " << filePath << "]" << DEF_COL << std::endl;
	std::ifstream file(filePath);

	if (!file.is_open())
	{
		error("Error opening JSON file");
		return ;
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string fileContent = buffer.str();
	file.close();
	this->clearJsonFile(filePath);

	// Find the start and end of the array
	size_t start = fileContent.find("[");
	size_t end = fileContent.rfind("]");

	// Extract the content of the array
	std::string arrayContent = fileContent.substr(start + 1, end - start - 1);

	// Initialize a string stream to parse the array content
	std::stringstream arrayStream(arrayContent);

	// Vector to store individual strings
	std::vector<std::string> stringArray;

	// Temporary variable to store each string
	std::string temp;

	// Iterate through the array content and extract individual strings
	while (std::getline(arrayStream, temp, ','))
	{
		// Remove leading and trailing whitespaces
		size_t firstNotSpace = temp.find_first_not_of(" \t\n\r\"");
		size_t lastNotSpace = temp.find_last_not_of(" \t\n\r\"");
		temp = temp.substr(firstNotSpace, lastNotSpace - firstNotSpace + 1);

		// Add the string to the vector
		//stringArray.insert(stringArray.begin(), temp);
		stringArray.push_back(temp);
	}

	std::cout << "file content = " << fileContent << "\n";
	// Iterate through the vector and print each string
	size_t i = 0;
	while(i < stringArray.size())
	{
		std::cout << "[" << i << "]" << stringArray[i] << "\n";

		writeToJsonFile(stringArray[i], filePath);
		//std::string url = this->_requestParams[1] + "/" + stringArray[i];
		//this->_config->setDeletePath(this->_configIndex, url, stringArray[i]);
		i++;
	}
}
