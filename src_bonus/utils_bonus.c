/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viaremko <lodyiaremko@proton.me>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 18:46:43 by viaremko          #+#    #+#             */
/*   Updated: 2025/06/22 16:00:20 by viaremko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../lib/pipex.h"

void	clean_heredoc(int heredoc_status)
{
	if (heredoc_status == 1)
	{
		get_next_line(-1);
		unlink(".heredoc");
	}
}

void	cleanup(t_pipex *px)
{
	int	i;
	int	j;

	i = 0;
	while (px->commands[i] != NULL)
	{
		j = 0;
		while (px->commands[i][j] != NULL)
		{
			free(px->commands[i][j]);
			j++;
		}
		free(px->pipes[i]);
		free(px->paths[i]);
		free(px->commands[i]);
		i++;
	}
	i = -1;
	while (px->candidates[++i])
		free(px->candidates[i]);
	free(px->candidates);
	free(px->pipes);
	free(px->paths);
	free(px->commands);
	clean_heredoc(px->heredoc);
}

void	pipe_cleaner(t_pipex *px, int argc)
{
	int	i;

	i = -1;
	while (++i < argc - 3 - px->heredoc)
	{
		close(px->pipes[i][0]);
		close(px->pipes[i][1]);
	}
	close(px->infile_fd);
	close(px->outfile_fd);
}

void	ft_error(int exit_code, char *msg)
{
	ft_putstr_fd(msg, 2);
	exit(exit_code);
}

void	null_command_check(int argc, char **argv)
{
	int		i;
	char	*trimmed_str;

	i = 2;
	while (i != argc - 1)
	{
		trimmed_str = ft_strtrim(argv[i], " 	\n");
		if (ft_strncmp(trimmed_str, "", 1) == 0)
		{
			free(trimmed_str);
			ft_error(888, "Null argument found\n");
		}
		free(trimmed_str);
		i++;
	}
}
