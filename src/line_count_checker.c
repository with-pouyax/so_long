/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line_count_checker.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 13:46:31 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/26 14:56:41 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/so_long.h"

void	handle_line_count(int fd, int *line_count)
{
	char	c;
	int		is_line_started;

	*line_count = 0;
	is_line_started = 0;
	while (read(fd, &c, 1) == 1)
	{
		if (c == '\n')
		{
			(*line_count)++;
			is_line_started = 0;
		}
		else if (!is_line_started)
			is_line_started = 1;
	}
	if (is_line_started)
		(*line_count)++;
}

void	check_map_height(char *str, int size)
{
	int		fd;
	int		line_count;

	fd = open(str, O_RDONLY);
	if (fd < 0)
		return ((void)ft_printf("Error opening file\n"));
	handle_line_count(fd, &line_count);
	close(fd);
	if (line_count > size || line_count < 3)
	{
		ft_printf("Height exceeds limits\n");
		exit(1);
	}
}
