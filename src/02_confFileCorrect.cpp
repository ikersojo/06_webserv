/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02_confFileCorrect.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdasilva <jdasilva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 21:34:37 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/23 18:39:19 by jdasilva         ###   ########.fr       */
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

static bool FirstCheck(std::string &line, int space, int &server_cont)
{
	std::string checkline, option, null;
	std::istringstream iss(line);
	if(space == 0)
	{
		iss >> checkline >> null;
		if(checkline != "server:" || !null.empty())
			return false;
		server_cont ++;
		return true;
	}
	else if(space == 2)
	{
		if(server_cont == 0)
		{
			error("Error No server:");
			return false;
		}
		iss >> checkline >> option >> null;
		if(line.find("listen:") != std::string::npos)
		{
			if(checkline != "listen:" || option.empty() || !null.empty())
				return false;
			return true;
		}
		else if(line.find("root:") != std::string::npos)
		{
			if(checkline != "root:" || !null.empty() || !doesNotStartWithDot(line))
				return false;
			return true;
		}
		else if(line.find("location:") != std::string::npos)
		{
			if(checkline != "location:" || option.empty() || !null.empty() || !location(line))
				return false;
			return true;
		}
		else if(line.find("error_page:") != std::string::npos)
		{
			if(checkline != "error_page:" || !ErrorPage(line))
				return false;
			return true;
		}
		else if(line.find("servername:") != std::string::npos)
		{
			if(checkline != "servername:" || option.empty() || !null.empty())
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
	int space, port;
	int listen_cont = 0;
	int location_cont = 0;
	int server_cont = 0;
	int cline = 1;
	bool firts_time = true;
	
	std::vector<int> Allport;
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
			if(space != 0 && space != 2)
			{
				std:: cout << line << " <--- " << "[line: " << cline << "] " ; 
				error("Wrong space format");
				filename.close();
				return(false);
			}
			
			if(!FirstCheck(line, space, server_cont))
			{
				std:: cout << line << " <--- " << "[line: " << cline << "] " ;
				error("Check error");
				filename.close();
				return(false);
			}
			
			if(DEBUG)
				std::cout << line << std::endl;
			
			if(line.find("listen:") != std::string::npos && space == 2) //En esta funcion mi objetivo es recolectar puertos para verificar si se repiten.
			{
				listen_cont ++;
				std::istringstream iss(line);
				std::string option, address;
				iss >> option >> address;
				if((port = checkAddress(address)) == -1)
				{
					std:: cout << line << " <--- " << "[line: " << cline << "] " ;
					error("Check Address error");
					filename.close();
					return(false);
				}
				Allport.push_back(port);
			}
			
			if(line.find("location:") != std::string::npos && space == 2)
			{
				location_cont ++;
				//std::cout << "******Compruebo el bloque location******\n";
				if(!CheckLocation(filename, line, cline))
				{
					filename.close();
					return(false);
				}
				cline--;
			}

			if(line.find("server:") != std::string::npos && space == 0)
			{
				if(!firts_time)
				{
					if(listen_cont == 0 || location_cont == 0)
					{
						error("Error: Bad configuration");
						filename.close();
						return(false);
					}
					listen_cont = 0;
					location_cont = 0;
				}
				firts_time = false;
			}
		}
		cline++;
	}
	if(filename.eof())
	{
		if(listen_cont == 0 || location_cont == 0)
		{
			filename.close();
			return(configError());
		}
	}
	filename.close();
	if(!Checkport(Allport))
	{
		error("Error Repeated port");
		return(false);
	}
	return (configOK());
}
