/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 14:27:36 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/18 23:01:44 by pghajard         ###   ########.fr       */
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

void	read_and_process_map(int fd, t_count *counts, int *width, int *height)
{
	char	c;
	int		line_len;
	int		line_num;

	line_len = 0;
	line_num = 0;

	while (read(fd, &c, 1) == 1)
	{
		if (c == '\n')
		{
			if (line_num == 0)
				*width = line_len;
			else if (line_len != *width)
				exit_with_error33("Error: Map is not rectangular", fd, NULL);

			line_num++;
			line_len = 0;
		}
		else
		{
			if (line_num == 0)
				line_len++;
			else if (line_len >= *width)
				exit_with_error33("Error: Line length exceeds expected width", fd, NULL);

			// Process the character directly
			if (c == 'P')
			{
				if (counts->p_x != -1 || counts->p_y != -1)
					exit_with_error33("Error: Multiple player start positions found", fd, NULL);
				counts->p_x = line_len;
				counts->p_y = line_num;
				counts->p_count++;
			}
			else if (c == 'C')
				counts->c_count++;
			else if (c == 'E')
				counts->e_count++;
			else if (c != '0' && c != '1')
				exit_with_error33("Error: Invalid character in map", fd, NULL);

			line_len++;
		}
	}

	if (line_len != 0)
	{
		if (line_len != *width)
			exit_with_error33("Error: Last line is not rectangular", fd, NULL);
		line_num++;
	}

	*height = line_num;

	// Additional validation can go here if necessary
	if (counts->p_count != 1 || counts->c_count < 1 || counts->e_count != 1)
		exit_with_error33("Error: Invalid map composition", fd, NULL);
}

////// you can do read_and_process_map without malloc I guess. 