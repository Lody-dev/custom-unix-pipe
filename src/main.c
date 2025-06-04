#include <fcntl.h>
#include "../lib/libft/libft.h"
#include <stdio.h>
#include <unistd.h>

int access_check(int argc, char **argv)
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

int main(int argc, char **argv, char **envp)
{
	char** PATH;
	
	if (argc < 5)
	{
		ft_printf("Usage: ./pipex file1 cmd1 cmd2 file2\n");	
		return(1);
	}
	if (access_check(argc, argv) == -1)
		printf("Access: KO!\n");
	PATH = find_path(envp);
	(void)PATH;
	return (0);
}
