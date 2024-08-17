/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 13:45:00 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/17 21:15:47 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	load_image(t_game *game, void **image, char *file_name)
{
	*image = mlx_xpm_file_to_image(game->mlx, \
	file_name, &game->width, &game->height);
	if (!*image)
	{
		cleanup(game);
		exit(1);
	}
}

void	load_images(t_game *game)
{
	load_image(game, (void **)&game->wall, "assets/wall.xpm");
	load_image(game, (void **)&game->ground, "assets/ground.xpm");
	load_image(game, (void **)&game->collect, "assets/collectable.xpm");
	load_image(game, (void **)&game->end, "assets/end.xpm");
	load_image(game, (void **)&game->player, "assets/player.xpm");
}

void	create_window(t_game *game, const char *filename)
{
	game->win = mlx_new_window(game->mlx, game->width * 100, \
	game->height * 100, "Map Window");
	if (game->win == NULL)
	{
		ft_printf("%s", "Error creating window\n");
		exit(1);
	}
	load_images(game);
	draw_map(game, filename);
	mlx_put_image_to_window(game->mlx, game->win, game->player, \
	game->p_x * 100, game->p_y * 100);
	mlx_key_hook(game->win, handle_keypress, game);
	mlx_hook(game->win, 17, 0, close_window, game);
	mlx_loop(game->mlx);
}

void	init_game(const char *filename, t_game *game)
{
	init_game_struct(game);
	game->collect_count = 0;
	game->collected = 0;
	game->moves = 0;
	get_map_dimensions(filename, &game->width, &game->height);
	game->mlx = mlx_init();
	if (game->mlx == NULL)
	{
		ft_printf("%s", "Error initializing MiniLibX\n");
		exit(1);
	}
}

void	make_window(const char *filename)
{
	t_game	game;

	init_game(filename, &game);
	create_window(&game, filename);
	cleanup(&game);
}
