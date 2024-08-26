/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_drawing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 13:45:42 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/26 10:25:35 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/so_long.h"

void	process_map_char(t_game *game, char c, int x, int y)
{
	if (c == 'P')
	{
		game->p_x = x;
		game->p_y = y;
	}
	else if (c == 'C')
	{
		game->collect_count++;
		mlx_put_image_to_window(game->mlx, game->win, \
		game->collect, x * 100, y * 100);
	}
	else if (c == '1')
		mlx_put_image_to_window(game->mlx, game->win, \
		game->wall, x * 100, y * 100);
	else if (c == '0')
		mlx_put_image_to_window(game->mlx, game->win, \
		game->ground, x * 100, y * 100);
	else if (c == 'E')
		mlx_put_image_to_window(game->mlx, game->win, \
		game->end, x * 100, y * 100);
	else if (c == 'P')
		mlx_put_image_to_window(game->mlx, game->win, \
		game->player, x * 100, y * 100);
}

void	read_and_draw_map(t_game *game, int fd, char *buffer)
{
	int	x;
	int	y;

	x = 0;
	y = 0;
	while (read(fd, buffer, 1) > 0)
	{
		if (buffer[0] == '\n')
		{
			y++;
			x = 0;
		}
		else
		{
			if (x >= game->width || y >= game->height)
				handle_error_and_exit(fd, game->map, buffer, game);
			game->map[y][x] = buffer[0];
			process_map_char(game, buffer[0], x, y);
			x++;
		}
	}
}

void	allocate_map_memory(t_game *game, int fd, char *buffer)
{
	int	i;

	game->map = (char **)malloc(sizeof(char *) * game->height);
	if (!game->map)
	{
		cleanup(game);
		handle_error_and_exit(fd, NULL, buffer, game);
	}
	i = 0;
	while (i < game->height)
	{
		game->map[i] = (char *)malloc(sizeof(char) * (game->width + 1));
		if (!game->map[i])
			return ;
		i++;
	}
}

void	draw_map(t_game *game, const char *filename)
{
	int		fd;
	char	*buffer;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		handle_error_and_exit(-1, NULL, NULL, game);
	buffer = (char *)malloc(sizeof(char) * 2);
	if (!buffer)
		handle_error_and_exit(fd, NULL, NULL, game);
	allocate_map_memory(game, fd, buffer);
	read_and_draw_map(game, fd, buffer);
	free(buffer);
	close(fd);
}
