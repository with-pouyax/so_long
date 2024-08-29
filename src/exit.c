/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 12:29:13 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/29 15:34:01 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/so_long.h"

void	exit_with_error(char *message, int fd, char *line)
{
	if (line)
		free(line);
	if (fd >= 0)
		close(fd);
	ft_printf("%s\n", message);
	exit(EXIT_FAILURE);
}

void	handle_error_and_exit(int fd, \
char **map, char *buffer, t_game *game)
{
	(void)buffer;
	free(map);
	if (fd >= 0)
		close(fd);
	if (game)
		cleanup(game);
	ft_printf("%s", "Error");
	exit(1);
}

void	handle_exit(t_game *game)
{
	ft_printf("Congratulations, you've won!\n");
	cleanup(game);
	exit(0);
}

void	exit_with_error33(char *message, int fd)
{
	if (fd >= 0)
		close(fd);
	ft_printf("%s\n", message);
	exit(EXIT_FAILURE);
}

void	cleanup_1(t_game *game)
{
	free_images(game);
	if (game->map)
		free(game->map);
	if (game->win)
	{
		mlx_destroy_window(game->mlx, game->win);
		game->win = NULL;
	}
	if (game->mlx)
	{
		mlx_destroy_display(game->mlx);
		free(game->mlx);
		game->mlx = NULL;
	}
}
