/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_len_checker.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 14:57:06 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/26 14:57:14 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_len_checker.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 13:46:31 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/26 14:51:59 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/so_long.h"

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
			current_len = 0;
		}
		else
		{
			current_len++;
		}
	}
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
