/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   image_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 13:45:00 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/27 21:10:59 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/so_long.h"

void load_image(t_game *game, void **image, char *file_name)
{
    *image = mlx_xpm_file_to_image(game->mlx, file_name, &game->width, &game->height);
    if (!*image)
    {
        free_images(game);  // Free any images that were successfully loaded before
        cleanup(game);       // Cleanup any other resources
        exit(1);             // Exit or return an error code
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

int create_window(t_game *game, const char *filename)
{
    game->win = mlx_new_window(game->mlx, game->width * 100, game->height * 100, "Map Window");
    if (game->win == NULL)
	{
        return (cleanup(game), 0);
	}
	load_images(game);
	draw_map(game, filename);

    if (mlx_put_image_to_window(game->mlx, game->win, game->player, game->p_x * 100, game->p_y * 100) == 0)
        return (cleanup(game), 0);

    if (mlx_key_hook(game->win, handle_keypress, game) == 0)
        return (cleanup(game), 0);

    if (mlx_hook(game->win, 17, 0, close_window, game) == 0)
        return (cleanup(game), 0);

    mlx_loop(game->mlx);

    return 1;
}


int init_game(const char *filename, t_game *game)
{
	init_game_struct(game);
	game->collect_count = 0;
	game->collected = 0;
	game->moves = 0;
	get_map_dimensions(filename, &game->width, &game->height);
	game->mlx = mlx_init();
	if (game->mlx == NULL)
	{
		ft_printf("Error initializing MiniLibX\n");
		return 0; // Return failure
	}
	return 1; // Return success
}

int make_window(const char *filename)
{
	t_game	game;

	if (!init_game(filename, &game))
		return 0; // If initialization fails, exit early

	if (!create_window(&game, filename))
	{
		cleanup(&game); // Ensure cleanup is called on failure
		return 0; // Exit early on failure
	}

	cleanup(&game); // Cleanup after successful execution
	return 1; // Return success
}
