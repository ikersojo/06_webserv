/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02_confFileCorrect.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdasilva <jdasilva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 21:34:37 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/05 20:27:44 by jdasilva         ###   ########.fr       */
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


static bool FirstCheck(std::string &line, int space) // El primer checkeo compruebo si esta [server:, listen:, root:, location:] dentro de la configuracion.
{
	std::string checkline;
	if(space == 0 && line.find("server:") != std::string::npos)
	{
		size_t end = line.find_last_not_of(" \t"); // Eliminar los espacios al final de la linea.
		
		if(end != std::string::npos)
			checkline = line.substr(0, end + 1);
		if(checkline != "server:")
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
			checkline = line.substr(2, 8);
			//std::cout <<"checkline: " <<checkline << std::endl;
			if (checkline != "listen: ")
				return false;
			return true;
		}
		else if (line.find("root: ") != std::string::npos)
		{
			checkline = line.substr(2, 6);
			//std::cout << "checkline: " << checkline << std::endl;
			if (checkline != "root: ")
				return false;
			return true;
		}
		else if (line.find("location: ") != std::string::npos)
		{
			checkline = line.substr(2,10);
			//std::cout << "checkline: " <<  checkline << std::endl;
			if (checkline != "location: ")
				return false;
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
				filename.close();
				return(configError());
			}
			
			if (!FirstCheck(line, space))
			{
				std::cout << line << " <---- ";
				error("Check error");
				filename.close();
				return(configError());
			}
			std::cout << line << std::endl;
		}
	}
	return (configOK());
}
