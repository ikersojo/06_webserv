/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ResponseBuilder_05_hCGI.cpp                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isojo-go <isojo-go@student.42urduliz.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/13 09:16:19 by isojo-go          #+#    #+#             */
/*   Updated: 2023/10/19 22:25:55 by isojo-go         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ResponseBuilder.hpp"


void handleTimeout(int signum)
{
	(void)signum;
	error("Child process timed out");
	exit(EXIT_FAILURE);
}


std::string	ResponseBuilder::cgiGETResponse(void)
{
	int			timeoutSeconds = 3;
	pid_t		pid;
	int			fd[2];
	int			status;
	int			exitStatus = 0;
	std::string	response = "";
	std::string	execFile = this->_config->getFile(this->_configIndex, this->_requestParams[1]).c_str();

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

		if (DEBUG)
		{
			std::cout << GREY << "[DEBUG: ...REQUEST_METHOD: " << this->_requestParams[0] << "]" << DEF_COL << std::endl;
			std::cout << GREY << "[DEBUG: ...SCRIPT_NAME: " << execFile << "]" << DEF_COL << std::endl;
		}

		// Execute the CGI script
		char* const argv[] = {const_cast<char*>(execFile.c_str()), NULL};
		char* const envp[] = {NULL};

		dup2(fd[1], STDOUT_FILENO);
		close(fd[0]);
		close(fd[1]);
		alarm(timeoutSeconds);
		exitStatus = execve(argv[0], argv, envp);
		error("Execve failed");
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
		}
		else
			return(this->errorResponse(500));
		close(fd[0]);
		return response;
	}
}
