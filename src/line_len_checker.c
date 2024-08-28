/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_len_checker.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 16:09:07 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/28 12:22:05 by pghajard         ###   ########.fr       */
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

void	process_line_content(char c, t_map_params *params, int size)
{
	if (c == '\n')
	{
		if (!(params->first_line_read) && params->current_len == 0)
			handle_error("Unexpected empty first line", params->fd);
		validate_line(params->current_len, size, params->fd);
		params->first_line_read = 1;
		params->current_len = 0;
	}
	else
		(params->current_len)++;
}

void	process_lines(int fd, int size)
{
	char			c;
	ssize_t			bytes_read;
	t_map_params	params;

	params.fd = fd;
	params.current_len = 0;
	params.first_line_read = 0;
	while (1)
	{
		bytes_read = read(fd, &c, 1);
		if (bytes_read == 1)
			process_line_content(c, &params, size);
		else
			break ;
	}
	if (bytes_read == -1)
		handle_error("Error reading from file", fd);
	else if (params.current_len > 0)
		validate_line(params.current_len, size, fd);
	close(fd);
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
