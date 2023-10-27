/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder_04_hDELETE_01-ToDo_App.cpp         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/27 11:35:51 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/27 12:25:33 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ResponseBuilder.hpp"

std::string ResponseBuilder::removeFromList(void)
{
	// Find the task index to be removed:
	std::string	task = this->_config->getActualPath(this->_configIndex,this->_requestParams[1]);
	std::string	filePath = task.substr(0, task.rfind("/"));
	if (DEBUG)
		std::cout << GREY << "[DEBUG: ...JSON file: " << filePath << "]" << DEF_COL << std::endl;
	int	index = std::atoi(task.substr(task.rfind("/") + 1).c_str());
	if (DEBUG)
		std::cout << GREY << "[DEBUG: ...index of task to be removed identified: " << index << "]" << DEF_COL << std::endl;

	// Get Json File Content and delete it
	std::string fileContent = this->readFromJsonFile(filePath);
	if (fileContent == "")
		return (this->errorResponse(404));
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
		if(i != (size_t)index)
			writeToJsonFile(stringArray[i], filePath);
		if (i == 0) break;
		i--;
	}

	return("HTTP/1.1 200 OK\r\n");
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
