/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   w_h_checker.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 13:46:31 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/17 16:51:45 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	check_map_height(char *str, int size)
{
	char	*line;
	int		fd;
	int		i;

	i = 0;
	fd = open(str, O_RDONLY);
	if (fd < 0)
	{
		printf("Error");
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
