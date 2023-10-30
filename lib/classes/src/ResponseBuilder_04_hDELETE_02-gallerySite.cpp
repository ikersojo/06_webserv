/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder_04_hDELETE_02-gallerySite.c        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdasilva <jdasilva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/30 17:18:36 by jdasilva          #+#    #+#             */
/*   Updated: 2023/10/30 18:41:35 by jdasilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ResponseBuilder.hpp"

std::string ResponseBuilder::deletePhoto(void)
{
	const char* folderPath  = "www/gallerySite/photos";
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
