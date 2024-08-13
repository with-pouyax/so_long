#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <mlx.h>

#include "get_next_line.h"

typedef struct s_counts
{
	int p_count;
	int c_count;
	int e_count;
	int p_x;
	int p_y;
}	t_counts;

typedef struct s_game {
    void    *mlx;
    void    *win;
    void    *img_wall;
    void    *img_ground;
    void    *img_collectable;
    void    *img_end;
    void    *img_player;
    char    **map;
    int     width;
    int     height;
    int     player_x;
    int     player_y;
    int     collectables;
    int     collected;
    int     moves;
} t_game;

int	ft_char_counter(char *str, int size)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (i > size ||  i < 3)
		return (0);
	return (1);
}

void	check_line_len(char *str, int size)
{
	char	*line;
	int		fd;

	fd = open(str, O_RDONLY);
	if (fd < 0)
	{
		perror("Error");
		exit (EXIT_FAILURE);
	}
	while ((line = get_next_line(fd)) != NULL)
	{
		if (!ft_char_counter(line, size))
		{
			free(line);
			close(fd);
			printf("Line has less than 3 or more than size\n");
			exit (EXIT_FAILURE);
		}
		free(line);
	}
	close(fd);
}

void	max_line(char *str, int size)
{
	char	*line;
	int		fd;
	int		i;

	i = 0;
	fd = open(str, O_RDONLY);
	if (fd < 0)
	{
		perror("Error");
		return ;
	}
	while ((line = get_next_line(fd)) != NULL)
	{
		i++;
	}
	free(line);
	close(fd);
	if (i > size || i < 3)
	{
		printf("Height exceeds characters\n");
		exit (1);
	}
}

//____________________________RECTANGLE__________________________

void	exit_with_error(char *message, int fd, char *line)
{
	if (line)
		free(line);
	if (fd >= 0)
		close(fd);
	printf("%s\n", message);
	exit(EXIT_FAILURE);
}

int		get_line_length(char *line)
{
	int	line_length;

	line_length = strlen(line);
	if (line[line_length - 1] == '\n')
		line_length--;
	return (line_length);
}

void	check_line_length(char *line, int expected_length, int fd)
{
	int	line_length;

	line_length = get_line_length(line);
	if (line_length != expected_length)
		exit_with_error("Not a rectangle", fd, line);
}

void	check_rectangle(char *str)
{
	char	*line;
	int		fd;
	int		count;
	char	*last_line = NULL;

	fd = open(str, O_RDONLY);
	if (fd < 0)
		exit_with_error("Error opening file", fd, NULL);
	line = get_next_line(fd);
	if (line == NULL)
		exit_with_error("File is empty or error reading file", fd, NULL);
	count = get_line_length(line);
	free(line);
	while ((line = get_next_line(fd)) != NULL)
	{
		check_line_length(line, count, fd);
		free(last_line);
		last_line = line;
	}
	if (last_line && last_line[strlen(last_line) - 1] == '\n')
		exit_with_error("Error: Extra newline after last line", fd, last_line);
	free(last_line);
	close(fd);
}

//____________________________^^^^^^^^^^__________________________
//____________________________surrounded_by_1_____________________

int		is_line_all_ones(char *line)
{
	int	i;
	int	line_length;

	line_length = strlen(line);
	if (line[line_length - 1] == '\n')
		line_length--;
	i = 0;
	while (i < line_length)
	{
		if (line[i] != '1')
			return (0);
		i++;
	}
	return (1);
}

void	check_first_and_last_line(char *line, int fd)
{
	if (!is_line_all_ones(line))
		exit_with_error("Error: Not surrounded by 1 (first or last line)", fd, line);
}

void	check_middle_line(char *line, int fd)
{
	int	line_length;

	line_length = strlen(line);
	if (line[line_length - 1] == '\n')
		line_length--;
	if (line[0] != '1' || line[line_length - 1] != '1')
		exit_with_error("Error: Not surrounded by 1 (middle lines)", fd, line);
}

void	check_surrounded_by_1(char *str)
{
	char	*line;
	int		fd;
	char	*last_line;

	last_line = NULL;
	fd = open(str, O_RDONLY);
	if (fd < 0)
		exit_with_error("Error opening file", fd, NULL);
	line = get_next_line(fd);
	if (line == NULL)
		exit_with_error("File is empty or error reading file", fd, NULL);
	check_first_and_last_line(line, fd);
	free(line);
	while ((line = get_next_line(fd)) != NULL)
	{
		if (last_line)
			check_middle_line(last_line, fd);
		free(last_line);
		last_line = line;
	}
	if (last_line)
		check_first_and_last_line(last_line, fd);
	free(last_line);
	close(fd);
}
//____________________________^^^^^^^^^^__________________________

//____________________________check_characters__________________________

void	update_counts(t_counts *counts, char c, int fd, char *line)
{
	if (c == 'P')
		counts->p_count++;
	else if (c == 'C')
		counts->c_count++;
	else if (c == 'E')
		counts->e_count++;
	else if (c != '0' && c != '1' && c != '\n')
		exit_with_error("Error: Invalid character found", fd, line);
}

void	validate_counts(t_counts counts)
{
	if (counts.p_count != 1)
		exit_with_error("Error: Number of 'P' is not equal to 1", -1, NULL);
	if (counts.c_count < 1)
		exit_with_error("Error: Number of 'C' is less than 1", -1, NULL);
	if (counts.e_count != 1)
		exit_with_error("Error: Number of 'E' is not equal to 1", -1, NULL);
}

void	check_characters(char *str)
{
	t_counts	counts = {0, 0, 0};
	char		*line;
	int			fd;
	int			i;

	fd = open(str, O_RDONLY);
	if (fd < 0)
		exit_with_error("Error opening file", fd, NULL);

	while ((line = get_next_line(fd)) != NULL)
	{
		i = 0;
		while (line[i])
			update_counts(&counts, line[i++], fd, line);
		free(line);
	}
	close(fd);
	validate_counts(counts);
}

//____________________________^^^^^^^^^^__________________________


//____________________________path_validator__________________________


void free_map(char **map, int height)
{
	for (int i = 0; i < height; i++)
		free(map[i]);
	free(map);
}

void flood_fill(char **map, int x, int y, int width, int height, t_counts *counts, int *exit_found)
{
	if (x < 0 || y < 0 || x >= width || y >= height || map[y][x] == '1' || map[y][x] == 'F')
		return;

	if (map[y][x] == 'E')
	{
		if (counts->c_count == counts->e_count) // All collectibles collected
		{
			*exit_found = 1;
		}
		return;
	}

	if (map[y][x] == 'C')
		counts->c_count++;

	map[y][x] = 'F'; // Mark the cell as visited by setting it to 'F' (Flooded)

	// Recursively visit adjacent cells
	flood_fill(map, x + 1, y, width, height, counts, exit_found);
	flood_fill(map, x - 1, y, width, height, counts, exit_found);
	flood_fill(map, x, y + 1, width, height, counts, exit_found);
	flood_fill(map, x, y - 1, width, height, counts, exit_found);
}

char	**read_map(int fd, t_counts *counts, int *width, int *height)
{
	char	**map = NULL;
	char	*line;

	while ((line = get_next_line(fd)) != NULL)
	{
		if (*height == 0)
			*width = strlen(line);
		map = realloc(map, (*height + 1) * sizeof(char *));
		if (!map)
			exit_with_error("Error: Memory allocation failed", fd, line);
		map[*height] = line;
		for (int i = 0; line[i]; i++)
		{
			if (line[i] == 'P')
			{
				counts->p_x = i;
				counts->p_y = *height;
			}
			else if (line[i] == 'C')
				counts->e_count++;
		}
		(*height)++;
	}
	return map;
}

void	validate_start_position(t_counts counts, char **map, int height, int fd)
{
	if (counts.p_x == -1 || counts.p_y == -1)
	{
		free_map(map, height);
		exit_with_error("Error: Player start position not found", fd, NULL);
	}
}

void	validate_path(t_counts counts, int exit_found, char **map, int height, int fd)
{
	if (counts.c_count != counts.e_count || !exit_found)
	{
		free_map(map, height);
		exit_with_error("Error: No valid path to collect all collectibles and reach the exit", fd, NULL);
	}
}

void	path_validator(char *filename)
{
	char	**map;
	int		fd = open(filename, O_RDONLY);
	int		width = 0, height = 0;
	int		exit_found = 0;
	t_counts counts = {0, 0, 0, -1, -1};

	if (fd < 0)
		exit_with_error("Error opening file", fd, NULL);

	map = read_map(fd, &counts, &width, &height);
	validate_start_position(counts, map, height, fd);

	flood_fill(map, counts.p_x, counts.p_y, width, height, &counts, &exit_found);
	validate_path(counts, exit_found, map, height, fd);

	free_map(map, height);
	close(fd);
}

void get_map_dimensions(const char *filename, int *width, int *height)
{
	int fd = open(filename, O_RDONLY);
	if (fd < 0)
	{
		perror("Error reading file");
		exit(1);
	}

	char buffer[2];
	*width = 0;
	*height = 0;
	int line_length = 0;
	int first_line = 1;

	while (read(fd, buffer, 1) > 0)
	{
		if (buffer[0] == '\n')
		{
			if (first_line)
			{
				*width = line_length;
				first_line = 0;
			}
			(*height)++;
			line_length = 0;
		}
		else
		{
			line_length++;
		}
	}
	if (line_length > 0)
	{
		(*height)++;
		if (first_line)
			*width = line_length;
	}

	close(fd);
}

void draw_map(t_game *game, const char *filename)
{
    int fd;
    char buffer[2];
    int x = 0;
    int y = 0;

    fd = open(filename, O_RDONLY);
    if (fd < 0)
    {
        perror("Error reading file");
        exit(1);
    }

    game->map = (char **)malloc(sizeof(char *) * game->height);
    for (int i = 0; i < game->height; i++)
        game->map[i] = (char *)malloc(sizeof(char) * (game->width + 1));

    while (read(fd, buffer, 1) > 0)
    {
        if (buffer[0] == '\n')
        {
            y++;
            x = 0;
        }
        else
        {
            game->map[y][x] = buffer[0];
            if (buffer[0] == 'P')
            {
                game->player_x = x;
                game->player_y = y;
            }
            else if (buffer[0] == 'C')
            {
                game->collectables++;
            }

            if (buffer[0] == '1')
                mlx_put_image_to_window(game->mlx, game->win, game->img_wall, x * 100, y * 100);
            else if (buffer[0] == '0')
                mlx_put_image_to_window(game->mlx, game->win, game->img_ground, x * 100, y * 100);
            else if (buffer[0] == 'C')
                mlx_put_image_to_window(game->mlx, game->win, game->img_collectable, x * 100, y * 100);
            else if (buffer[0] == 'E')
                mlx_put_image_to_window(game->mlx, game->win, game->img_end, x * 100, y * 100);
            else if (buffer[0] == 'P')
                mlx_put_image_to_window(game->mlx, game->win, game->img_player, x * 100, y * 100);

            x++;
        }
    }
    close(fd);
}

int handle_keypress(int keycode, t_game *game)
{
    int new_x = game->player_x;
    int new_y = game->player_y;

    if (keycode == 65307) // Escape key to close the window
        exit(0);
    else if (keycode == 119) // W key for up
        new_y--;
    else if (keycode == 115) // S key for down
        new_y++;
    else if (keycode == 97) // A key for left
        new_x--;
    else if (keycode == 100) // D key for right
        new_x++;

    // Check if the new position is a wall
    if (game->map[new_y][new_x] == '1')
        return (0);

    // Check if the new position is the end
    if (game->map[new_y][new_x] == 'E')
    {
        if (game->collected == game->collectables)
        {
            printf("Congratulations, you've won!\n");
            exit(0);
        }
        else
        {
            return (0); // Do not allow entering the end block
        }
    }

    // Check if the new position is a collectible
    if (game->map[new_y][new_x] == 'C')
    {
        game->collected++;
        game->map[new_y][new_x] = '0'; // Change collectible to ground
    }

    // Check if the player has actually moved
    if (new_x != game->player_x || new_y != game->player_y)
    {
        // Update player's position on the map
        mlx_put_image_to_window(game->mlx, game->win, game->img_ground, game->player_x * 100, game->player_y * 100);
        game->player_x = new_x;
        game->player_y = new_y;
        mlx_put_image_to_window(game->mlx, game->win, game->img_player, game->player_x * 100, game->player_y * 100);

        // Increment the movement counter and print it
        game->moves++;
        printf("Moves: %d\n", game->moves);
    }

    return (0);
}


int close_window(t_game *game)
{
    mlx_destroy_window(game->mlx, game->win);
    exit(0);
    return (0);
}

void make_window(const char *filename)
{
    t_game game;

    game.collectables = 0;
    game.collected = 0;
    game.moves = 0;

    get_map_dimensions(filename, &game.width, &game.height);
    game.mlx = mlx_init();
    if (game.mlx == NULL)
    {
        fprintf(stderr, "Error initializing MiniLibX\n");
        exit(1);
    }
    game.win = mlx_new_window(game.mlx, game.width * 100, game.height * 100, "Map Window");
    if (game.win == NULL)
    {
        fprintf(stderr, "Error creating window\n");
        exit(1);
    }

    game.img_wall = mlx_xpm_file_to_image(game.mlx, "wall.xpm", &game.width, &game.height);
    game.img_ground = mlx_xpm_file_to_image(game.mlx, "ground.xpm", &game.width, &game.height);
    game.img_collectable = mlx_xpm_file_to_image(game.mlx, "collectable.xpm", &game.width, &game.height);
    game.img_end = mlx_xpm_file_to_image(game.mlx, "end.xpm", &game.width, &game.height);
    game.img_player = mlx_xpm_file_to_image(game.mlx, "player.xpm", &game.width, &game.height);

    draw_map(&game, filename);

    mlx_key_hook(game.win, handle_keypress, &game);
    mlx_hook(game.win, 17, 0, close_window, &game);  // Hook to handle window close event
    mlx_loop(game.mlx);
}

int	main(int ac, char **av)
{
	int size = 10;

	if (ac != 2)
	{
		printf("Error\n");
		return (1);
	}
	check_line_len (av[1], 20);
	max_line(av[1], 9);
	check_rectangle(av[1]);
	check_surrounded_by_1(av[1]);
	check_characters(av[1]);
	path_validator(av[1]);

	make_window(av[1]);

}