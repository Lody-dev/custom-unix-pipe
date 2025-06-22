/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   files_io.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viaremko <lodyiaremko@proton.me>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 18:46:43 by viaremko          #+#    #+#             */
/*   Updated: 2025/06/22 16:48:56 by viaremko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../lib/pipex.h"

void	open_io_files(t_pipex *px, int argc, char **argv)
{
	int	infile;
	int	outfile;

	infile = 1;
	outfile = argc - 1;
	if (px->heredoc == 0)
	{
		px->outfile_fd = open(argv[outfile], O_CREAT | O_WRONLY | O_TRUNC,
				0644);
		if (px->outfile_fd < 0)
		{
			ft_error(1, "Can't create/open outfile\n");
		}
		px->infile_fd = open(argv[infile], O_RDONLY);
		if (px->infile_fd < 0)
		{
			close(px->outfile_fd);
			ft_error(1, "Can't open infile\n");
		}
	}
	else
		px->outfile_fd = open(argv[outfile], O_CREAT | O_WRONLY | O_APPEND,
				0644);
}

void	heredoc(t_pipex *px)
{
	char	*line;

	px->infile_fd = open(".heredoc", O_CREAT | O_RDWR | O_TRUNC, 0644);
	while (1)
	{
		line = get_next_line(0);
		if (!line || !*line)
			break ;
		if (ft_strncmp(line, px->heredoc_eof, ft_strlen(line) - 1) == 0)
			break ;
		ft_putstr_fd(line, px->infile_fd);
		free(line);
	}
	free(line);
}

void	heredoc_check(t_pipex *px, char **argv, int argc)
{
	if (ft_strncmp("here_doc", argv[1], ft_strlen(argv[1])) == 0)
	{
		if (argc == 5)
			ft_error(9898, "Not enough arguments for piping\n");
		px->heredoc = 1;
		px->heredoc_eof = argv[2];
		heredoc(px);
	}
}
