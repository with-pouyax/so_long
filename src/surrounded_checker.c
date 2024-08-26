#include "../include/so_long.h"

void	check_middle_line(char *l, char *line, int fd)
{
	int	len;

	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		len--;
	if (line[0] != '1' || line[len - 1] != '1')
	{
		free(l);
		exit_with_error("Error: Not surrounded by 1 (middle lines)", fd, line);
	}
}

int	is_line_all_ones(char *line)
{
	int	i;
	int	len;

	len = ft_strlen(line);
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

#define BUFFER_SIZE 1024

char	*read_line(int fd)
{
	char	*line;
	char	c;
	int		i;
	ssize_t ret;

	if (fd <= 0)
		exit_with_error("Invalid file descriptor", fd, NULL);

	line = malloc(BUFFER_SIZE + 1 * sizeof(char)); // Allocate and initialize memory
	if (!line)
		exit_with_error("Memory allocation failed", fd, NULL);

	i = 0;
	while ((ret = read(fd, &c, 1)) == 1)
	{
		if (i < BUFFER_SIZE)
		{
			line[i++] = c;
			if (c == '\n')
				break;
		}
		else
		{
			free(line);
			exit_with_error("Line too long for buffer", fd, NULL);
		}
	}

	if (ret == -1) // Handle read error
	{
		free(line);
		exit_with_error("Error reading file", fd, NULL);
	}

	// If no bytes were read and no newline was found, return NULL
	if (i == 0)
	{
		free(line);
		return (NULL);
	}

	line[i] = '\0'; // Null-terminate the string
	return (line);
}



void	process_file_lines(int fd)
{
	char	*line;
	char	*last_line;

	last_line = NULL;
	line = read_line(fd);
	if (line == NULL)
		exit_with_error("File is empty or error reading file", fd, NULL);
	check_first_last_line(line, fd);
	free(line);
	line = read_line(fd);
	while (line != NULL)
	{
		if (last_line)
			check_middle_line(line ,last_line, fd);
		free(last_line);
		last_line = line;
		line = read_line(fd);
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
