/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_reader.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 13:45:54 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/26 15:03:32 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/so_long.h"

void	track_map_info(char c, int x, int y, t_count *counts)
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

char	*allocate_map_line(int y, int width)
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

void	finalize_line(t_map_params *params, int *y)
{
	params->map[*y][params->line_len] = '\0';
	if (*y == 0)
		params->width = params->line_len;
	(*y)++;
	params->line_len = 0;
}

int	process_char(char c, t_map_params *params, t_count *counts, int y)
{
	if (params->line_len == 0)
	{
		params->map[y] = allocate_map_line(y, params->width);
		if (!params->map[y])
			return (-1);
	}
	params->map[y][params->line_len] = c;
	track_map_info(c, params->line_len, y, counts);
	params->line_len++;
	return (0);
}

char	**read_map(int fd, t_count *counts, int *width, int *height)
{
	t_map_params	params;
	char			c;
	int				y;

	params.map = malloc(MAX_HEIGHT * sizeof(char *));
	if (!params.map)
		exit_with_error33("Error: Memory allocation failed", fd);
	params.fd = fd;
	params.line_len = 0;
	y = 0;
	while (read(fd, &c, 1) == 1)
	{
		if (c == '\n')
			finalize_line(&params, &y);
		else if (process_char(c, &params, counts, y) == -1)
		{
			free_map_memory(params.map, y);
			exit_with_error33("Error: Memory allocation failed", fd);
		}
	}
	if (params.line_len != 0)
		finalize_line(&params, &y);
	*width = params.width;
	*height = y;
	return (params.map);
}
