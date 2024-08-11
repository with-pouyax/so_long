#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "get_next_line.h"

typedef struct s_counts
{
	int p_count;
	int c_count;
	int e_count;
	int p_x;
	int p_y;
}	t_counts;

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



int	main(int ac, char **av)
{
	int size = 22;

	if (ac != 2)
	{
		printf("Error\n");
		return (1);
	}
	check_line_len (av[1], size);
	max_line(av[1], size);
	check_rectangle(av[1]);
	check_surrounded_by_1(av[1]);
	check_characters(av[1]);
	path_validator(av[1]);
}