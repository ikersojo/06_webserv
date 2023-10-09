/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02_confFileCorrect.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdasilva <jdasilva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 21:34:37 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/09 20:19:42 by jdasilva         ###   ########.fr       */
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

static bool isOneOf(std::string &line)
{
	std::vector<std::string> valiOptions;
	std::string option;
	std::istringstream iss(line);
	valiOptions.push_back("file:");
	valiOptions.push_back("redirect:");
	valiOptions.push_back("autoindex:");
	
	iss >> option;

	return std::find(valiOptions.begin(),valiOptions.end(), option) != valiOptions.end();
}
static bool isAllowof(std::string &line)
{
	std::vector<std::string> valiOptions;
	valiOptions.push_back("GET");
	valiOptions.push_back("POST");
	valiOptions.push_back("DELETE");
	std::string config, option1, option2, option3;
	std::istringstream iss(line);
	iss >> config >> option1 >> option2 >> option3;

	if(std::find(valiOptions.begin(), valiOptions.end(), option1) != valiOptions.end());

}

static bool CheckLocation(std::ifstream &file, std::string &line)
{
	int space, cont = 0;
	
	while(std::getline(file, line))
	{
		space = SpaceCounter(line);
		if(space != 4)
			break;
		if (isOneOf(line))
			cont++;
		if(line.find("allow:") != std::string::npos)
		{
			if(!isAllowof(line))
			{
				std::cout << line << "<-----";
				error("allow config error");
				return false;
			}
		}
		std::cout << line << std::endl;
	}
	std::cout << "******************\n";
	if(cont > 0)
		return true;
	else
		return false;
}


static bool FirstCheck(std::string &line, int space) // El primer checkeo compruebo si esta [server:, listen:, root:, location:] dentro de la configuracion.
{
	std::string checkline, option, null;
	std::istringstream iss(line);
	if(space == 0)
	{
		iss >> checkline >> null;
		if(checkline != "server:" || !null.empty())
			return false;
		return true;
	}
	else if(space == 2)
	{
		iss >> checkline >> option >> null;
		if(line.find("listen:") != std::string::npos)
		{
			if((checkline != "listen:") || option.empty() || !null.empty())
				return false;
			return true;
		}
		else if (line.find("root:") != std::string::npos)
		{
			if (checkline != "root:" || !null.empty())
				return false;
			return true;
		}
		else if (line.find("location:") != std::string::npos)
		{
			if (checkline != "location:" || option.empty() || !null.empty())
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
	
	while(line.find("location:") != std::string::npos || std::getline(filename, line))
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
			if (space != 0 && space != 2)
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
			if (line.find("location:") != std::string::npos)
			{
				std::cout << "******Compruebo el bloque location******\n";
				if(!CheckLocation(filename, line))
				{
					error("Error Location Config");
					return(configError());
				}
			}
		}
	}
	return (configOK());
}
