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

void command_access_check(char **paths, char **commands)
{
	char *path;
	int i;
	int j;

	j = -1;
	while(paths[++j] != NULL)
	{
		i = -1;
		while(commands[++i] != NULL)
		{
			path = ft_strjoin(paths[j], commands[i]);
			if(access(path, X_OK) != 0)
				free(path);
			else
			{
				ft_printf("\nPath found for %s%s\n\n", paths[j], commands[i]);
				free(path);
			}
		}
	}
}

char** find_path(char **envp)
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
		ft_printf("\nString: %s\n\n", argv[i + 2]);
		tmp_commands[i] = strdup(argv[i + 2]); 
		ft_printf("Command: %s\n", tmp_commands[i]);
		i++;
	}
	while(i != -1)
	{
		commands[i] = ft_strjoin("/", tmp_commands[i]);
		free(tmp_commands[i]);
		i--;
	}
	free(tmp_commands);
	return(commands);
}
	
int main(int argc, char **argv, char **envp)
{
	char** PATH;
	char** COMMANDS;	
	if (argc < 5)
	{
		ft_printf("Usage: ./pipex file1 cmd1 cmd2 file2\n");	
		return(1);
	}	if (file_access_check(argc, argv) == -1)
		printf("Access: KO!\n");
	PATH = find_path(envp);                        // <-- FREE ME PLEASE, I'M BEGGING YOU
	COMMANDS = create_commands_array(argv, argc);  // <-- FREE ME AS WELL
	command_access_check(PATH,COMMANDS);
	return (0);
}
