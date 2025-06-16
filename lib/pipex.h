#ifndef PIPEX_H
# include <fcntl.h>
# include <sys/wait.h>
# include "../lib/libft/libft.h"
# include <stdio.h>
# include <unistd.h>
# include <string.h>
# include <errno.h>

typedef struct s_pipex
{
    int	    prev_read_fd;
    char**  candidates;
    char    **paths;        // Executable full paths for each cmd
    char    ***commands;    // Arguments for each command
    char    **envp;
    int     infile_fd;
    int     outfile_fd;
    int     **pipes;
    int     cmd_index;      // Current command number in loop
    int     total_cmds;     // Total number of commands
}   t_pipex;

#endif
