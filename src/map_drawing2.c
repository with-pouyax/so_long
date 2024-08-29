/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_drawing2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/29 13:02:16 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/29 14:01:59 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/so_long.h"

void	put_image(t_map_data *data, void *img)
{
	if (mlx_put_image_to_window(data->game->mlx, data->game->win, img, \
	data->x * 100, data->y * 100) == -1)
	{
		mlx_destroy_image(data->game->mlx, img);
		ft_printf("%s", "Failed to put image to window\n");
		handle_error_and_exit(data->fd, NULL, NULL, data->game);
	}
}

void	process_map_char(t_map_data *data)
{
	if (data->c == 'P')
	{
		data->game->p_x = data->x;
		data->game->p_y = data->y;
		put_image(data, data->game->player);
	}
	else if (data->c == 'C')
	{
		data->game->collect_count++;
		put_image(data, data->game->collect);
	}
	else if (data->c == '1')
		put_image(data, data->game->wall);
	else if (data->c == '0')
		put_image(data, data->game->ground);
	else if (data->c == 'E')
		put_image(data, data->game->end);
}

void	process_map_character(t_game *game, t_map_data *data, char *buffer)
{
	if (data->x >= game->width || data->y >= game->height)
		handle_error_and_exit(data->fd, game->map, buffer, game);
	game->map[data->y][data->x] = buffer[0];
	data->c = buffer[0];
	process_map_char(data);
	(data->x)++;
}

void	handle_newline(int *x, int *y)
{
	(*y)++;
	*x = 0;
}
