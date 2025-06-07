#include <fcntl.h>
#include "../lib/libft/libft.h"
#include <stdio.h>
#include <unistd.h>

int file_access_check(int argc, char **argv)
{
	if (access(argv[1], R_OK) != 0)
		return (printf("Can't access %s\n", argv[1]),-1);
	if (access(argv[argc - 1], W_OK) == -1)
	{
		open(argv[argc - 1], O_WRONLY | O_CREAT, 0644);
		if (access(argv[argc - 1], F_OK) == -1)
			return (printf("Can't create %s\n", argv[argc-1]), -1);
		if (access(argv[argc - 1], W_OK) == -1)
			return (printf("Can't write on %s file\n", argv[argc-1]), -1);
	}
	return(printf("Files are accessible\n"), 0);
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
	ft_printf("Error: corrupted env variable\n");
	exit(0);
}

char** create_commands_array(char **argv, int argc)
{
	int i;
	char **tmp_commands;
	char **commands;
	ft_printf("\nArgc: %d\n",argc);
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

int main(int argc, char **argv, char **envp)
{
	char** candidates;
	char** commands;	
	char** paths;
	if (argc < 5)
	{
		ft_printf("Usage: ./pipex file1 cmd1 cmd2 file2\n");	
		return(1);
	}
	if (file_access_check(argc, argv) == -1)
		printf("Access: KO!\n");
	candidates = find_candidate(envp);			// <-- FREE ME PLEASE, I'M BEGGING YOU
	commands = create_commands_array(argv, argc);		// <-- FREE ME AS WELL
	paths = get_paths_array(candidates, commands, argc);
	//command_access_check(candidates,commands);
	
	int i = 0;
	while(i < argc-3)
	{
		ft_printf("Path: %s\n", paths[i]);
		i++;
	}
	return (0);
}
