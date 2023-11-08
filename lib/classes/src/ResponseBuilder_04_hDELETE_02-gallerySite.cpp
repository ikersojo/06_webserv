/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder_04_hDELETE_02-gallerySite.c        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdasilva <jdasilva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 17:18:36 by jdasilva          #+#    #+#             */
/*   Updated: 2023/11/08 19:14:58 by jdasilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ResponseBuilder.hpp"

std::string		FileName(std::string request)
{
	std::string	res;

	res = request.substr(request.find("filename") + 10, request.size());
	res = res.substr(0, res.find("\n") - 1);
	return (res);
}


std::string ResponseBuilder::deletePhoto(void)
{
	std:: string content = this->_requestStr;

	std::string root = _config->getRoot(_configIndex, this->_requestParams[1]);
	if (DEBUG)
		std::cout << GREY << "[DEBUG: ...root dir: " << root << "]" <<DEF_COL << std::endl;

	std::string name = FileName(this->_requestStr);
	if (DEBUG)
		std::cout << GREY << "[DEBUG: ...file name identified: " << name << "]" <<DEF_COL << std::endl;

	std::string fileName = root + "/photos/" + name;
	if (DEBUG)
		std::cout << GREY << "[DEBUG: ...file name: " << fileName << "]" <<DEF_COL << std::endl;



	if (std::remove(fileName.c_str()) == 0) {
		if(DEBUG)
        	std::cout << GREY << "[DEBUG: ...delete file name: " << fileName << "]" <<DEF_COL << std::endl;
    } else {
         error("I cannot delete the file");
    }

	std::string folderRemoveSTR = root + "/photos"; 
	const char* folderRemove = folderRemoveSTR.c_str();

    DIR* directorio = opendir(folderRemove);
    if (directorio) {
        struct dirent* entrada;
        bool hasFiles = false;
		
        while ((entrada = readdir(directorio)) && !hasFiles) {
            if (std::strcmp(entrada->d_name, ".") != 0 && std::strcmp(entrada->d_name, "..") != 0) {
                hasFiles = true;
            }
        }

        closedir(directorio);

        if (!hasFiles) {
            if (rmdir(folderRemove) == 0) {
				if(DEBUG)
        		std::cout << GREY << "[DEBUG: ...delete folder]" <<DEF_COL << std::endl;
            } else {
                error("Error deleting the directory.");
            }
        }
    }
	
	return("HTTP/1.1 200 OK\r\n");
}
