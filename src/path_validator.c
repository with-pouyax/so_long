/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_validator.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 13:45:54 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/22 19:13:45 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"



static void flood_fill(t_map_params *params, int x, int y, t_count *counts)
{
    if (x < 0 || y < 0 || x >= params->width || y >= params->height ||
        params->map[y][x] == '1' || params->map[y][x] == 'F')
        return;
    if (params->map[y][x] == 'E')
    {
        if (counts->c_count == counts->e_count)
            counts->exit_found = 1;
        return;
    }
    if (params->map[y][x] == 'C')
        counts->c_count++;
    params->map[y][x] = 'F'; // Mark as visited
    flood_fill(params, x + 1, y, counts);
    flood_fill(params, x - 1, y, counts);
    flood_fill(params, x, y + 1, counts);
    flood_fill(params, x, y - 1, counts);
}

static void validate_start_pos(t_count counts, t_map_params *params)
{
    if (counts.p_x == -1 || counts.p_y == -1)
    {
        free_map_memory(params->map, params->height);
        exit_with_error33("Error: Player start position not found", params->fd);
    }
}

static void validate_path(t_count counts, t_map_params *params)
{
    if (counts.c_count != counts.e_count || !counts.exit_found)
    {
        free_map_memory(params->map, params->height);
        exit_with_error33("Error: No valid path to collect all collectibles and reach the exit", params->fd);
    }
}

static void track_map_info(char c, int x, int y, t_count *counts)
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

char **read_map(int fd, t_count *counts, int *width, int *height)
{
    char **map;
    char c;
    int line_len;
    int y;

    map = malloc(MAX_HEIGHT * sizeof(char *));
    if (!map)
        exit_with_error33("Error: Memory allocation failed", fd);

    line_len = 0;
    y = 0;
    while (read(fd, &c, 1) == 1)
    {
        if (c == '\n')
        {
            map[y][line_len] = '\0';
            if (y == 0)
                *width = line_len;
            y++;
            line_len = 0;
        }
        else
        {
            if (line_len == 0)
            {
                map[y] = malloc((*width + 1) * sizeof(char));
                if (!map[y])
                {
                    free_map_memory(map, y);
                    exit_with_error33("Error: Memory allocation failed", fd);
                }
            }
            map[y][line_len] = c;
            track_map_info(c, line_len, y, counts);
            line_len++;
        }
    }

    if (line_len != 0)
    {
        map[y][line_len] = '\0';
        y++;
    }

    *height = y;
    return map;
}

void validate_map_path(char *filename)
{
    t_map_params params;
    t_count counts;
    int fd;

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
