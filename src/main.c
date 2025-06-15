#include "../lib/pipex.h"

void ft_error(int exit_code, char *msg)
{
	ft_putstr_fd(msg, 2);
	exit(exit_code);
}

void open_IO_files(int *infile_fd, int *outfile_fd, int argc, char **argv)
{
	int infile;
	int outfile;

	infile = 1;
	outfile = argc-1;
	
	*outfile_fd = open(argv[outfile],O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if(*outfile_fd < 0)
	{
		ft_error(1,"Can't create/open outfile\n");
	}
	*infile_fd = open(argv[infile], O_RDONLY);
	if(*infile_fd < 0)
	{
		close(*outfile_fd);
		ft_error(1,"Can't open infile\n");
	}
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

void open_pipes(int ***pipes,int argc)  //one more malloc happend here. TODO FREE();
{
	int pipe_index;
	int total_pipes;		

	pipe_index = 0;
	total_pipes = argc - 3;
	*pipes = malloc(total_pipes * sizeof(int*));
	if(!*pipes)
		ft_error(100,"Pipe malloc falied\n");
	while(pipe_index < total_pipes)
	{
		(*pipes)[pipe_index] = malloc(2 * sizeof(int));
		if(!(*pipes)[pipe_index] || pipe((*pipes)[pipe_index]) == -1)
			{
				while(pipe_index > 0)
				{
					pipe_index--;
					free((*pipes)[pipe_index]);
				}
				free(*pipes);
				ft_error(100, "Pipe malloc falied\n");
			}
		pipe_index++;
	}	
}

pid_t start_pipex(t_pipex *px, int argc)
{
	pid_t pid;
		
	pid = fork();
	if (pid == 0) //child
	{
		if (px->cmd_index == 0) //first command
			dup2(px->infile_fd, STDIN_FILENO);
		else
			dup2(px->prev_read_fd, STDIN_FILENO);

		if( px->cmd_index == argc - 4) //argc -4 is the last command
			dup2(px->outfile_fd, STDOUT_FILENO);
		else
			dup2(px->pipes[px->cmd_index][1], STDOUT_FILENO);

		execve(px->paths[px->cmd_index],px->commands[px->cmd_index],px->envp);
		//free everything in child
		ft_error(127, "Child process falied");
	}
	else if (pid > 0) //parent 
	{
		close(px->pipes[px->cmd_index][1]);
		close(px->prev_read_fd);
		px->prev_read_fd = px->pipes[px->cmd_index][0];

		waitpid(pid, NULL, 0);
	}
	else //error
		ft_error(1,"Child process falied\n");
	return(pid);
}

int main(int argc, char **argv, char **envp)
{
	static t_pipex	px;
	char** candidates;

	if (argc < 5)
		ft_error(1, "Not enough arguments\nUsage: ./pipex file1 cmd1 cmd2 file2\n");
	open_IO_files(&px.infile_fd, &px.outfile_fd, argc, argv);

	candidates = find_candidate(envp);			// <-- FREE ME PLEASE, I'M BEGGING YOU
	px.commands = create_commands_array(argv, argc);		// <-- FREE ME AS WELL
	px.paths = get_paths_array(candidates, px.commands, argc);    // <-- ^^^^ ^^ ^^ ^^^^
	path_check(px.paths, argc);
	
	px.cmd_index = 0;
	px.total_cmds = argc - 3;
	px.envp = envp;
	open_pipes(&px.pipes, argc);
	while(px.cmd_index < px.total_cmds)
	{
		start_pipex(&px, argc);
		px.cmd_index++;
	}
	ft_printf("OK\n");
	return (0);
}
