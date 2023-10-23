/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04_Config_list.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdasilva <jdasilva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 16:34:41 by jdasilva          #+#    #+#             */
/*   Updated: 2023/10/23 16:14:47 by jdasilva         ###   ########.fr       */
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
	option = trimChars(option, "\"");
	if(option != "on" && option != "off")
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

	num = trimChars(num, "\"");
	if(!isInteger(num))
	{
		error("Error number");
		return false;
	}

	return true;
}

bool location(std::string &line)
{
	std::string config, str;
	std::istringstream iss(line);
	char lastchar = '\0';
	iss >> config >> str;
	
	str = trimChars(str, "\"");
	lastchar = str[0];
	
	if (str.length() == 1 && lastchar != '/')
		return false;
	else if (str.length() > 1)
	{
		if(str[0] != '/')
			return false;
		lastchar = str[str.length() - 1];
	}
	return lastchar == '/';	
}
