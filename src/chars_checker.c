/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chars_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 13:43:52 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/17 16:41:40 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	validate_counts(t_count counts)
{
	if (counts.p_count != 1)
		exit_with_error("Error: 'P' count != 1", -1, NULL);
	if (counts.c_count < 1)
		exit_with_error("Error: 'C' count < 1", -1, NULL);
	if (counts.e_count != 1)
		exit_with_error("Error: 'E' count != 1", -1, NULL);
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
