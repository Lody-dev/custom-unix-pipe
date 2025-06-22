/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viaremko <lodyiaremko@proton.me>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 18:46:43 by viaremko          #+#    #+#             */
/*   Updated: 2025/06/22 16:00:20 by viaremko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../lib/pipex.h"

void	child_process(t_pipex *px, int argc)
{
	if (px->heredoc == 1)
	{
		close(px->infile_fd);
		px->infile_fd = open(".heredoc", O_RDONLY, 0644);
	}
	if (px->cmd_index == 0)
		dup2(px->infile_fd, STDIN_FILENO);
	else
		dup2(px->prev_read_fd, STDIN_FILENO);
	if (px->cmd_index == argc - 4 - px->heredoc)
		dup2(px->outfile_fd, STDOUT_FILENO);
	else
		dup2(px->pipes[px->cmd_index][1], STDOUT_FILENO);
	execve(px->paths[px->cmd_index], px->commands[px->cmd_index], px->envp);
	pipe_cleaner(px, argc);
	cleanup(px);
	ft_error(126, "Child process falied\n");
}

pid_t	start_pipex(t_pipex *px, int argc)
{
	pid_t	pid;

	pid = fork();
	if (pid == 0)
		child_process(px, argc);
	else if (pid > 0)
	{
		close(px->pipes[px->cmd_index][1]);
		close(px->prev_read_fd);
		px->prev_read_fd = px->pipes[px->cmd_index][0];
		if (px->cmd_index == argc - 4 - px->heredoc)
			close(px->pipes[px->cmd_index][0]);
	}
	return (pid);
}

void	open_pipes(t_pipex *px, int ***pipes, int argc)
{
	int	pipe_index;
	int	total_pipes;

	pipe_index = 0;
	total_pipes = argc - 3 - px->heredoc;
	*pipes = ft_calloc(total_pipes * sizeof(int *));
	if (!*pipes)
		ft_error(100, "Pipe malloc falied\n");
	while (pipe_index < total_pipes)
	{
		(*pipes)[pipe_index] = ft_calloc(2 * sizeof(int));
		if (!(*pipes)[pipe_index] || pipe((*pipes)[pipe_index]) == -1)
		{
			while (pipe_index > 0)
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
