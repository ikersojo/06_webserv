/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   02_confFileCorrect.cpp                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdasilva <jdasilva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 21:34:37 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/12 20:01:33 by jdasilva         ###   ########.fr       */
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
			else if(line[cont] == '\t')
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

static bool Allow(std::string &line)
{
	std::vector<std::string> valiOptions;
	valiOptions.push_back("GET");
	valiOptions.push_back("POST");
	valiOptions.push_back("DELETE");
	std::string config, option1, option2, option3, null;
	std::istringstream iss(line);
	iss >> config >> option1 >> option2 >> option3 >> null;
	
	std::transform(option1.begin(), option1.end(), option1.begin(), ::toupper);
    std::transform(option2.begin(), option2.end(), option2.begin(), ::toupper);
    std::transform(option3.begin(), option3.end(), option3.begin(), ::toupper);

	if(!null.empty())
		return false;

 	if(std::find(valiOptions.begin(), valiOptions.end(), option1) != valiOptions.end())
	{
		if(!option2.empty())
		{
			if(std::find(valiOptions.begin(), valiOptions.end(), option2) != valiOptions.end())
			{
				if(option1 == option2)
				{
					std::cout << option1;
					error(" <--- is repeated");
					return false;
				}
				if(!option3.empty())
				{ 
					if(std::find(valiOptions.begin(), valiOptions.end(), option3) != valiOptions.end())
					{
						if(option3 == option1 || option2 == option3)
						{
							std::cout << option3;
							error(" <---- is repeated");
							return false;
						}
						return true;
					}
					return false;
				}
				return true;
			}
			return false;
		}
		return true;
	}
	return false;
}

static bool Buffersize(std::string &line)
{
	std::string config;
	int numb;
	std::istringstream iss(line);
	iss >> config >> numb;
	
	if(numb > 0)
		return true;
	return false;
}

static bool AutoIndex(std::string &line)
{
	std::string config, option, null;
	std::istringstream iss(line);
	iss >> config >> option >> null;
	std::transform(option.begin(), option.end(), option.begin(), ::tolower);
	
	if(!null.empty())
		return false;
	if(option != "on" && option != "off")
		return false;
	return true;
}

static bool Redirect(std::string &line)
{
	std::string config, null;
	std::istringstream iss(line);
	iss >> config >> null;
	
	if(null.empty())
		return false;
	return true;
}

static bool CheckLocation(std::ifstream &file, std::string &line)
{
	int space, cont = 0;
	
	while(std::getline(file, line))
	{
		space = SpaceCounter(line);
		if(space != 4)
			break;
		if(isOneOf(line))
			cont++;
		if(line.find("allow:") != std::string::npos)
		{
			if(!Allow(line))
			{
				std::cout << line << "<-----";
				error("allow config error");
				return false;
			}
		}
		if(line.find("buffer_size:") != std::string::npos)
		{
			if(!Buffersize(line))
			{
				std::cout << line << "<-----";
				error("Buffer_size config error");
				return false;
			}
		}
		if(line.find("autoindex:") != std::string::npos)
		{
			if(!AutoIndex(line))
			{
				std::cout << line << "<-----";
				error("Auto_index config error");
				return false;
			}
		}
		if(line.find("redirect:") != std::string::npos)
		{
			if(!Redirect(line))
			{
				std::cout << line << "<-----";
				error("Redirect config error");
				return false;
			}
		}
		std::cout << line << std::endl;
	}
	if (cont > 0)
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
			if(checkline != "listen:" || option.empty() || !null.empty())
				return false;
			return true;
		}
		else if(line.find("root:") != std::string::npos)
		{
			if(checkline != "root:" || !null.empty())
				return false;
			return true;
		}
		else if(line.find("location:") != std::string::npos)
		{
			if(checkline != "location:" || option.empty() || !null.empty())
				return false;
			return true;
		}
		return false;	
	}
	return true;
} 

static bool isInteger(std::string port)
{
	for(std::string::size_type i = 0; i < port.length(); i++)
	{
		if(!std::isdigit(port[i]))
			return false;
	}
	return true;
}

static int checkAddress(std::string address)
{
	int port;
	size_t pos = address.find(":");
	
	if(pos != std::string::npos)
	{	
		if(isInteger(address.substr(pos + 1)))
		{
			port = std::stoi(address.substr(pos + 1));
			if(port > 0)
				return port;
		}
	}
	return -1;
}

static bool Checkport(std::vector<int> &Allport)
{
	/* for (std::vector<int>::size_type i = 0; i < Allport.size(); i++) {
        std::cout << "Elemento " << i << ": " <<Allport[i] << std::endl;
    } */
	for(std::vector<int>::size_type i = 0; i < Allport.size(); i++)
	{
		for(std::vector<int>::size_type j = i + 1; j < Allport.size(); j ++)
		{
			if(Allport[i] == Allport[j])
			{
				std::cout << Allport[i] << " <---- ";
				return false;
			}
		}
	}
	return true;
}

bool confFileCorrect(const char **argv)
{
	std::string line;
	int space, port;
	int flag = 0;
	std::vector<int> Allport;
	std::ifstream filename(argv[1]);
	if(!filename.is_open())
	{
		error("Error can't open file");
		return(false);
	}
	
	while((line.find("location:") != std::string::npos && line.find(" ") !=std::string::npos
		&& line.find("\n") != std::string::npos) || std::getline(filename, line))
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
		
		if(isWhitespace && flag)
		{
			flag = 0;
		}
		else if(!isWhitespace && flag)
		{
			error("No line feed");
			filename.close();
			return (configError());
		}
			
		if(!isWhitespace && !flag)
		{
			space = SpaceCounter(line);
			if(space != 0 && space != 2)
			{
				std:: cout << line << " <--- "; 
				error("Wrong space format");
				filename.close();
				return(configError());
			}
			
			if(!FirstCheck(line, space))
			{
				std::cout << line << " <---- ";
				error("Check error");
				filename.close();
				return(configError());
			}
			std::cout << line << std::endl;
			if(line.find("location:") != std::string::npos)
			{
				std::cout << "******Compruebo el bloque location******\n";
				if(!CheckLocation(filename, line))
				{
					error("Error Location Config");
					filename.close();
					return(configError());
				}
				flag = 1;
			}
			if(line.find("listen:") != std::string::npos) //En esta funcion mi objetivo es recolectar puertos para verificar si se repiten.
			{
				std::istringstream iss(line);
				std::string option, address;
				iss >> option >> address;
				if((port = checkAddress(address)) == -1)
				{
					std::cout << line << "<----";
					error("Check Address error");
					filename.close();
					return(configError());
				}
				Allport.push_back(port);
			}
		
		}
	}
	if(!Checkport(Allport))
	{
		error("Error Repeated port");
		return(configError());
	}
	return (configOK());
}
