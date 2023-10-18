/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 09:16:19 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/17 23:09:52 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ResponseBuilder.hpp"

bool	ResponseBuilder::_shutdownRequested = false;


void	ResponseBuilder::SignalHandler(int signal)
{
	if (signal == SIGINT || signal == SIGTERM)
		_shutdownRequested = true;
}


ResponseBuilder::ResponseBuilder(Config * config, int configIndex, std::string requestStr)
{
	this->_config = config;
	this->_configIndex = configIndex;
	this->_ok = true;
	this->_requestStr = requestStr;
	this->_responseStr = "";
	debug("ResponseBuilder Object Created");
}


ResponseBuilder::~ResponseBuilder(void)
{
	debug("ResponseBuilder Object Destroyed");
}


std::string		ResponseBuilder::computeResponse(void)
{
	this->assessRequest();
	if (!this->_ok)
		return (this->errorResponse(400));
	if (this->_requestParams[2] != "HTTP/1.1")
	{
		error("webserv only work with HTTP/1.1 requests");
		return (this->errorResponse(505));
	}
	if (this->_ok && !this->_config->isValidRequest(this->_configIndex, this->_requestParams[1]))
	{
		error("Not valid url");
		return (this->errorResponse(404));
	}

	if (this->_ok && this->_requestParams[0] == "GET")
		return (this->getResponse());
	else if (this->_ok && this->_requestParams[0] == "POST")
		return (this->postResponse());
	else if (this->_ok && this->_requestParams[0] == "DELETE")
		return (this->deleteResponse());
	else if (this->_ok)
		return (this->errorResponse(405));
	else
		return (this->errorResponse(500));
}


void	ResponseBuilder::assessRequest(void)
{
	debug("Postprocessing request...");
	std::istringstream	iss(this->_requestStr);
	std::string			item;

	while (iss >> item)
		this->_requestParams.push_back(item);
	if (this->_requestParams.size() < 3)
	{
		error("HTTP request header is not correct (too few arguments)");
		this->_ok = false;
		return ;
	}
	int i = -1;
	while (++i < 3)
	{
		if (this->_requestParams[i].empty())
		{
			error("HTTP request header content is not correct (empty arguments)");
			this->_ok = false;
			return ;
		}
		if (DEBUG)
			std::cout << GREY << "[DEBUG: ...param " << i << ": "
					<< this->_requestParams[i] << "]" << DEF_COL << std::endl;
	}
	debug("...request assessed");
}


std::string	ResponseBuilder::getResponse(void)
{
	if (!this->_config->isGET(this->_configIndex, this->_requestParams[1]))
	{
		error("GET is not allowed");
		return(this->errorResponse(405));
	}
	if (this->_config->isRedir(this->_configIndex, this->_requestParams[1]))
	{
		debug("...redirection requested");
		return (this->redirResponse());
	}
	else if (this->_config->isCgi(this->_configIndex, this->_requestParams[1]))
	{
		debug("CGI requested");
		return (this->cgiResponse());
	}
	else if (this->_config->isAutoIndex(this->_configIndex, this->_requestParams[1]))
	{
		debug("autoindex requested");
		return (this->aiResponse());
	}
	else
	{
		debug("...file requested");
		return (this->fileResponse());
	}
}


std::string	ResponseBuilder::errorResponse(int code) // TODO: Add files
{
	if (code == 400)
		this->_responseStr = "HTTP/1.1 400 Bad Request\r\n";
	else if (code == 404)
		this->_responseStr = "HTTP/1.1 404 Not Found\r\n";
	else if (code == 405)
		this->_responseStr = "HTTP/1.1 405 Method Not Allowed\r\n";
	else if (code == 500)
		this->_responseStr = "HTTP/1.1 500 Internal Server Error\r\n";
	else if (code == 505)
		this->_responseStr = "HTTP/1.1 505 HTTP Version Not Supported\r\n";
	else
		this->_responseStr = "HTTP/1.1 XXX Unkown Error\r\n";

	this->_responseStr += "Content-Type: text/html\r\n\r\n";
	this->_responseStr += "<html><head><title>Error " + intToString(code) + "</title></head><body>";
	this->_responseStr +=  "<h1>Error " + intToString(code) + "</h1>";
	debug("...error response built");
	return (this->_responseStr);
}


std::string	ResponseBuilder::redirResponse(void)
{
	this->_responseStr = "HTTP/1.1 308 Permanent Redirect\r\n";
	this->_responseStr += "Location: ";
	this->_responseStr += this->_config->getFile(this->_configIndex, this->_requestParams[1]);
	this->_responseStr += "\r\n\r\n";
	debug("...redirect response built");
	return (this->_responseStr);
}


std::string	ResponseBuilder::aiResponse(void)
{
	std::string	requestedDir = this->_config->getFile(this->_configIndex, this->_requestParams[1]);
	std::string	path = this->_requestParams[1];
	DIR* dir = opendir(requestedDir.c_str());
	if (dir != NULL)
	{
		this->_responseStr = "HTTP/1.1 200 OK\r\n";
		this->_responseStr += "Content-Type: text/html\r\n\r\n";
		this->_responseStr += "<html><head><title>Index of " + path + "</title></head><body>";
		this->_responseStr +=  "<h1>Index of " + path + "</h1><ul>";

		struct dirent *	entry;
		while ((entry = readdir(dir)) != NULL)
		{
			std::string url = this->_requestParams[1] + "/" + entry->d_name;
			this->_responseStr += "<li><a href=\"" + this->_requestParams[1] + "/" + entry->d_name + "\">" + entry->d_name + "</a></li>";
			this->_config->setAIFile(this->_configIndex, url, requestedDir, entry->d_name);
		}
		this->_responseStr += "</ul></body></html>";
		closedir(dir);
		debug("...files in directory listed and added to config");
		if (DEBUG)
			this->_config->printConfig();

		if (this->_config->isValidRequest(this->_configIndex, this->_requestParams[1] + "/index.html"))
		{
			this->_requestParams[1] = this->_requestParams[1] + "/index.html";
			debug("...index file found");
			return (this->fileResponse());
		}
		debug("...auto index response built");
		return (this->_responseStr);
	}
	else
	{
		error("Directory not found");
		return (this->errorResponse(500));
	}
}


// 		"HTTP/1.1 200 OK\r\n"
// 		"Content-Type: text/plain\r\n"
// 		"Content-Length: 23\r\n"
// 		"\r\n"
// 		"Hello, Andoni and John!";

std::string	ResponseBuilder::fileResponse(void)
{
	std::string			filePath;
	std::ifstream		inFile;
	std::ostringstream	fileContentStream;
	std::string			fileContent;
	int					fileSize;

	filePath = this->_config->getFile(this->_configIndex, this->_requestParams[1]);
	if (DEBUG)
		std::cout << GREY << "[DEBUG: ...requested file: " << filePath << "]" << DEF_COL << std::endl;

	inFile.open(filePath);
	if (!inFile.is_open())
	{
		error("Requested file not found");
		return (this->errorResponse(404));
	}
    fileContentStream << inFile.rdbuf();
    fileContent = fileContentStream.str();
	fileSize = fileContent.size();

	this->_responseStr = "HTTP/1.1 200 OK\r\n";
	if (filePath.find(".html") != std::string::npos)
		this->_responseStr += "Content-Type: text/html\r\n";
	else if (this->_requestParams[1] == "/favicon.ico" || this->_requestParams[1] == "/image" || this->_requestParams[1].find(".jpg") != std::string::npos || this->_requestParams[1].find(".png") != std::string::npos)
		this->_responseStr += "Content-Type: image/vnd.microsoft.icon\r\n";
	else if (filePath.find(".json") != std::string::npos)
	{
		this->_responseStr += "Content-Type: application/json\r\n";
		this->initJson(filePath);
	}
	else
		this->_responseStr += "Content-Type: text/plain\r\n";
	this->_responseStr += "Content-Length: ";
	this->_responseStr += intToString(fileSize);
	this->_responseStr += "\r\n\r\n";
	this->_responseStr += fileContent;
	inFile.close();
	debug("...file response built");
	return (this->_responseStr);
}


std::string	ResponseBuilder::postResponse(void)
{
	if (!this->_config->isPOST(this->_configIndex, this->_requestParams[1]))
	{
		error("POST is not allowed");
		return(this->errorResponse(405));
	}
	if (this->_config->getHandlePOST(this->_configIndex, this->_requestParams[1]) == "addToList")
	{
		this->addToList();

		std::string			filePath;
		std::ifstream		inFile;
		std::ostringstream	fileContentStream;
		std::string			fileContent;
		int					fileSize;

		filePath = this->_config->getFile(this->_configIndex, this->_requestParams[1]);
		this->initJson(filePath);
		if (DEBUG)
			std::cout << GREY << "[DEBUG: ...requested file: " << filePath << "]" << DEF_COL << std::endl;

		inFile.open(filePath);
		if (!inFile.is_open())
		{
			error("Requested file not found");
			return (this->errorResponse(404));
		}
		fileContentStream << inFile.rdbuf();
		fileContent = fileContentStream.str();
		fileSize = fileContent.size();

		this->_responseStr  = "HTTP/1.1 200 OK\r\n";
		this->_responseStr += "Content-Type: application/json\r\n";
		this->_responseStr += "Content-Length: ";
		this->_responseStr += intToString(fileSize);
		this->_responseStr += "\r\n\r\n";
		this->_responseStr += fileContent;
		inFile.close();
		debug("...post response built");
		return (this->_responseStr);
	}
	else
		return(this->errorResponse(500));
}


std::string	extractTask(const std::string& str)
{
	size_t quoteStart = 0;
	size_t quoteEnd = str.rfind("\"");
	size_t count = 0;

	while (count < 3)
	{
		quoteStart = str.find("\"", quoteStart);
		++count;
		if (count < 3)
			quoteStart++;
	}
	std::string extractedContent = str.substr(quoteStart + 1, quoteEnd - quoteStart - 1);
	size_t i = 0;
	while(i < extractedContent.size())
	{
		if (extractedContent[i] == ' ')
			extractedContent[i] = '_';
		i++;
	}
	return (extractedContent);
}

void	ResponseBuilder::addToList(void)
{
	// Find the last line (after a blank line)
	size_t lastBlankLinePos = this->_requestStr.rfind("\r\n\r\n");
	if (lastBlankLinePos != std::string::npos)
	{
		std::string lastLine = this->_requestStr.substr(lastBlankLinePos + 4);
		std::string	task = extractTask(lastLine);
		if (DEBUG)
			std::cout << GREY << "[DEBUG: ...extracted task: " << task << " ]" << DEF_COL << std::endl;

		std::string	filePath = this->_config->getFile(this->_configIndex, this->_requestParams[1]);
		this->writeToJsonFile(task, filePath);

		std::string url = this->_requestParams[1] + "/" + task;
		this->_config->setDeletePath(this->_configIndex, url, task);
		debug("...url with task added to config for deletion");
		if (DEBUG)
			this->_config->printConfig();
	}
}

void	ResponseBuilder::writeToJsonFile(std::string task, std::string filePath)
{
	std::ifstream		inFile;

	if (DEBUG)
		std::cout << GREY << "[DEBUG: ...JSON file to include task: " << filePath << "]" << DEF_COL << std::endl;

	// Open the file in input mode to read the existing content
	inFile.open(filePath);
	if (!inFile.is_open())
	{
		error("JSON file not found");
		return ;
	}

	// Read the existing content into a string
	std::ostringstream	fileContentStream;
	fileContentStream << inFile.rdbuf();
	std::string fileContent = fileContentStream.str();

	// Close the input file
	inFile.close();

	// Find the position of the first "[" character
	size_t position = fileContent.find_first_of('[');
	if (position == std::string::npos)
	{
		error("Invalid JSON file format");
		return;
	}

	// Insert the new task at the position after the first "[" character
	if (countOccurrences(fileContent, '\"') > 0)
		fileContent.insert(position + 1, "\"" + task + "\", ");
	else 
		fileContent.insert(position + 1, "\"" + task + "\"");

	// Open the file in output mode to write the updated content
	std::ofstream outFile(filePath);
	if (!outFile.is_open())
	{
		error("Unable to open the JSON file for writing");
		return;
	}

	// Write the updated content back to the file
	outFile << fileContent;

	// Close the output file
	outFile.close();

	debug("...task included in json file");
}


std::string	ResponseBuilder::deleteResponse(void)
{
	if (!this->_config->isDELETE(this->_configIndex, this->_requestParams[1]))
	{
		error("DELETE is not allowed");
		return(this->errorResponse(405));
	}
	if (this->_config->getHandleDELETE(this->_configIndex, this->_requestParams[1]) == "removeFromList")
	{
		std::string	task = this->_config->getFile(this->_configIndex, this->_requestParams[1]);
		if (DEBUG)
			std::cout << GREY << "[DEBUG: ...task to be removed identified: " << task << "]" << DEF_COL << std::endl;

		this->removeFromList(task);

		this->_requestParams[1] = this->_requestParams[1].substr(0, this->_requestParams[1].rfind("api"));
		if (DEBUG)
			std::cout << GREY << "[DEBUG: ...redirecting to " << this->_requestParams[1] << "]" << DEF_COL << std::endl;
		return(this->fileResponse());
	}
	else
		return(this->errorResponse(500));
}

void	ResponseBuilder::removeFromList(std::string task)
{
	std::string	filePath = this->_config->getFile(this->_configIndex, this->_requestParams[1].substr(0, this->_requestParams[1].rfind("/")));
	if (DEBUG)
		std::cout << GREY << "[DEBUG: ...JSON file: " << filePath << "]" << DEF_COL << std::endl;
	std::ifstream file(filePath);

	if (!file.is_open())
	{
		error("Error opening JSON file");
		return ;
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string fileContent = buffer.str();
	file.close();
	this->clearJsonFile(filePath);

	// Find the start and end of the array
	size_t start = fileContent.find("[");
	size_t end = fileContent.rfind("]");

	// Extract the content of the array
	std::string arrayContent = fileContent.substr(start + 1, end - start - 1);

	// Initialize a string stream to parse the array content
	std::stringstream arrayStream(arrayContent);

	// Vector to store individual strings
	std::vector<std::string> stringArray;

	// Temporary variable to store each string
	std::string temp;

	// Iterate through the array content and extract individual strings
	while (std::getline(arrayStream, temp, ','))
	{
		// Remove leading and trailing whitespaces
		size_t firstNotSpace = temp.find_first_not_of(" \t\n\r\"");
		size_t lastNotSpace = temp.find_last_not_of(" \t\n\r\"");
		temp = temp.substr(firstNotSpace, lastNotSpace - firstNotSpace + 1);

		// Add the string to the vector
		stringArray.push_back(temp);
	}

	// Iterate through the vector and print each string
	size_t i = 0;
	while(i < stringArray.size())
	{
		if(task != stringArray[i])
			writeToJsonFile(stringArray[i], filePath);
		i++;
	}
}

void	ResponseBuilder::clearJsonFile(std::string filePath)
{
	std::ofstream outFile(filePath);
	if (!outFile.is_open())
	{
		error("Unable to open the JSON file for writing");
		return;
	}
	outFile << "[]";
	outFile.close();
}


void	ResponseBuilder::initJson(std::string filePath)
{
	if (DEBUG)
		std::cout << GREY << "[DEBUG: ...Initializing JSON file: " << filePath << "]" << DEF_COL << std::endl;
	std::ifstream file(filePath);

	if (!file.is_open())
	{
		error("Error opening JSON file");
		return ;
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string fileContent = buffer.str();
	file.close();
	this->clearJsonFile(filePath);

	// Find the start and end of the array
	size_t start = fileContent.find("[");
	size_t end = fileContent.rfind("]");

	// Extract the content of the array
	std::string arrayContent = fileContent.substr(start + 1, end - start - 1);

	// Initialize a string stream to parse the array content
	std::stringstream arrayStream(arrayContent);

	// Vector to store individual strings
	std::vector<std::string> stringArray;

	// Temporary variable to store each string
	std::string temp;

	// Iterate through the array content and extract individual strings
	while (std::getline(arrayStream, temp, ','))
	{
		// Remove leading and trailing whitespaces
		size_t firstNotSpace = temp.find_first_not_of(" \t\n\r\"");
		size_t lastNotSpace = temp.find_last_not_of(" \t\n\r\"");
		temp = temp.substr(firstNotSpace, lastNotSpace - firstNotSpace + 1);

		// Add the string to the vector
		stringArray.push_back(temp);
	}

	// Iterate through the vector and print each string
	size_t i = 0;
	while(i < stringArray.size())
	{
		writeToJsonFile(stringArray[i], filePath);
		std::string url = this->_requestParams[1] + "/" + stringArray[i];
		this->_config->setDeletePath(this->_configIndex, url, stringArray[i]);
		i++;
	}
}

std::string	ResponseBuilder::cgiResponse(void)
{
	int			fd[2];
	pid_t		pid;
	std::string	response = "";
	std::string	execFile = this->_config->getFile(this->_configIndex, this->_requestParams[1]).c_str();

	if (pipe(fd) == -1)
		error("pipe failed");

	pid = fork();

	if (pid == -1)
		return ("fork failed");

	else if (pid == 0) // Child process
	{
		close(fd[1]);
		dup2(fd[0], STDIN_FILENO);
		close(fd[0]);

		// Execute the CGI script
		char* const argv[] = {const_cast<char*>(execFile.c_str())};
		char* const envp[] = {NULL};
		execve(argv[0], argv, envp);
		return ("execve failed");
	}
	else // Parent process
	{
		close(fd[0]);
		write(fd[1], command.c_str(), command.length());
		close(fd[1]);

		// Wait for the child process to finish
		int status;
		waitpid(pid, &status, 0);

		// Read the response from the child process
		char buffer[1024];
		while (read(fd[0], buffer, sizeof(buffer)) != 0)
			response += buffer;
		close(fd[0]);

		return response;
	}
}
