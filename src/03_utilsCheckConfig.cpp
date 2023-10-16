/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   03_utilsCheckConfig.cpp                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdasilva <jdasilva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 11:57:48 by jdasilva          #+#    #+#             */
/*   Updated: 2023/10/16 19:59:56 by jdasilva         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/webserv.hpp"

bool Allow(std::string &line)
{
	std::vector<std::string> valiOptions;
	valiOptions.push_back("GET");
	valiOptions.push_back("POST");
	valiOptions.push_back("DELETE");
	std::string config, option1, option2, option3, null;
	std::istringstream iss(line);
	iss >> config >> option1 >> option2 >> option3 >> null;
	
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

bool Buffersize(std::string &line)
{
	std::string config;
	int numb;
	std::istringstream iss(line);
	iss >> config >> numb;
	
	if(numb > 0)
		return true;
	return false;
}

bool AutoIndex(std::string &line)
{
	std::string config, option, null;
	std::istringstream iss(line);
	iss >> config >> option >> null;
	std::transform(option.begin(), option.end(), option.begin(), ::tolower);
	
	if(!null.empty())
		return false;
	option = trimDoubleQuotes(option);
	if(option != "on" && option != "off")
		return false;
	return true;
}

bool Redirect(std::string &line)
{
	std::string config, null;
	std::istringstream iss(line);
	iss >> config >> null;
	
	if(null.empty())
		return false;
	return true;
}

bool ErrorPage(std::string &line)
{
	std::string config, num, str, null;
	std::istringstream iss(line);
	iss >> config >> num >> str >> null;
	
	if(!null.empty())
		return false;

	num = trimDoubleQuotes(num);
	if(!isInteger(num))
	{
		error("Error number");
		return false;
	}

	if(!(str.find(num) != std::string::npos))
	{
		error("The file does not correspond to the error number");
		return false;
	}
	
	str = trimDoubleQuotes(str);
	if (access(str.c_str(), F_OK) != 0)
	{
		error("The file does not exist");
		return false;
	}
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
			if(port > 0)
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

bool isOneOf(std::string &line)
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

bool CheckLocation(std::ifstream &file, std::string &line)
{
	int space, cont = 0;
	std::string trimstr, config;
	while(std::getline(file, line))
	{
		std::istringstream iss(line);
		iss >> config;
		trimstr = line;
		trimstr = trimSpace(trimstr);
		space = SpaceCounter(line);
		if(space != 4 || trimstr.empty())
			break;
		if(isOneOf(line))
			cont++;
		if(!isLocationConfigOf(line))
		{
			std::cout << line << " <----- ";
			error("The configuration does not exist");
			return false;
		}
		if(config == "allow:")
		{
			if(!Allow(line))
			{
				std::cout << line << " <----- ";
				error("allow config error");
				return false;
			}
		}
		if(config == "buffer_size:")
		{
			if(!Buffersize(line))
			{
				std::cout << line << " <----- ";
				error("Buffer_size config error");
				return false;
			}
		}
		if(config == "autoindex:")
		{
			if(!AutoIndex(line))
			{
				std::cout << line << " <----- ";
				error("Auto_index config error");
				return false;
			}
		}
		if (config  == "redirect:")
		{
			if(!Redirect(line))
			{
				std::cout << line << " <----- ";
				error("Redirect config error");
				return false;
			}
		}
 		if (config == "error_page:")
		{
			if(!ErrorPage(line))
			{
				std::cout << line << " <----- ";
				error("Error Page config error");
				return false;
			}
		}
		std::cout << line << std::endl;
	}
	
	if(!trimstr.empty() && !(line.find("location:") != std::string::npos) && !file.eof())
	{
		error("No empty line");
		return false;
	}
	if (cont > 0)
		return true;
	else
		return false;
}
