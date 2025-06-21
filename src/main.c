#include "../lib/pipex.h"

void clean_heredoc(int heredoc_status)
{
	if(heredoc_status == 1)
	{
		get_next_line(-1);
		unlink(".heredoc");
	}
}
void cleanup(t_pipex *px)
{
	int i;
	int j;
	
	i = 0;
	while(px->commands[i] != NULL)
	{
		j = 0;
		while(px->commands[i][j] != NULL)
		{
			free(px->commands[i][j]);
			j++;
		}
		free(px->pipes[i]);
		free(px->paths[i]);
		free(px->commands[i]);
		i++;
	}
	i=-1;
	while(px->candidates[++i])
		free(px->candidates[i]);
	free(px->candidates);
	free(px->pipes);
	free(px->paths);
	free(px->commands);
	clean_heredoc(px->heredoc);
}

void pipe_cleaner(t_pipex *px, int argc)
{
	int i;
	
	i = -1;
	while(++i < argc - 3 - px->heredoc)
	{
		close(px->pipes[i][0]);
		close(px->pipes[i][1]);
	}
	close(px->infile_fd);
	close(px->outfile_fd);
}
void ft_error(int exit_code, char *msg)
{
	ft_putstr_fd(msg, 2);
	exit(exit_code);
}

void null_command_check(int argc, char **argv)
{
	int i;
	char *trimmed_str;	
	i = 2;
	while(i != argc-1)
	{
		trimmed_str = ft_strtrim(argv[i]," 	\n");
		if(ft_strncmp(trimmed_str,"",1) == 0)
		{
			free(trimmed_str);
			ft_error(888, "Null argument found\n");
		}
		free(trimmed_str);
		i++;
	}
}

void open_IO_files(t_pipex *px, int *infile_fd, int *outfile_fd, int argc, char **argv)
{
	int infile;
	int outfile;

	infile = 1;
	outfile = argc-1;

	if(px->heredoc == 0)
	{
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
	else
		*outfile_fd = open(argv[outfile],O_CREAT | O_WRONLY | O_APPEND, 0644);
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

char*** create_commands_array(t_pipex *px, char **argv, int argc)
{
	int i;
	char ***commands;
	commands = ft_calloc((argc - 2 - px->heredoc) * sizeof(char**));
	i = 0;

	while(i + 2 + px->heredoc != argc-1)
	{
		commands[i] = ft_split(argv[i + 2 + px->heredoc], ' '); 
		i++;
	}
	commands[i] = NULL;
	return(commands);
}

char* find_path(char **candidates, char *command)
{
	char *path;
	int i;

	i = 0;
	while(candidates[i] != NULL )
	{
		if(access(command, X_OK) == 0)
			return(ft_strdup(command));
		path = ft_tstrjoin(candidates[i], "/", command);
		if(access(path, X_OK) == 0)
			return(path);
		free(path);
		i++;
	}
	return(NULL);
}

char** get_paths_array(t_pipex *px,char **candidates, char ***commands, int argc)
{
	int i;
	char **paths;
	
	paths = ft_calloc((argc - 3 - px->heredoc) * sizeof(char*));
	i = -1;
	while(++i < argc - 3 - px->heredoc)
		paths[i] = find_path(candidates, commands[i][0]);
	return(paths);
}

void path_check(t_pipex *px, int argc)
{
	int i;
	int null_counter;

	i = 0;
	null_counter = 0;
	while(i < argc - 3 - px->heredoc)
	{
		if(px->paths[i] == NULL)
			null_counter++;
		i++;
	}
	if (null_counter != 0)
	{
		pipe_cleaner(px, argc);
		cleanup(px);
		ft_error(4, "Error: command not found\n");
	}
}

void open_pipes(t_pipex *px, int ***pipes,int argc)
{
	int pipe_index;
	int total_pipes;		

	pipe_index = 0;
	total_pipes = argc - 3 - px->heredoc;
	*pipes = ft_calloc(total_pipes * sizeof(int*));
	if(!*pipes)
		ft_error(100,"Pipe malloc falied\n");
	while(pipe_index < total_pipes)
	{
		(*pipes)[pipe_index] = ft_calloc(2 * sizeof(int));
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
void child_process(t_pipex *px, int argc)
{
	if(px->heredoc == 1)
	{
		close(px->infile_fd);
		px->infile_fd = open(".heredoc", O_RDONLY, 0644);
	}
	if (px->cmd_index == 0) //first command
		dup2(px->infile_fd, STDIN_FILENO); // if heredoc -> pass heredoc as infile_fd;
	else
		dup2(px->prev_read_fd, STDIN_FILENO);

	if( px->cmd_index == argc - 4 - px->heredoc) //argc -4 is the last command
		dup2(px->outfile_fd, STDOUT_FILENO);
	else
		dup2(px->pipes[px->cmd_index][1], STDOUT_FILENO);
	execve(px->paths[px->cmd_index],px->commands[px->cmd_index],px->envp);
	pipe_cleaner(px, argc);
	cleanup(px);
	ft_error(126, "Child process falied\n");
}

pid_t start_pipex(t_pipex *px, int argc)
{
	pid_t pid;
//	int sig;	
	pid = fork();
	if (pid == 0) //child
		child_process(px, argc);
	else if (pid > 0) //parent 
	{
		close(px->pipes[px->cmd_index][1]);
		close(px->prev_read_fd);
		px->prev_read_fd = px->pipes[px->cmd_index][0];
		if(px->cmd_index == argc - 4 - px->heredoc)
			close(px->pipes[px->cmd_index][0]);
	        //waitpid(pid, &sig, 0);
		//printf("%d\n", sig % 255);
	}
	return(pid);
}

void heredoc(t_pipex *px)
{
	char *line;

	px->infile_fd = open(".heredoc", O_CREAT | O_RDWR | O_TRUNC, 0644);
	while (1)
	{
		line = get_next_line(0);
		if (!line || !*line)
			break;
		if(ft_strncmp(line, px->heredoc_EOF, ft_strlen(line)-1) == 0)
			break;
		ft_putstr_fd(line,px->infile_fd);
		free(line);
	}
	free(line);
}

void heredoc_check(t_pipex *px,char **argv, int argc)
{
	if(argc == 5)
		ft_error(9898, "Not enough arguments for piping\n");
	if(ft_strncmp("here_doc", argv[1], ft_strlen(argv[1])) == 0)
	{
		px->heredoc = 1;
		px->heredoc_EOF = argv[2];
		heredoc(px);
	}
}


int main(int argc, char **argv, char **envp)
{
	static t_pipex	px;

	if (argc < 5)
		ft_error(1, "Not enough arguments\nUsage: ./pipex file1 cmd1 cmd2 file2\n");
	null_command_check(argc, argv);
	heredoc_check(&px, argv, argc);
	px.candidates = find_candidate(envp);
	open_IO_files(&px, &px.infile_fd, &px.outfile_fd, argc, argv);
	px.commands = create_commands_array(&px, argv, argc);
	px.paths = get_paths_array(&px, px.candidates, px.commands, argc);
	open_pipes(&px, &px.pipes, argc);
	path_check(&px, argc);
	px.envp = envp;	
	px.cmd_index = 0;
	px.total_cmds = argc - 3 - px.heredoc;
	while(px.cmd_index < px.total_cmds)
	{
		start_pipex(&px, argc);
		px.cmd_index++;
	}
	while (wait(NULL) > 0)
    		;
	close(px.infile_fd);
	close(px.outfile_fd);
	cleanup(&px);

	close(2);
	close(1);
}
