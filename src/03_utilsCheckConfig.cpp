/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   03_utilsCheckConfig.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdasilva <jdasilva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 11:57:48 by jdasilva          #+#    #+#             */
/*   Updated: 2023/10/23 18:47:48 by jdasilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/webserv.hpp"

bool doesNotStartWithDot(std::string &line)
{
	std::string config, dir;
	std::istringstream iss(line);
	iss >> config >> dir;

	dir = trimChars(dir, "\"");
	if(dir.empty())
		return false;
	if(dir.length() >= 1 && (dir[0] == '/' || (dir[0] == '.' && dir[1] == '/')))
		return false;
	return true;
}


bool Empty(std::string &line)
{
	std::string config, null;
	std::istringstream iss(line);
	iss >> config >> null;
	null = trimChars(null, "\"");
	if(null.empty())
		return false;
	return true;
}

bool isInteger(std::string numb)
{
	for(std::string::size_type i = 0; i < numb.length(); i++)
	{
		if(!std::isdigit(numb[i]))
			return false;
	}
	return true;
}

int checkAddress(std::string address)
{
	int port;
	size_t pos = address.find(":");
	
	if(pos != std::string::npos)
	{	
		if(isInteger(address.substr(pos + 1)))
		{
			port = std::stoi(address.substr(pos + 1));
			if(port > 0 && port <= 65535)
				return port;
		}
	}
	return -1;
}

bool Checkport(std::vector<int> &Allport)
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

int SpaceCounter(std::string &line) 
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



bool isLocationConfigOf(std::string &line)
{
	std::vector<std::string> valiOptions;
	std::string option;
	std::istringstream iss(line);
	valiOptions.push_back("file:");
	valiOptions.push_back("redirect:");
	valiOptions.push_back("autoindex:");
	valiOptions.push_back("allow:");
	valiOptions.push_back("buffer_size:");
	valiOptions.push_back("autoindex:");
	valiOptions.push_back("error_page:");
	valiOptions.push_back("cgi:");
	valiOptions.push_back("root:");
	
	iss >> option;

	return std::find(valiOptions.begin(),valiOptions.end(), option) != valiOptions.end();
}

static std::string trimSpace(std::string &str)
{
	size_t	start = 0, end = str.length();

	while (start < end && (str[start] == ' ' || str[start] == '\t' || str[start] == '\n'))
		start++;

	while (end > start && (str[end - 1] == ' ' || str[end -1] == '\t'|| str[end -1] == '\n'))
		end--;
	return str.substr(start, end - start);
}

static bool LocationCheckConfig(std::string &line, int &cline)
{
	std::istringstream iss(line);
	std:: string config;
	iss >> config;
	
	if(config == "allow:")
	{
		if(!Allow(line))
		{
			std:: cout << line << " <--- " << "[line: " << cline << "] ";
			error("allow config error");
			return false;
		}
	}
	if(config == "buffer_size:")
	{
		if(!Buffersize(line))
		{
			std:: cout << line << " <--- " << "[line: " << cline << "] ";
			error("Buffer_size config error");
			return false;
		}
	}
	if(config == "autoindex:")
	{
		if(!AutoIndex(line))
		{
			std:: cout << line << " <--- " << "[line: " << cline << "] ";
			error("Auto_index config error");
			return false;
		}
	}
	if (config  == "redirect:")
	{
		if(!Empty(line))
		{
			std:: cout << line << " <--- " << "[line: " << cline << "] ";
			error("Redirect config error");
			return false;
		}
	}
	if (config == "error_page:")
	{
		if(!ErrorPage(line))
		{
			std:: cout << line << " <--- " << "[line: " << cline << "] ";
			error("Error_Page config error");
			return false;
		}
	}
	if(config == "cgi:")
	{
		if(!Empty(line))
		{
			std:: cout << line << " <--- " << "[line: " << cline << "] ";
			error("cgi config error");
			return false;
		}
	}
	if(config == "file:")
	{
		if(!Empty(line) || !doesNotStartWithDot(line))
		{
			std:: cout << line << " <--- " << "[line: " << cline << "] ";
			error("File config error");
			return false;
		}
	}
	if(config == "root:")
	{
		if(!Empty(line) || !doesNotStartWithDot(line))
		{
			std:: cout << line << " <--- " << "[line: " << cline << "] ";
			error("root config error");
			return false;	
		}
	}
	return true;
}

bool CheckLocation(std::ifstream &file, std::string &line, int &cline)
{
	int space;
	std::string trimstr;
	while(std::getline(file, line))
	{
		cline++;
		trimstr = line;
		trimstr = trimSpace(trimstr);
		space = SpaceCounter(line);
	 	if(space != 4 && !line.empty()  && !(line.find("location:") != std::string::npos))
		{
			std:: cout << line << " <--- " << "[line: " << cline << "] ";
			error("Wrong space format or No empty line");
			return (false);
		} 
		if(space != 4 || trimstr.empty())
			break;
		if(!isLocationConfigOf(line))
		{
			std:: cout << line << " <--- " << "[line: " << cline << "] ";
			error("The configuration does not exist");
			return false;
		}
		if(!LocationCheckConfig(line, cline))
			return false;
			
		if(DEBUG)
			std::cout << line << std::endl;
	}
	
	if(!trimstr.empty() && !(line.find("location:") != std::string::npos) && !file.eof())
	{
		error("No empty line");
		return false;
	}
	
	return true;
}
