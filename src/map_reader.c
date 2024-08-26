/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_reader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 13:45:54 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/26 13:16:43 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/so_long.h"

static char	*allocate_map_line(int y, int width)
{
	char	*line;

	if (y == 0)
		line = malloc((MAX_WIDTH + 1) * sizeof(char));
	else
		line = malloc((width + 1) * sizeof(char));
	if (!line)
		return (NULL);
	return (line);
}

static void finalize_line(char **map, int *line_len, int *y, int *width)
{
	map[*y][*line_len] = '\0';
	if (*y == 0)
		*width = *line_len;
	(*y)++;
	*line_len = 0;
}

static int process_char(char c, char **map, int *line_len, int y, t_count *counts, int width)
{
	if (*line_len == 0)
	{
		map[y] = allocate_map_line(y, width);
		if (!map[y])
			return (-1);
	}
	map[y][*line_len] = c;
	track_map_info(c, *line_len, y, counts);
	(*line_len)++;
	return (0);
}

char	**read_map(int fd, t_count *counts, int *width, int *height)
{
	char	**map;
	char	c;
	int		line_len;
	int		y;

	map = malloc(MAX_HEIGHT * sizeof(char *));
	if (!map)
		exit_with_error33("Error: Memory allocation failed", fd);
	line_len = 0;
	y = 0;
	while (read(fd, &c, 1) == 1)
	{
		if (c == '\n')
			finalize_line(map, &line_len, &y, width);
		else if (process_char(c, map, &line_len, y, counts, *width) == -1)
		{
			free_map_memory(map, y);
			exit_with_error33("Error: Memory allocation failed", fd);
		}
	}
	if (line_len != 0)
		finalize_line(map, &line_len, &y, width);
	*height = y;
	return (map);
}

static void	track_map_info(char c, int x, int y, t_count *counts)
{
	if (c == 'P')
	{
		counts->p_x = x;
		counts->p_y = y;
		counts->p_count++;
	}
	else if (c == 'C')
		counts->e_count++;
	else if (c == 'E')
		counts->exit_found = 1;
}
