#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "get_next_line.h"

int	ft_char_counter(char *str, int size)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (i > size)
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
			printf("Line exceeds characters\n");
			exit (EXIT_FAILURE);
		}
		free(line);
	}
	close(fd);
}

void	max_line(char *str, int size)
{
	char *line;
	int	fd;
	int i;

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
	if (i > size)
	{
		printf("Height exceeds characters\n");
		exit (1);
	}
}

int	main(int ac, char **av)
{
	int size = 5;

	if (ac != 2)
	{
		printf("Error\n");
		return (1);
	}
	check_line_len (av[1], size);
	max_line(av[1], size);
}