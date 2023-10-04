/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02_confFileCorrect.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdasilva <jdasilva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 21:34:37 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/04 20:38:09 by jdasilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/webserv.hpp"

static bool	configOK(void)
{
	debug("Config File OK");
	return (true);
}

static bool	configError(void)
{
	error("Error: Config File: the provided file is not valid");
	return (false);
}

static int SpaceCounter(std::string &line) 
{
	int cont = 0;
	
	while(line[cont] == ' ' || line[cont] == '\t')
		{
			if(line[cont] == ' ')
				cont ++;
			else if (line[cont] == '\t')
				cont += 2;		
		}
	
	return cont;
}


static bool FirstCheck(std::string &line, int space)
{
	std::string serverline, listenline, rootline, locationline;
	if(space == 0 && line.find("server:") != std::string::npos)
	{
		size_t end = line.find_last_not_of(" \t"); // Eliminar los espacios al final de la linea.
		
		if(end != std::string::npos)
			serverline = line.substr(0, end + 1);
		if(serverline != "server:")
		{
			error(line);
			return false;
		}		
		return true;
	}
	else if(space == 2)
	{
		if(line.find("listen: ") != std::string::npos)
		{
			listenline = line.substr(2, 10);
			if (listenline != "listen: ")
				return false;
			//std::cout <<"listenline: " <<listenline << std::endl;
			return true;
		}
		else if (line.find("root: ") != std::string::npos)
		{
			rootline = line.substr(2, 8);
			if (rootline != "root: ")
				return false;
			//std::cout << "rootline: " << rootline << std::endl;
			return true;
		}
		else if (line.find("location: ") != std::string::npos)
		{
			locationline = line.substr(2,12);
			if (locationline != "location: ")
				return false;
			//std::cout << "locationline: " <<  locationline << std::endl;
			return true;
		}
		return false;	
	}
	return true;
} 

bool confFileCorrect(const char **argv)
{
	std::string line;
	int space;
	std::ifstream filename(argv[1]);
	if(!filename.is_open())
	{
		error("Error can't open file");
		return(false);
	}
	
	while(std::getline(filename, line))
	{
		bool isWhitespace = true;  //Para comprobar si la linea no solo esta vacia, si tiene espacios o /t, solo tambien me lo salto.
		for(size_t i = 0; i < line.length(); i++)
		{
			if(line[i] != ' ' && line[i] != '\t')
			{
				isWhitespace = false;
				break;
			}
		}
	
		if(!isWhitespace)
		{
			space = SpaceCounter(line);
			if (space != 0 && space != 2 && space != 4)
			{
				std:: cout << line << " <--- "; 
				error("Wrong space format");
				return(configError());
			}
			std::cout << line << "espacios: " << space << std::endl;
			
			if (!FirstCheck(line, space))
			{
				std::cout << line << " <---- ";
				error("Check error");
				return(configError());
			}
		}
		
	}
	
	return (configOK());
}
