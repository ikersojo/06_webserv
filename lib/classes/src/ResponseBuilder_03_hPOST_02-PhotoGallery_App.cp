/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder_03_hPOST_02-PhotoGallery_Ap        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdasilva <jdasilva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/02 16:22:46 by jdasilva          #+#    #+#             */
/*   Updated: 2023/11/13 15:58:42 by jdasilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ResponseBuilder.hpp"

std::string FileExtension(const std::string &file)
{
	size_t p = file.find_last_of('.');
	
	return file.substr(p + 1);
}

std::string		extractFileName(std::string request)
{
	std::string	res;

	res = request.substr(request.find("filename") + 10, request.size());
	res = res.substr(0, res.find("\n") - 1);
	return (res);
}

std::string		ResponseBuilder::uploadPhoto(void)
{

	std::string root = _config->getRoot(_configIndex, this->_requestParams[1]);
	if (DEBUG)
		std::cout << GREY << "[DEBUG: ...root dir: " << root << "]" <<DEF_COL << std::endl;

	std::string name = extractFileName(this->_requestStr);
	if (DEBUG)
		std::cout << GREY << "[DEBUG: ...file name identified: " << name << "]" <<DEF_COL << std::endl;

	std::string fileName = root + "/photos/" + name;
	if (DEBUG)
		std::cout << GREY << "[DEBUG: ...file name: " << fileName << "]" <<DEF_COL << std::endl;
	
	
	const char* folderPath  = "www/gallerySite/photos";
    // Comprueba si la carpeta existe
    struct stat st;
    if (stat(folderPath, &st) != 0) {
        // La carpeta no existe, intenta crearla
        if (mkdir(folderPath, 0755) == 0) {
			if(DEBUG)
            	std::cout << GREY << "[DEBUG: ... folder created: " << folderPath << "]" <<DEF_COL << std::endl;
        } else {
			if(DEBUG)
            	error("Error al crear la carpeta.");
        }
    }
	
	// Add task to DB:
	size_t lastBlankLinePos = this->_requestStr.rfind("\r\n\r\n");
	if (lastBlankLinePos != std::string::npos)
	{
 		std::string body = this->_requestStr.substr(lastBlankLinePos + 4);
		// Open the file in output mode to write the updated content
		std::ofstream outFile(fileName);
		if (!outFile.is_open())
		{
			error("Unable to open the file");
			return(this->errorResponse(500));
		}
		outFile.write(body.c_str(), body.size());
		outFile.close();
	}
	else
		error("Info not found");

	// Build response
	std::string ext = FileExtension(name);
	this->_responseStr  = "HTTP/1.1 200 OK\r\n";
	this->_responseStr += "Content-Type: image/" + ext + "\r\n";
	this->_responseStr += "Content-Length: ";
	this->_responseStr += intToString(fileName.size());
	this->_responseStr += "\r\n\r\n";
	this->_responseStr += fileName;

	debug("...post uploadPhoto completed and response built");
	return (this->_responseStr);
}
