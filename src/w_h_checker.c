/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   w_h_checker.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 13:46:31 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/18 19:34:40 by pghajard         ###   ########.fr       */
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
		ft_printf("Error");
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
		ft_printf("Height exceeds limits\n");
		exit(1);
	}
}

int	validate_line_len(int current_len, int size)
{
	if (current_len > size || current_len < 3)
		return (0);
	return (1);
}

void	validate_line(int current_len, int size, int fd)
{
	if (!validate_line_len(current_len, size))
	{
		close(fd);
		ft_printf("Line length error\n");
		exit(EXIT_FAILURE);
	}
}

void	process_lines(int fd, int size)
{
	char	c;
	int		current_len;
	int		first_line_read;

	current_len = 0;
	first_line_read = 0;

	while (read(fd, &c, 1) == 1)
	{
		if (c == '\n')
		{
			if (!first_line_read && current_len == 0)
				handle_error("Unexpected empty first line", fd);

			validate_line(current_len, size, fd);
			first_line_read = 1;
			current_len = 0; // Reset length for the next line
		}
		else
		{
			current_len++;
		}
	}

	// Handle the last line if the file doesn't end with a newline
	if (current_len > 0)
		validate_line(current_len, size, fd);
}

void	check_line_len(char *str, int size)
{
	int		fd;

	fd = open(str, O_RDONLY);
	if (fd < 0)
		handle_error("Error opening file", fd);
	process_lines(fd, size);
	close(fd);
}
