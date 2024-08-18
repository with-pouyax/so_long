/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 14:27:36 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/18 22:53:25 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	track_map_info(char *line, t_count *counts, int *height)
{
	int	i;

	i = 0;
	while (line[i])
	{
		if (line[i] == 'P')
		{
			counts->p_x = i;
			counts->p_y = *height;
		}
		else if (line[i] == 'C')
			counts->e_count++;
		i++;
	}
}

char	**expand_map(t_map_info *info)
{
	char	**new_map;
	int		i;

	i = 0;
	info->map_capacity *= 2;
	new_map = malloc(info->map_capacity * sizeof(char *));
	if (!new_map)
		exit_with_error33("Error: Memory allocation failed", info->fd);
	while (i < info->height)
	{
		new_map[i] = info->map[i];
		i++;
	}
	free(info->map);
	return (new_map);
}

char	**read_map(int fd, t_count *counts, int *width, int *height)
{
	t_map_info	info;
	char		c;
	int			line_len;

	info.map_capacity = INITIAL_MAP_SIZE;
	info.map = malloc(info.map_capacity * sizeof(char *));
	info.height = 0;
	info.fd = fd;
	if (!info.map)
		exit_with_error33("Error: Memory allocation failed", fd);

	line_len = 0;
	while (read(fd, &c, 1) == 1)
	{
		if (c == '\n')
		{
			info.map[info.height][line_len] = '\0';
			if (info.height == 0)
				*width = line_len;
			info.height++;
			line_len = 0;
			if (info.height >= info.map_capacity)
				info.map = expand_map(&info);
		}
		else
		{
			if (line_len == 0)
				info.map[info.height] = malloc((*width + 1) * sizeof(char));
			info.map[info.height][line_len++] = c;
			if (c == 'P' || c == 'C')
				track_map_info(info.map[info.height], counts, &info.height);
		}
	}

	*height = info.height;
	return (info.map);
}
