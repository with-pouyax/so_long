/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_controls.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 12:33:33 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/26 10:24:59 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/so_long.h"

int	close_window(t_game *game)
{
	cleanup(game);
	exit(0);
	return (0);
}

void	update_position(int new_x, int new_y, t_game *game)
{
	if (new_x != game->p_x || new_y != game->p_y)
	{
		if (game->map[new_y][new_x] == 'C')
		{
			game->collected++;
			game->map[new_y][new_x] = '0';
		}
		mlx_put_image_to_window(game->mlx, game->win, game->ground,
			game->p_x * 100, game->p_y * 100);
		game->p_x = new_x;
		game->p_y = new_y;
		mlx_put_image_to_window(game->mlx, game->win, game->player,
			game->p_x * 100, game->p_y * 100);
		game->moves++;
		ft_printf("Moves: %d\n", game->moves);
	}
}

int	is_move_valid(int new_x, int new_y, t_game *game)
{
	if (new_x < 0 || new_y < 0 || new_x >= game->width || new_y >= game->height)
		return (0);
	if (game->map[new_y][new_x] == '1')
		return (0);
	if (game->map[new_y][new_x] == 'E')
	{
		if (game->collected == game->collect_count)
		{
			handle_exit(game);
			return (1);
		}
		else
			return (0);
	}
	return (1);
}

void	process_key(int keycode, int *new_x, int *new_y)
{
	if (keycode == 119)
		(*new_y)--;
	else if (keycode == 115)
		(*new_y)++;
	else if (keycode == 97)
		(*new_x)--;
	else if (keycode == 100)
		(*new_x)++;
}

int	handle_keypress(int keycode, t_game *game)
{
	int	new_x;
	int	new_y;

	new_x = game->p_x;
	new_y = game->p_y;
	if (keycode == 65307)
	{
		cleanup(game);
		exit(0);
	}
	process_key(keycode, &new_x, &new_y);
	if (is_move_valid(new_x, new_y, game))
		update_position(new_x, new_y, game);
	return (0);
}
