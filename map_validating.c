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
    int width;
    int height;
    int exit_found;
} t_counts;

typedef struct	s_game
{
	void	*mlx;
	void	*win;
	void	*img_wall;
	void	*img_ground;
	void	*img_collectable;
	void	*img_end;
	void	*img_player;
	char	**map;
	int		width;
	int		height;
	int		player_x;
	int		player_y;
	int		collectables;
	int		collected;
	int		moves;
}			t_game;

void initialize_game_struct(t_game *game)
{
game->mlx = NULL;
    game->win = NULL;
    game->img_wall = NULL;
    game->img_ground = NULL;
    game->img_collectable = NULL;
    game->img_end = NULL;
    game->img_player = NULL;
    game->map = NULL;
    game->width = 0;
    game->height = 0;
    game->player_x = 0;
    game->player_y = 0;
    game->collectables = 0;
    game->collected = 0;
    game->moves = 0;
}

int	handle_keypress(int keycode, t_game *game);
void	process_key(int keycode, int *new_x, int *new_y);
int	is_move_valid(int new_x, int new_y, t_game *game);
void	update_position(int new_x, int new_y, t_game *game);

void free_game_map(t_game *game) {
    int i;

    if (game->map == NULL)
        return;
    i = 0;
    while (i < game->height) {
        free(game->map[i]);
        i++;
    }
    free(game->map);
    game->map = NULL;
}


void free_images(t_game *game)
{
    if (game->img_wall)
    {
        mlx_destroy_image(game->mlx, game->img_wall);
        game->img_wall = NULL;
    }
    if (game->img_ground)
    {
        mlx_destroy_image(game->mlx, game->img_ground);
        game->img_ground = NULL;
    }
    if (game->img_collectable)
    {
        mlx_destroy_image(game->mlx, game->img_collectable);
        game->img_collectable = NULL;
    }
    if (game->img_end)
    {
        mlx_destroy_image(game->mlx, game->img_end);
        game->img_end = NULL;
    }
    if (game->img_player)
    {
        mlx_destroy_image(game->mlx, game->img_player);
        game->img_player = NULL;
    }
}

void cleanup(t_game *game)
{
    // Free images if they were allocated
    if (game->img_wall)
        mlx_destroy_image(game->mlx, game->img_wall);
    if (game->img_ground)
        mlx_destroy_image(game->mlx, game->img_ground);
    if (game->img_collectable)
        mlx_destroy_image(game->mlx, game->img_collectable);
    if (game->img_end)
        mlx_destroy_image(game->mlx, game->img_end);
    if (game->img_player)
        mlx_destroy_image(game->mlx, game->img_player);

    // Free the map if it was allocated
    if (game->map)
    {
        for (int i = 0; i < game->height; i++)
        {
            free(game->map[i]);
        }
        free(game->map);
    }

    // Destroy the window and display if they were created
    if (game->win)
        mlx_destroy_window(game->mlx, game->win);
    if (game->mlx)
    {
        mlx_destroy_display(game->mlx);
        free(game->mlx);
    }
}


void	handle_exit(t_game *game)
{
    printf("Congratulations, you've won!\n");
    cleanup(game);
    exit(0);
}

int	close_window(t_game *game)
{
    cleanup(game);
    exit(0);
    return (0);
}

int	ft_char_counter(char *str, int size)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (i > size || i < 3)
		return (0);
	return (1);
}

void	handle_error(const char *message, int fd)
{
	perror(message);
	if (fd >= 0)
		close(fd);
	exit(EXIT_FAILURE);
}

void	validate_line(char *line, int size, int fd)
{
	if (!ft_char_counter(line, size))
	{
		free(line);
		close(fd);
		printf("Line has less than 3 or more than size\n");
		exit(EXIT_FAILURE);
	}
}

void	process_lines(int fd, int size)
{
	char	*line;
	int		first_line_read;

	first_line_read = 0;
	line = get_next_line(fd);
	while (line != NULL)
	{
		if (!line && first_line_read)
			handle_error("unexpected NULL line after successful read", fd);
		first_line_read = 1;
		validate_line(line, size, fd);
		free(line);
		line = get_next_line(fd);
	}
}

void	check_line_len(char *str, int size)
{
	int		fd;

	fd = open(str, O_RDONLY);
	if (fd < 0)
		handle_error("Error", fd);
	process_lines(fd, size);
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
	line = get_next_line(fd);
	while (line != NULL)
	{
		i++;
		free(line);
		line = get_next_line(fd);
	}
	close (fd);
	if (i > size || i < 3)
	{
		printf("Height exceeds characters\n");
		exit(1);
	}
}

void	exit_with_error(char *message, int fd, char *line)
{
	if (line)
		free(line);
	if (fd >= 0)
		close(fd);
	printf("%s\n", message);
	exit(EXIT_FAILURE);
}

int	get_line_length(char *line)
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
	char	*last_line;

	last_line = NULL;
	fd = open(str, O_RDONLY);
	if (fd < 0)
		exit_with_error("Error opening file", fd, NULL);
	line = get_next_line(fd);
	if (line == NULL)
		exit_with_error("File is empty or error reading file", fd, NULL);
	count = get_line_length(line);
	free(line);
	line = get_next_line(fd);
	while (line != NULL)
	{
		check_line_length(line, count, fd);
		free(last_line);
		last_line = line;
		line = get_next_line(fd);
	}
	if (last_line && last_line[strlen(last_line) - 1] == '\n')
		exit_with_error("Error: Extra newline after last line", fd, last_line);
	free(last_line);
	close(fd);
}

int	is_line_all_ones(char *line)
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
		exit_with_error("Not surrounded by 1 (first or last line)", fd, line);
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

void	process_file_lines(int fd)
{
	char	*line;
	char	*last_line;

	last_line = NULL;
	line = get_next_line(fd);
	if (line == NULL)
		exit_with_error("File is empty or error reading file", fd, NULL);
	check_first_and_last_line(line, fd);
	free(line);
	line = get_next_line(fd);
	while (line != NULL)
	{
		if (last_line)
			check_middle_line(last_line, fd);
		free(last_line);
		last_line = line;
		line = get_next_line(fd);
	}
	if (last_line)
		check_first_and_last_line(last_line, fd);
	free(last_line);
}

void	check_surrounded_by_1(char *str)
{
	int		fd;

	fd = open(str, O_RDONLY);
	if (fd < 0)
		exit_with_error("Error opening file", fd, NULL);
	process_file_lines(fd);
	close(fd);
}

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
	t_counts	counts;
	char		*line;
	int			fd;
	int			i;

	counts.p_count = 0;
	counts.c_count = 0;
	counts.e_count = 0;
	fd = open(str, O_RDONLY);
	if (fd < 0)
		exit_with_error("Error opening file", fd, NULL);

	line = get_next_line(fd);
	while (line != NULL)
	{
		i = 0;
		while (line[i])
		{
			update_counts(&counts, line[i++], fd, line);
		}
		free(line);
		line = get_next_line(fd);
	}
	close(fd);
	validate_counts(counts);
}

void	free_map(char **map, int height)
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

void	flood_fill(char **map, int x, int y, int width, int height, \
	t_counts *counts, int *exit_found)
{
	if (x < 0 || y < 0 || x >= width || y >= height || \
	map[y][x] == '1' || map[y][x] == 'F')
		return ;
	if (map[y][x] == 'E')
	{
		if (counts->c_count == counts->e_count)
		{
			*exit_found = 1;
		}
		return ;
	}
	if (map[y][x] == 'C')
		counts->c_count++;
	map[y][x] = 'F';
	flood_fill(map, x + 1, y, width, height, counts, exit_found);
	flood_fill(map, x - 1, y, width, height, counts, exit_found);
	flood_fill(map, x, y + 1, width, height, counts, exit_found);
	flood_fill(map, x, y - 1, width, height, counts, exit_found);
}

#define INITIAL_MAP_SIZE 10
void	track_map_info(char *line, t_counts *counts, int *height)
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

char	**expand_map(char **map, int *map_capacity, int height, int fd, char *line)
{
	char	**new_map;
	int		i;

	i = 0;
	*map_capacity *= 2;
	new_map = malloc(*map_capacity * sizeof(char *));
	if (!new_map)
		exit_with_error("Error: Memory allocation failed", fd, line);
	while (i < height)
	{
		new_map[i] = map[i];
		i++;
	}
	free(map);
	return (new_map);
}

char	**read_map(int fd, t_counts *counts, int *width, int *height)
{
	char	**map;
	char	*line;
	int		map_capacity;

	map_capacity = INITIAL_MAP_SIZE;
	map = malloc(map_capacity * sizeof(char *));
	if (!map)
		exit_with_error("Error: Memory allocation failed", fd, NULL);
	line = get_next_line(fd);
	while (line != NULL)
	{
		if (*height == 0)
			*width = strlen(line);
		if (*height >= map_capacity)
			map = expand_map(map, &map_capacity, *height, fd, line);
		map[*height] = line;
		track_map_info(line, counts, height);
		(*height)++;
		line = get_next_line(fd);
	}
	return (map);
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
	char		**map;
	int			fd;
	t_counts	counts;

	counts = (t_counts){0, 0, 0, -1, -1, 0, 0, 0};
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		exit_with_error("Error opening file", fd, NULL);
	map = read_map(fd, &counts, &counts.width, &counts.height);
	validate_start_position(counts, map, counts.height, fd);
	flood_fill(map, counts.p_x, counts.p_y,
		counts.width, counts.height, &counts, &counts.exit_found);
	validate_path(counts, counts.exit_found, map, counts.height, fd);
	free_map(map, counts.height);
	close(fd);
}

#define MAX_WIDTH 1000  // Example maximum width
#define MAX_HEIGHT 1000 // Example maximum height

void	ft_error(const char *msg, int fd, char *buffer)
{
	if (fd >= 0)
		close(fd);
	if (buffer)
		free(buffer);
	perror(msg);
	exit(1);
}

void	update_dimensions(int *width, int *height, int line_length, int first_line)
{
	if (first_line)
	{
		*width = line_length;
		if (*width > MAX_WIDTH)
		{
			fprintf(stderr, "Line width exceeds maximum allowed size\n");
			exit(1);
		}
	}
	(*height)++;
	if (*height > MAX_HEIGHT)
	{
		fprintf(stderr, "Number of lines exceeds maximum allowed size\n");
		exit(1);
	}
}

void	process_line(int fd, int *width, int *height)
{
	char	*buffer;
	int		line_length;
	int		first_line;

	buffer = malloc(2 * sizeof(char));
	if (!buffer)
		ft_error("Error allocating memory", fd, NULL);
	line_length = 0;
	first_line = 1;
	while (read(fd, buffer, 1) > 0)
	{
		if (buffer[0] == '\n')
		{
			update_dimensions(width, height, line_length, first_line);
			first_line = 0;
			line_length = 0;
		}
		else
			line_length++;
	}
	if (line_length > 0)
		update_dimensions(width, height, line_length, first_line);
	free(buffer);
}

void	read_file_and_update(const char *filename, int *width, int *height)
{
	int		fd;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		ft_error("Error reading file", fd, NULL);
	process_line(fd, width, height);
	if (*height == 0)
		ft_error("File is empty or does not contain valid lines", fd, NULL);
	close(fd);
}

void	get_map_dimensions(const char *filename, int *width, int *height)
{
	*width = 0;
	*height = 0;
	read_file_and_update(filename, width, height);
}

void	handle_error_and_exit(const char *message, int fd, char **map, char *buffer, int height)
{
	int	i;

	i = 0;
	if (map)
	{
		while (i < height)
		{
			if (map[i])
				free(map[i]);
			i++;
		}
		free(map);
	}
	if (buffer)
		free(buffer);
	if (fd >= 0)
		close(fd);
	perror(message);
	exit(1);
}

void	allocate_map_memory(t_game *game, int fd, char *buffer)
{
	int	i;

	game->map = (char **)malloc(sizeof(char *) * game->height);
	if (!game->map)
		handle_error_and_exit("Error allocating memory for map", fd, NULL, buffer, 0);
	i = 0;
	while (i < game->height)
	{
		game->map[i] = (char *)malloc(sizeof(char) * (game->width + 1));
		if (!game->map[i])
			handle_error_and_exit("Error allocating memory for map rows", fd, game->map, buffer, game->height);
		i++;
	}
}

void process_map_char(t_game *game, char c, int x, int y)
{
    if (c == 'P')
    {
        game->player_x = x;
        game->player_y = y;
    }
    if (c == '1')
        mlx_put_image_to_window(game->mlx, game->win, game->img_wall, x * 100, y * 100);
    else if (c == '0')
        mlx_put_image_to_window(game->mlx, game->win, game->img_ground, x * 100, y * 100);
    else if (c == 'C')
        mlx_put_image_to_window(game->mlx, game->win, game->img_collectable, x * 100, y * 100);
    else if (c == 'E')
        mlx_put_image_to_window(game->mlx, game->win, game->img_end, x * 100, y * 100);
    else if (c == 'P')
        mlx_put_image_to_window(game->mlx, game->win, game->img_player, x * 100, y * 100);
}


void	read_and_draw_map(t_game *game, int fd, char *buffer)
{
	int	x;
	int	y;

	x = 0;
	y = 0;
	while (read(fd, buffer, 1) > 0)
	{
		if (buffer[0] == '\n')
		{
			y++;
			x = 0;
		}
		else
		{
			if (x >= game->width || y >= game->height)
				handle_error_and_exit("Error: Map dimensions exceed expected width/height", fd, game->map, buffer, game->height);
			game->map[y][x] = buffer[0];
			process_map_char(game, buffer[0], x, y);
			x++;
		}
	}
}

void	draw_map(t_game *game, const char *filename)
{
	int		fd;
	char	*buffer;

	fd = open(filename, O_RDONLY);
	if (fd < 0)
		handle_error_and_exit("Error reading file", -1, NULL, NULL, 0);
	buffer = (char *)malloc(sizeof(char) * 2);
	if (!buffer)
		handle_error_and_exit("Error allocating memory for buffer", fd, NULL, NULL, 0);
	allocate_map_memory(game, fd, buffer);
	read_and_draw_map(game, fd, buffer);
	free(buffer);
	close(fd);
}

void load_images(t_game *game)
{
    game->img_wall = mlx_xpm_file_to_image(game->mlx, "wall.xpm", &game->width, &game->height);
    if (!game->img_wall)
    {
        cleanup(game);
        exit(1);
    }

    game->img_ground = mlx_xpm_file_to_image(game->mlx, "ground.xpm", &game->width, &game->height);
    if (!game->img_ground)
    {
        cleanup(game);
        exit(1);
    }

    game->img_collectable = mlx_xpm_file_to_image(game->mlx, "collectable.xpm", &game->width, &game->height);
    if (!game->img_collectable)
    {
        cleanup(game);
        exit(1);
    }

    game->img_end = mlx_xpm_file_to_image(game->mlx, "end.xpm", &game->width, &game->height);
    if (!game->img_end)
    {
        cleanup(game);
        exit(1);
    }

    game->img_player = mlx_xpm_file_to_image(game->mlx, "player.xpm", &game->width, &game->height);
    if (!game->img_player)
    {
        cleanup(game);
        exit(1);
    }
}



void	create_window(t_game *game, const char *filename)
{
	game->win = mlx_new_window(game->mlx, \
	game->width * 100, game->height * 100, "Map Window");
	if (game->win == NULL)
	{
		fprintf(stderr, "Error creating window\n");
		exit(1);
	}

	load_images(game);
	draw_map(game, filename);

	mlx_key_hook(game->win, handle_keypress, game);
	mlx_hook(game->win, 17, 0, close_window, game);
	mlx_loop(game->mlx);
}

void	initialize_game(const char *filename, t_game *game)
{
    initialize_game_struct(game);  // Initialize the structure

    game->collectables = 0;
    game->collected = 0;
    game->moves = 0;

    get_map_dimensions(filename, &game->width, &game->height);
    game->mlx = mlx_init();
    if (game->mlx == NULL)
    {
        fprintf(stderr, "Error initializing MiniLibX\n");
        exit(1);
    }
}

void	make_window(const char *filename)
{
	t_game	game;

	initialize_game(filename, &game);
	create_window(&game, filename);
	cleanup(&game); // Ensure cleanup is called even after window is closed
}

int	handle_keypress(int keycode, t_game *game)
{
	int	new_x;
	int	new_y;

	new_x = game->player_x;
	new_y = game->player_y;
	if (keycode == 65307) // Escape key
	{
		cleanup(game); // Ensure cleanup is done
		exit(0); // Then exit
	}
	process_key(keycode, &new_x, &new_y);
	if (is_move_valid(new_x, new_y, game))
		update_position(new_x, new_y, game);
	return (0);
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

int	is_move_valid(int new_x, int new_y, t_game *game)
{
	if (new_x < 0 || new_y < 0 || new_x >= game->width || new_y >= game->height)
		return (0);
	if (game->map[new_y][new_x] == '1')
		return (0);
	if (game->map[new_y][new_x] == 'E')
	{
		if (game->collected == game->collectables)
			handle_exit(game);
		else
			return (0);
	}
	return (1);
}

void update_position(int new_x, int new_y, t_game *game)
{
    // Check if the player is trying to move to a different position
    if (new_x != game->player_x || new_y != game->player_y)
    {
        if (game->map[new_y][new_x] == 'C')
        {
            game->collected++;
            game->map[new_y][new_x] = '0';
        }

        // Clear the player's previous position
        mlx_put_image_to_window(game->mlx, game->win, game->img_ground,
            game->player_x * 100, game->player_y * 100);
        
        // Update the player's position
        game->player_x = new_x;
        game->player_y = new_y;

        // Draw the player at the new position
        mlx_put_image_to_window(game->mlx, game->win, game->img_player,
            game->player_x * 100, game->player_y * 100);

        // Increment the move counter
        game->moves++;
        printf("Moves: %d\n", game->moves);
    }
}

int	main(int ac, char **av)
{
	int	size;

	size = 10;
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
	return (0);
}
