/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 14:27:36 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/18 18:55:47 by pghajard         ###   ########.fr       */
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

char	**expand_map(t_map_info *info, char *line)
{
	char	**new_map;
	int		i;

	i = 0;
	info->map_capacity *= 2;
	new_map = malloc(info->map_capacity * sizeof(char *));
	if (!new_map)
		exit_with_error("Error: Memory allocation failed", info->fd, line);
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
	char		*line;

	info.map_capacity = INITIAL_MAP_SIZE;
	info.map = malloc(info.map_capacity * sizeof(char *));
	info.height = 0;
	info.fd = fd;
	if (!info.map)
		exit_with_error("Error: Memory allocation failed", fd, NULL);
	line = get_next_line(fd);
	while (line != NULL)
	{
		if (info.height == 0)
			*width = ft_strlen(line);
		if (info.height >= info.map_capacity)
			info.map = expand_map(&info, line);
		info.map[info.height] = line;
		track_map_info(line, counts, &info.height);
		info.height++;
		line = get_next_line(fd);
	}
	*height = info.height;
	return (info.map);
}
