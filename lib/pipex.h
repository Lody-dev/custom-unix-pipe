/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viaremko <lodyiaremko@proton.me>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/02 09:03:15 by viaremko          #+#    #+#             */
/*   Updated: 2024/10/02 09:53:51 by viaremko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#ifndef PIPEX_H
# define PIPEX_H
# include "../lib/libft/src/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

typedef struct s_pipex
{
	int		prev_read_fd;
	char	**candidates;
	char	**paths;
	char	***commands;
	char	**envp;
	int		infile_fd;
	int		outfile_fd;
	int		**pipes;
	int		cmd_index;
	int		total_cmds;
	int		pipe_heredoc[2];
	int		heredoc;
	char	*heredoc_eof;
}			t_pipex;

/* utils.c */
void		ft_error(int exit_code, char *msg);
void		null_command_check(int argc, char **argv);
void		clean_heredoc(int heredoc_status);
void		cleanup(t_pipex *px);
void		pipe_cleaner(t_pipex *px, int argc);

/* file_io.c */
void		open_io_files(t_pipex *px, int argc, char **argv);
void		heredoc(t_pipex *px);
void		heredoc_check(t_pipex *px, char **argv, int argc);

/* pipes.c */
void		open_pipes(t_pipex *px, int ***pipes, int argc);
pid_t		start_pipex(t_pipex *px, int argc);
void		child_process(t_pipex *px, int argc);

/* commands.c */
char		**find_candidate(char **envp);
char		***create_commands_array(t_pipex *px, char **argv, int argc);
char		*find_path(char **candidates, char *command);
char		**get_paths_array(t_pipex *px, char **candidates, char ***commands,
				int argc);
void		path_check(t_pipex *px, int argc);

#endif
