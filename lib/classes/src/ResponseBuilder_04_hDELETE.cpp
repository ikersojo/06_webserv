/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder_04_hDELETE.cpp                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarrien- <aarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 09:16:19 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/25 14:28:57 by aarrien-         ###   ########.fr       */
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

	std::string	task = this->_config->getActualPath(this->_configIndex,this->_requestParams[1]);
	size_t pos = task.rfind("/");
	std::string	filePath = task.substr(0, pos);
	int	index = std::atoi(task.substr(pos+1).c_str());

	if (DEBUG)
		std::cout << GREY << "[DEBUG: ...index task to be removed identified: " << index << "]" << DEF_COL << std::endl;

	this->removeFromList(index, filePath);

	return("HTTP/1.1 200 OK\r\n");
}


void	ResponseBuilder::removeFromList(int taskIndex, std::string filePath)
{
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
		//stringArray.insert(stringArray.begin(), temp);
	}

	// Iterate through the vector and print each string
	size_t i = stringArray.size() - 1;
	while(true)
	{
		if(i != (size_t)taskIndex)
			writeToJsonFile(stringArray[i], filePath);
		if (i == 0) break;
		i--;
	}
}
