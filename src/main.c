/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: viaremko <lodyiaremko@proton.me>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/06 18:46:43 by viaremko          #+#    #+#             */
/*   Updated: 2025/06/22 16:51:01 by viaremko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "../lib/pipex.h"

static void	close_io(int infile_fd, int outfile_fd)
{
	close(infile_fd);
	close(outfile_fd);
}

int	main(int argc, char **argv, char **envp)
{
	static t_pipex	px;

	if (argc < 5)
		ft_error(1,
			"Not enough arguments\nUsage: ./pipex file1 cmd1 cmd2 file2\n");
	null_command_check(argc, argv);
	heredoc_check(&px, argv, argc);
	open_io_files(&px, argc, argv);
	px.candidates = find_candidate(envp);
	px.commands = create_commands_array(&px, argv, argc);
	px.paths = get_paths_array(&px, px.candidates, px.commands, argc);
	open_pipes(&px, &px.pipes, argc);
	path_check(&px, argc);
	px.envp = envp;
	px.cmd_index = 0;
	px.total_cmds = argc - 3 - px.heredoc;
	while (px.cmd_index < px.total_cmds)
	{
		start_pipex(&px, argc);
		px.cmd_index++;
	}
	while (wait(NULL) > 0)
		;
	close_io(px.infile_fd, px.outfile_fd);
	cleanup(&px);
}
