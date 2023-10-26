/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   04_Config_list.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdasilva <jdasilva@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/18 16:34:41 by jdasilva          #+#    #+#             */
/*   Updated: 2023/10/26 15:53:08 by jdasilva         ###   ########.fr       */
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
					std::cout << option1 << " <--- is repeated\n";
					return false;
				}
				if(!option3.empty())
				{
					if(std::find(valiOptions.begin(), valiOptions.end(), option3) != valiOptions.end())
					{
						if(option3 == option1 || option2 == option3)
						{
							std::cout << option3 << " <---- is repeated\n";
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
	int numb = 0;
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

	if(!null.empty() || option.empty())
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
	num = trimChars(num, "\"");
	str = trimChars(str, "\"");

	if(!null.empty() || num.empty() || str.empty())
		return false;
		
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
	char c = '\0';
	iss >> config >> str;
	
	str = trimChars(str, "\"");
	c = str[0];
	if(str.empty())
		return false;
	else if (str.length() > 1)
	{
		if(str[0] != '/')
		{
			return false;
		}
	}
 	return c == '/';	
}
