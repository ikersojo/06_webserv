/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder_04_hDELETE.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 09:16:19 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/19 19:43:06 by isojo-go         ###   ########.fr       */
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
	if (this->_config->getHandleDELETE(this->_configIndex, this->_requestParams[1]) == "removeFromList")
	{
		std::string	task = this->_config->getFile(this->_configIndex, this->_requestParams[1]);
		if (DEBUG)
			std::cout << GREY << "[DEBUG: ...task to be removed identified: " << task << "]" << DEF_COL << std::endl;

		this->removeFromList(task);

		this->_requestParams[1] = this->_requestParams[1].substr(0, this->_requestParams[1].rfind("api"));
		if (DEBUG)
			std::cout << GREY << "[DEBUG: ...redirecting to " << this->_requestParams[1] << "]" << DEF_COL << std::endl;
		return(this->fileResponse());
	}
	else
		return(this->errorResponse(500));
}


void	ResponseBuilder::removeFromList(std::string task)
{
	std::string	filePath = this->_config->getFile(this->_configIndex, this->_requestParams[1].substr(0, this->_requestParams[1].rfind("/")));
	if (DEBUG)
		std::cout << GREY << "[DEBUG: ...JSON file: " << filePath << "]" << DEF_COL << std::endl;
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
		stringArray.push_back(temp);
	}

	// Iterate through the vector and print each string
	size_t i = 0;
	while(i < stringArray.size())
	{
		if(task != stringArray[i])
			writeToJsonFile(stringArray[i], filePath);
		i++;
	}
}
