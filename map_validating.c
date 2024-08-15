#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <mlx.h>

#include "get_next_line.h"

typedef struct s_count
{
	int		p_count;
	int		c_count;
	int		e_count;
	int		p_x;
	int		p_y;
	int		width;
	int		height;
	int		exit_found;
}	t_count;

typedef struct s_game
{
	void	*mlx;
	void	*win;
	void	*wall;
	void	*ground;
	void	*collect;
	void	*end;
	void	*player;
	char	**map;
	int		width;
	int		height;
	int		p_x;
	int		p_y;
	int		collect_count;
	int		collected;
	int		moves;
}	t_game;

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

// Function prototypes
void	process_key(int keycode, int *new_x, int *new_y);
int		is_move_valid(int new_x, int new_y, t_game *game);
void	update_position(int new_x, int new_y, t_game *game);
int		handle_keypress(int keycode, t_game *game);

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

int	validate_line_len(char *str, int size)
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
	if (!validate_line_len(line, size))
	{
		free(line);
		close(fd);
		printf("Line length error\n");
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
			handle_error("Unexpected NULL line after successful read", fd);
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

void	check_map_height(char *str, int size)
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
	close(fd);
	if (i > size || i < 3)
	{
		printf("Height exceeds limits\n");
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
	int	len;

	len = strlen(line);
	if (line[len - 1] == '\n')
		len--;
	return (len);
}

void	check_line_length(char *line, int expected_len, int fd)
{
	int	line_len;

	line_len = get_line_length(line);
	if (line_len != expected_len)
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
	int	len;

	len = strlen(line);
	if (line[len - 1] == '\n')
		len--;
	i = 0;
	while (i < len)
	{
		if (line[i] != '1')
			return (0);
		i++;
	}
	return (1);
}

void	check_first_last_line(char *line, int fd)
{
	if (!is_line_all_ones(line))
		exit_with_error("Not surrounded by 1 (first or last line)", fd, line);
}

void	check_middle_line(char *line, int fd)
{
	int	len;

	len = strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		len--;
	if (line[0] != '1' || line[len - 1] != '1')
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
	check_first_last_line(line, fd);
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
		check_first_last_line(last_line, fd);
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

void	update_counts(t_count *counts, char c, int fd, char *line)
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

void	validate_counts(t_count counts)
{
	if (counts.p_count != 1)
		exit_with_error("Error: 'P' count != 1", -1, NULL);
	if (counts.c_count < 1)
		exit_with_error("Error: 'C' count < 1", -1, NULL);
	if (counts.e_count != 1)
		exit_with_error("Error: 'E' count != 1", -1, NULL);
}

void	check_characters(char *str)
{
	t_count	counts;
	char	*line;
	int		fd;
	int		i;

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

void	flood_fill(char **map, int x, int y, int width, int height, \
	t_count *counts, int *exit_found)
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
void	track_map_info(char *line, t_count *counts, int *height)
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

char	**read_map(int fd, t_count *counts, int *width, int *height)
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

void	validate_start_pos(t_count counts, char **map, int height, int fd)
{
	if (counts.p_x == -1 || counts.p_y == -1)
	{
		free_map_memory(map, height);
		exit_with_error("Error: Player start position not found", fd, NULL);
	}
}

void	validate_path(t_count counts, int exit_found, char **map, int height, int fd)
{
	if (counts.c_count != counts.e_count || !exit_found)
	{
		free_map_memory(map, height);
		exit_with_error("Error: No valid path to collect all collectibles and reach the exit", fd, NULL);
	}
}

void	validate_map_path(char *filename)
{
	char		**map;
	int			fd;
	t_count		counts;

	counts = (t_count){0, 0, 0, -1, -1, 0, 0, 0};
	fd = open(filename, O_RDONLY);
	if (fd < 0)
		exit_with_error("Error opening file", fd, NULL);
	map = read_map(fd, &counts, &counts.width, &counts.height);
	validate_start_pos(counts, map, counts.height, fd);
	flood_fill(map, counts.p_x, counts.p_y,
		counts.width, counts.height, &counts, &counts.exit_found);
	validate_path(counts, counts.exit_found, map, counts.height, fd);
	free_map_memory(map, counts.height);
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
		handle_error_and_exit("Err allocate mem for map", fd, NULL, buffer, 0);
	i = 0;
	while (i < game->height)
	{
		game->map[i] = (char *)malloc(sizeof(char) * (game->width + 1));
		if (!game->map[i])
			handle_error_and_exit("Err allocating memory for map rows", \
			fd, game->map, buffer, game->height);
		i++;
	}
}

void	process_map_char(t_game *game, char c, int x, int y)
{
	if (c == 'P')
	{
		game->p_x = x;
		game->p_y = y;
	}
	else if (c == 'C')
	{
		game->collect_count++;
		mlx_put_image_to_window(game->mlx, game->win, \
		game->collect, x * 100, y * 100);
	}
	else if (c == '1')
		mlx_put_image_to_window(game->mlx, game->win, \
		game->wall, x * 100, y * 100);
	else if (c == '0')
		mlx_put_image_to_window(game->mlx, game->win, \
		game->ground, x * 100, y * 100);
	else if (c == 'E')
		mlx_put_image_to_window(game->mlx, game->win, \
		game->end, x * 100, y * 100);
	else if (c == 'P')
		mlx_put_image_to_window(game->mlx, game->win, \
		game->player, x * 100, y * 100);
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
				handle_error_and_exit("Err Map dimensions exceed expected width/height", \
				fd, game->map, buffer, game->height);
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
		handle_error_and_exit("Err allocate mem for buffer", fd, NULL, NULL, 0);
	allocate_map_memory(game, fd, buffer);
	read_and_draw_map(game, fd, buffer);
	free(buffer);
	close(fd);
}

void load_image(t_game *game, void **image, char *file_name)
{
	*image = mlx_xpm_file_to_image(game->mlx, \
	file_name, &game->width, &game->height);
	if (!*image)
	{
		cleanup(game);
		exit(1);
	}
}

void load_images(t_game *game)
{
	load_image(game, (void **)&game->wall, "wall.xpm");
	load_image(game, (void **)&game->ground, "ground.xpm");
	load_image(game, (void **)&game->collect, "collectable.xpm");
	load_image(game, (void **)&game->end, "end.xpm");
	load_image(game, (void **)&game->player, "player.xpm");
}

void	create_window(t_game *game, const char *filename)
{
	game->win = mlx_new_window(game->mlx, game->width * 100, \
	game->height * 100, "Map Window");
	if (game->win == NULL)
	{
		fprintf(stderr, "Error creating window\n");
		exit(1);
	}
	load_images(game);
	draw_map(game, filename);
	mlx_put_image_to_window(game->mlx, game->win, game->player, game->p_x * 100, game->p_y * 100);
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
		fprintf(stderr, "Error initializing MiniLibX\n");
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
	check_line_len(av[1], 20);
	check_map_height(av[1], 9);
	check_rectangle(av[1]);
	check_surrounded_by_1(av[1]);
	check_characters(av[1]);
	validate_map_path(av[1]);
	make_window(av[1]);
	return (0);
}
