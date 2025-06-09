#include <fcntl.h>
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

char** create_commands_array(char **argv, int argc)
{
	int i;
	char **tmp_commands;
	char **commands;
	tmp_commands = malloc((argc - 3) * sizeof(char*)); //argc - 3 because in "./pipex in cmd cmd out" argc = 5 y cantidad de cmd = 2
	commands = malloc((argc - 3) * sizeof(char*));
	i = 0;
	while(i + 2 != argc-1)
	{
		tmp_commands[i] = strdup(argv[i + 2]); 
		i++;
	}
	i--;
	while(i != -1)
	{
		commands[i] = ft_strjoin("/", tmp_commands[i]);
		free(tmp_commands[i]);
		i--;
	}
	free(tmp_commands);
	return(commands);
}

char* find_path(char **candidates, char *command)
{
	char *path;
	int i;

	i = 0;
	while(candidates[i] != NULL)
	{
		path = ft_strjoin(candidates[i], command);
		if(access(path, X_OK) == 0)
			return(path);
		free(path);
		i++;
	}
	return(NULL);
}

char** get_paths_array(char **candidates, char **commands, int argc)
{
	int i;
	char **paths;
	
	paths = malloc((argc - 3) * sizeof(char*)); //DO NOT FORGET TO FREEEEEEEEE!!!!!!!!!!!!!!!11!!!!
	i = 0;
	while(i < argc-3)
	{
		paths[i] = find_path(candidates, commands[i]);
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

int main(int argc, char **argv, char **envp)
{
	char** candidates;
	char** commands;	
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
	ft_printf("OK\n");
	return (0);
}
