/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder_04_hDELETE_02-gallerySite.c        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdasilva <jdasilva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 17:18:36 by jdasilva          #+#    #+#             */
/*   Updated: 2023/11/03 18:43:43 by jdasilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ResponseBuilder.hpp"

std::string ResponseBuilder::deletePhoto(void)
{
	std:: string content = this->_requestStr;
	size_t start = content.find("{");
	size_t end = content.find("}");
	std::string arrayContent = content.substr(start + 1, end - start - 1);
	std::cout << " ====== " << arrayContent << "\n";
 	std::string dbFilePath = this->_config->getActualPath(this->_configIndex, this->_requestParams[1]);
	std::string	filePath = dbFilePath.substr(dbFilePath.rfind("/") + 1);
	if (DEBUG)
		std::cout << GREY << "[DEBUG: ...JSON file: " << filePath << "]" << DEF_COL << std::endl;
	
	size_t startPos = arrayContent.find(":\"") + 2;
    size_t endPos = arrayContent.rfind("\"");

	std::string deletePhoto = arrayContent.substr(startPos, endPos - startPos);
	if (DEBUG)
		std::cout << GREY << "[DEBUG: ...the deleted photo is: " << deletePhoto  <<DEF_COL << std::endl;
		
	std::string filecontent = this->readFromJsonFile(dbFilePath);
	if (filecontent == "")
		return(this->errorResponse(404));
	this->clearJsonFile(dbFilePath);
	
 	size_t json_start = filecontent.find("[");
	size_t json_end = filecontent.find("]");
	std::string json_content = filecontent.substr(json_start + 1, json_end - json_start - 1); 
	
	std::stringstream ss(json_content);
	std::string line;
	std::cout << " ========== \n" << json_content << "\n" << " ======== " << "\n";

	while(std::getline(ss, line, ','))
	{
		if(!(line.find(deletePhoto) != std::string::npos))
		{
			std::cout << line ;
			this->writeToJsonFile(line, dbFilePath);
		}

	}
	
	
	
	
	return("HTTP/1.1 200 OK\r\n");
}
