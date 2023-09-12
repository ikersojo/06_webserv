/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   01_syntaxCorrect.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/09/12 19:12:16 by isojo-go          #+#    #+#             */
/*   Updated: 2023/09/12 20:39:23 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/webserv.hpp"

// Syntax error
static bool	syntaxError(void)
{
	std::cout << RED << "Error: Syntax: use ./bin/webserv XXXXX" << EXT << DEF_COL << std::endl;
	return (false);
}

// Syntax OK
static bool	syntaxOK(void)
{
	debug("Syntax ok!");
	return (true);
}

// Checks if the file extension of the filename provided is valid.
static bool fileExtCorrect(const std::string & filename, const std::string & ext)
{
	if (!filename.empty() &&
		filename.find(ext) != std::string::npos &&

		filename.substr(filename.length() - ext.length()) == ext &&

		filename.length() > ext.length())
			return (syntaxOK());
	return (syntaxError());
}

// Checks the user input for the main executable
bool syntaxCorrect(int argc, const char** argv)
{
	if (argc != 2)
		return (syntaxError());
	std::string filename(argv[1]);
	return (fileExtCorrect(filename, EXT));
}
