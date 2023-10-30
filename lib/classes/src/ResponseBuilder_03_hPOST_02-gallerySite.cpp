/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder_03_hPOST_02-gallerySite.cpp        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdasilva <jdasilva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 18:29:45 by jdasilva          #+#    #+#             */
/*   Updated: 2023/10/30 19:15:54 by jdasilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ResponseBuilder.hpp"

std::string ResponseBuilder::uploadPhoto(void)
{
	std::cout << " ===== " << this->_config->getEnvp() << "\n";
	
	const char* folderPath  = "www/gallerySite/photos";
	
    // Comprueba si la carpeta existe
    struct stat st;
    if (stat(folderPath, &st) != 0) {
        // La carpeta no existe, intenta crearla
        if (mkdir(folderPath, 0755) == 0) {
            std::cout << "Carpeta creada: " << folderPath << std::endl;
        } else {
            std::cerr << "Error al crear la carpeta." << std::endl;
        }
    }
	
	DIR *dir;
	struct dirent *ent;

	if((dir = opendir(folderPath)) != NULL)
	{
		while((ent = readdir(dir)) != NULL)
		{
			if (ent->d_type== DT_REG)
			{
				std ::cout << ent->d_name << "\n";
			}
		}
		closedir(dir);
	}
	else
	{
		std::cerr<< "Error al abrir la carpeta\n";
	}
	
	return("HTTP/1.1 200 OK\r\n");
}
