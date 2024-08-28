/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_validator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 13:45:54 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/28 17:09:09 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/so_long.h"

void	flood_fill(t_map_params *params, int x, int y, t_count *counts)
{
	if (x < 0 || y < 0 || x >= params->width || y >= params->height \
	|| params->map[y][x] == '1' || params->map[y][x] == 'F')
		return ;
	if (params->map[y][x] == 'E')
	{
		counts->exit_found++;
		return ;
	}
	if (params->map[y][x] == 'C')
		counts->c_count--;
	params->map[y][x] = 'F';
	flood_fill(params, x + 1, y, counts);
	flood_fill(params, x - 1, y, counts);
	flood_fill(params, x, y + 1, counts);
	flood_fill(params, x, y - 1, counts);
}

void	validate_start_pos(t_count counts, t_map_params *params)
{
	if (counts.p_x == -1 || counts.p_y == -1)
	{
		free_map_memory(params->map, params->height);
		exit_with_error33("Error: Player start position not found", params->fd);
	}
}

void	validate_path(t_count counts, t_map_params *params)
{
	if (counts.c_count)
	{
		free_map_memory(params->map, params->height);
		exit_with_error33("Error: No valid path to collect all \
		collectibles", params->fd);
	}
	if (!counts.exit_found)
	{
		free_map_memory(params->map, params->height);
		exit_with_error33("Error: No valid path to reach the exit", params->fd);
	}
}

void	validate_map_path(char *filename)
{
	t_map_params	params;
	t_count			counts;
	int				fd;

	counts = (t_count){0, 0, 0, -1, -1, 0, 0, 0};
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		exit_with_error33("Error opening file", fd);
	params.fd = fd;
	params.map = read_map(fd, &counts, &params.width, &params.height);
	validate_start_pos(counts, &params);
	flood_fill(&params, counts.p_x, counts.p_y, &counts);
	validate_path(counts, &params);
	free_map_memory(params.map, params.height);
	close(fd);
}
