/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   commands.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viaremko <lodyiaremko@proton.me>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 18:46:43 by viaremko          #+#    #+#             */
/*   Updated: 2025/06/22 16:00:20 by viaremko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../lib/pipex.h"

char	**find_candidate(char **envp)
{
	int	i;

	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=/", 6) == 0)
			return (ft_split(&envp[i][5], ':'));
		i++;
	}
	ft_error(3, "Error: corrupted env variable\n");
	return (NULL);
}

char	***create_commands_array(t_pipex *px, char **argv, int argc)
{
	int		i;
	char	***commands;

	commands = ft_calloc((argc - 2 - px->heredoc) * sizeof(char **));
	i = 0;
	while (i + 2 + px->heredoc != argc - 1)
	{
		commands[i] = ft_split(argv[i + 2 + px->heredoc], ' ');
		i++;
	}
	commands[i] = NULL;
	return (commands);
}

char	*find_path(char **candidates, char *command)
{
	char	*path;
	int		i;

	i = 0;
	while (candidates[i] != NULL)
	{
		if (access(command, X_OK) == 0)
			return (ft_strdup(command));
		path = ft_tstrjoin(candidates[i], "/", command);
		if (access(path, X_OK) == 0)
			return (path);
		free(path);
		i++;
	}
	return (NULL);
}

char	**get_paths_array(t_pipex *px, char **candidates, char ***commands,
		int argc)
{
	int		i;
	char	**paths;

	paths = ft_calloc((argc - 3 - px->heredoc) * sizeof(char *));
	i = -1;
	while (++i < argc - 3 - px->heredoc)
		paths[i] = find_path(candidates, commands[i][0]);
	return (paths);
}

void	path_check(t_pipex *px, int argc)
{
	int	i;
	int	null_counter;

	i = 0;
	null_counter = 0;
	while (i < argc - 3 - px->heredoc)
	{
		if (px->paths[i] == NULL)
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
