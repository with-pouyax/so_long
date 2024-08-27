/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 12:34:09 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/27 17:05:17 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/so_long.h"

void	init_game_struct(t_game *game)
{
	game->mlx = NULL;
	game->win = NULL;
	game->wall = NULL;
	game->ground = NULL;
	game->collect = NULL;
	game->end = NULL;
	game->player = NULL;
	game->map = NULL;
	game->width = 0;
	game->height = 0;
	game->p_x = 0;
	game->p_y = 0;
	game->collect_count = 0;
	game->collected = 0;
	game->moves = 0;
}

void	update_dimensions(int *width, int *height, \
int line_length, int first_line)
{
	if (first_line)
	{
		*width = line_length;
		if (*width > MAX_WIDTH)
		{
			ft_printf("%s", "Line width exceeds maximum allowed size\n");
			exit(1);
		}
	}
	(*height)++;
	if (*height > MAX_HEIGHT)
	{
		ft_printf("%s", "Number of lines exceeds maximum allowed size\n");
		exit(1);
	}
}

void	process_line(int fd, int *width, int *height)
{
	char	*buffer;
	int		line_length;
	int		first_line;

	buffer = malloc(2 * sizeof(char));
	if (!buffer)
		ft_error("Error allocating memory", fd, NULL);
	line_length = 0;
	first_line = 1;
	while (read(fd, buffer, 1) > 0)
	{
		if (buffer[0] == '\n')
		{
			update_dimensions(width, height, line_length, first_line);
			first_line = 0;
			line_length = 0;
		}
		else
			line_length++;
	}
	if (line_length > 0)
		update_dimensions(width, height, line_length, first_line);
	free(buffer);
}

void	read_file_and_update(const char *filename, int *width, int *height)
{
	int		fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		ft_error("Error reading file", fd, NULL);
	process_line(fd, width, height);
	if (*height == 0)
		ft_error("File is empty or does not contain valid lines", fd, NULL);
	close(fd);
}

void	get_map_dimensions(const char *filename, int *width, int *height)
{
	*width = 0;
	*height = 0;
	read_file_and_update(filename, width, height);
}
