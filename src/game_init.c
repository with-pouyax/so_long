/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 12:34:09 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/28 11:50:02 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/so_long.h"

void	update_dimensions(int *width, int *height, \
int line_length, int first_line)
{
	if (first_line)
	{
		*width = line_length;
		if (*width > MAX_WIDTH)
		{
			ft_printf("%s", "Line width exceeds maximum allowed size\n");
			exit(1);
		}
	}
	(*height)++;
	if (*height > MAX_HEIGHT)
	{
		ft_printf("%s", "Number of lines exceeds maximum allowed size\n");
		exit(1);
	}
}

void	process_buffer(char *buffer, t_map_params *params)
{
	if (buffer[0] == '\n')
	{
		update_dimensions(&params->width, &params->height, \
		params->line_len, params->first_line);
		params->first_line = 0;
		params->line_len = 0;
	}
	else
	{
		(params->line_len)++;
	}
}

void	process_line(int fd, int *width, int *height)
{
	char			buffer[2];
	ssize_t			bytes_read;
	t_map_params	params;

	params.line_len = 0;
	params.first_line = 1;
	params.width = *width;
	params.height = *height;
	bytes_read = read(fd, buffer, 1);
	while (bytes_read > 0)
	{
		process_buffer(buffer, &params);
		bytes_read = read(fd, buffer, 1);
	}
	if (bytes_read == -1)
	{
		ft_error("Error reading from file", fd, NULL);
	}
	if (params.line_len > 0)
		update_dimensions(&params.width, &params.height, params.line_len, \
		params.first_line);
	*width = params.width;
	*height = params.height;
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

void	get_map_dimensions(const char *filename, int *width, \
int *height)
{
	*width = 0;
	*height = 0;
	read_file_and_update(filename, width, height);
}
