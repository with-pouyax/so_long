/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_drawing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 13:45:42 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/29 14:01:07 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/so_long.h"

void	read_and_draw_map(t_game *game, int fd, char *buffer)
{
	t_map_data	data;

	data.x = 0;
	data.y = 0;
	data.game = game;
	data.fd = fd;
	while (read(fd, buffer, 1) > 0)
	{
		if (buffer[0] == '\n')
			handle_newline(&data.x, &data.y);
		else
			process_map_character(game, &data, buffer);
	}
}

void	handle_error_and_exit_1(int fd, \
char **map, char *buffer, t_game *game)
{
	if (map)
		free(map);
	if (buffer)
		free(buffer);
	if (fd >= 0)
		close(fd);
	if (game)
		cleanup_1(game);
	ft_printf("%s", "Error");
	exit(1);
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
			handle_error_and_exit_1(fd, NULL, buffer, game);
		i++;
	}
}

void	draw_map(t_game *game, const char *filename)
{
	int		fd;
	char	buffer[2];

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		handle_error_and_exit(-1, NULL, NULL, game);
	allocate_map_memory(game, fd, buffer);
	read_and_draw_map(game, fd, buffer);
	close(fd);
}
