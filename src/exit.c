/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 12:29:13 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/17 16:51:22 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	exit_with_error(char *message, int fd, char *line)
{
	if (line)
		free(line);
	if (fd >= 0)
		close(fd);
	printf("%s\n", message);
	exit(EXIT_FAILURE);
}

void	handle_error_and_exit(int fd, \
char **map, char *buffer, t_game *game)
{
	free(map);
	if (buffer)
		free(buffer);
	if (fd >= 0)
		close(fd);
	if (game)
		cleanup(game);
	printf("%s", "Error");
	exit(1);
}

void	handle_exit(t_game *game)
{
	printf("Congratulations, you've won!\n");
	cleanup(game);
	exit(0);
}
