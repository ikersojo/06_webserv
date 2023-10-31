/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder_05_hCGI.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aarrien- <aarrien-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 09:16:19 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/31 12:11:21 by aarrien-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ResponseBuilder.hpp"

extern char **environ;

void handleTimeout(int signum)
{
	(void)signum;
	error("Child process timed out");
	exit(EXIT_FAILURE);
}

std::string	ResponseBuilder::cgiPOSTResponse(void)
{
	error("POST cgiArgs still not captured");
	return (cgiResponse());
}

std::string	ResponseBuilder::cgiGETResponse(void)
{
	std::string	args, arg;
	size_t	pos;

	if (_requestParams[1].find("?") != std::string::npos) {
		debug("CGI arguments received");

		args = _requestParams[1].substr(_requestParams[1].find("?")+1);
		_requestParams[1] = _requestParams[1].substr(0, _requestParams[1].find("?"));

		while (true) {
			pos = args.find("?");
			arg = args.substr(0, pos);

			if (arg.size() < 3 || arg.find("=") == std::string::npos || arg.find("=") == arg.size()-1 || arg.find("=") == 0)
				error("Invalid argument");
			else
				_cgiArgs.push_back(args.substr(0, pos));
			if (pos == std::string::npos)
				break;
			args = args.substr(pos+1);
		}
		debug("CGI arguments saved in config");

		debug("...CAPTURED ARGS:");
		for (std::vector<std::string>::iterator it = _cgiArgs.begin(); it != _cgiArgs.end(); it++)
			debug(*it);
	}

	return (cgiResponse());
}

std::string	ResponseBuilder::cgiResponse(void)
{
	int			timeoutSeconds = 3;
	pid_t		pid;
	int			fd[2];
	int			status;
	int			exitStatus = 0;
	std::string	response = "";
	std::string	execFile = this->_config->getActualPath(this->_configIndex, this->_requestParams[1]).c_str();

	signal(SIGALRM, handleTimeout);

	if (pipe(fd) == -1)
		error("pipe failed");
	pid = fork();
	if (pid == -1)
	{
		error("fork failed");
		return response;
	}
	else if (pid == 0) // Child process
	{
		setenv("REQUEST_METHOD", this->_requestParams[0].c_str(), 1);
		setenv("SCRIPT_NAME", execFile.c_str(), 1);

		for (std::vector<std::string>::iterator it = _cgiArgs.begin(); it != _cgiArgs.end(); it++) {
			std::string key = it->substr(0, it->find("="));
			std::string value = it->substr(it->find("=")+1);
			setenv(key.c_str(), value.c_str(), 1);
		}

		if (DEBUG)
		{
			std::cout << GREY << "[DEBUG: ...REQUEST_METHOD: " << this->_requestParams[0] << "]" << DEF_COL << std::endl;
			std::cout << GREY << "[DEBUG: ...SCRIPT_NAME: " << execFile << "]" << DEF_COL << std::endl;
		}

		// Execute the CGI script
		char* const argv[] = {const_cast<char*>(execFile.c_str()), NULL};

		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		alarm(timeoutSeconds);
		exitStatus = execve(argv[0], argv, environ);
		error("Execve failed");
		perror("ERROR_INFO");
		exit(exitStatus);
	}
	else // Parent process
	{
		close(fd[1]);
		waitpid(pid, &status, 0);

		if (WIFEXITED(status) && WEXITSTATUS(status) == 0) // Child process exited successfully with status 0
		{
			char buffer[1024];
			while (ssize_t bytesRead = read(fd[0], buffer, sizeof(buffer)))
			{
				if (bytesRead == -1)
				{
					error("Read from CGI script failed");
					break;
				}
				buffer[bytesRead] = '\0';
				response += buffer;
			}
			close(fd[0]);
		}
		else
		{
			close(fd[0]);
			error("Child exit status");
			return(this->errorResponse(500));
		}
		return response;
	}
}
