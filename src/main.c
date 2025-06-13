#include <fcntl.h>
#include <sys/wait.h>
#include "../lib/libft/libft.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

void ft_error(int exit_code, char *msg)
{
	ft_putstr_fd(msg, 2);
	exit(exit_code);
}

int file_access_check(int argc, char **argv)
{
	if (strcmp(argv[argc - 1], argv[1]) == 0 && access(argv[1], R_OK) != 0)
	{
		open(argv[argc - 1], O_WRONLY | O_CREAT, 0644);
		return(-1);
	}
	if (access(argv[argc - 1], W_OK) == -1)
	{
		open(argv[argc - 1], O_WRONLY | O_CREAT, 0644);
		if (access(argv[argc - 1], F_OK) == -1)
			return (-1);
		if (access(argv[argc - 1], W_OK) == -1)
			return (-1);
	}
	if (access(argv[1], R_OK) != 0)
		return (-1);
	return (0);
}

char** find_candidate(char **envp)
{
	int i;
	
	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=/", 6) == 0)
			return(ft_split(&envp[i][5], ':'));
		i++;
	}		
	ft_error(3, "Error: corrupted env variable\n");
	return(NULL);
}

char*** create_commands_array(char **argv, int argc)
{
	int i;
	char ***commands;
	commands = malloc((argc - 3) * sizeof(char**));
	i = 0;

	while(i + 2 != argc-1)
	{
		commands[i] = ft_split(argv[i + 2], ' '); 
		i++;
	}
	return(commands);
}

char* find_path(char **candidates, char *command)
{
	char *path;
	int i;

	i = 0;
	while(candidates[i] != NULL)
	{
		path = ft_tstrjoin(candidates[i], "/", command);
		if(access(path, X_OK) == 0)
			return(path);
		free(path);
		i++;
	}
	return(NULL);
}

char** get_paths_array(char **candidates, char ***commands, int argc)
{
	int i;
	char **paths;

	paths = malloc((argc - 3) * sizeof(char*)); //DO NOT FORGET TO FREEEEEEEEE!!!!!!!!!!!!!!!11!!!!
	i = 0;
	while(i < argc-3)
	{
		paths[i] = find_path(candidates, commands[i][0]);
		i++;
	}
	return(paths);
}

void path_check(char **paths, int argc)
{
	int i;
	int null_counter;

	i = 0;
	null_counter = 0;
	while(i < argc-3)
	{
		if(paths[i] == NULL)
			null_counter++;
		i++;
	}
	if (null_counter != 0)
		ft_error(4, "Error: command not found\n");
}

void start_pipex(char *path, char **command, char **envp, char **argv)
{
	pid_t pid;
	int infile_fd;

	pid = fork();
	if (pid == 0) //child
	{
		infile_fd = open(argv[1], O_RDONLY);
		dup2(infile_fd, STDIN_FILENO);
		close(infile_fd); //infile_fd is immedeately closed because its value now passed to STDIN of this process. So infile_fd can be closed.
		execve(path,command,envp);
		ft_error(127, "Child process falied");
	}
	else if (pid > 0) //parent 
	{
		waitpid(pid, NULL, 0);
		ft_printf("I am parent process\n");
	}
	else if (pid < 0) //error
		ft_error(1,"Child process falied\n");
}

int main(int argc, char **argv, char **envp)
{
	char** candidates;
	char*** commands;
	char** paths;

	if (argc < 5)
		ft_error(1, "Not enough arguments\nUsage: ./pipex file1 cmd1 cmd2 file2\n");
	if (file_access_check(argc, argv) == -1)
		ft_error(2, "Can't access file\n");
	candidates = find_candidate(envp);			// <-- FREE ME PLEASE, I'M BEGGING YOU
	commands = create_commands_array(argv, argc);		// <-- FREE ME AS WELL
	paths = get_paths_array(candidates, commands, argc);    // <-- ^^^^ ^^ ^^ ^^^^
		
	int i = 0;
	while(i < argc-3)
	{
		ft_printf("Path: %s\n", paths[i]);
		i++;
	}
	path_check(paths, argc);
	i = 0;
	while(i < argc - 3)
	{
		start_pipex(paths[i],commands[i], envp, argv);
		i++;
	}
	ft_printf("OK\n");
	return (0);
}
