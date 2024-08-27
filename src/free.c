/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 13:44:35 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/27 20:33:29 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/so_long.h"

void	free_map_memory(char **map, int height)
{
	int	i;

	if (!map)
		return ;
	i = 0;
	while (i < height)
	{
		free(map[i]);
		i++;
	}
	free(map);
}

void	free_map(t_game *game)
{
	int	i;

	if (!game->map)
		return ;
	i = 0;
	while (i < game->height)
	{
		free(game->map[i]);
		i++;
	}
	free(game->map);
	game->map = NULL;
}

void	free_images(t_game *game)
{
	if (game->wall)
	{
		mlx_destroy_image(game->mlx, game->wall);
		game->wall = NULL;
	}
	if (game->ground)
	{
		mlx_destroy_image(game->mlx, game->ground);
		game->ground = NULL;
	}
	if (game->collect)
	{
		mlx_destroy_image(game->mlx, game->collect);
		game->collect = NULL;
	}
	if (game->end)
	{
		mlx_destroy_image(game->mlx, game->end);
		game->end = NULL;
	}
	if (game->player)
	{
		mlx_destroy_image(game->mlx, game->player);
		game->player = NULL;
	}
}


void	cleanup(t_game *game)
{
	free_images(game);
	free_map(game);
	if (game->win)
	{
		mlx_destroy_window(game->mlx, game->win);
		game->win = NULL;
	}
	if (game->mlx)
	{
		mlx_destroy_display(game->mlx);
		free(game->mlx);
		game->mlx = NULL;
	}
}

void	cleanup2(t_game *game, int fd, char *buffer, int i)
{
	while (i > 0)
	{
		i--;
		free(game->map[i]);
		game->map[i] = NULL;
	}
	free(game->map);
	game->map = NULL;
	if (buffer)
		free(buffer);
	if (fd >= 0)
		close(fd);
	cleanup(game);
	ft_printf("error");
	exit(1);
}
