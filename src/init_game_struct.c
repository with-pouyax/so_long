/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_game_struct.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 11:45:14 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/28 17:06:22 by pghajard         ###   ########.fr       */
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
