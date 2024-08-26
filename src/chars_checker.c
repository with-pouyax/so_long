/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chars_checker.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 13:43:52 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/26 10:24:37 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/so_long.h"

void	exit_with_error12(char *message, int fd)
{
	if (fd >= 0)
		close(fd);
	ft_printf("%s\n", message);
	exit(EXIT_FAILURE);
}

void	validate_counts(t_count counts)
{
	if (counts.p_count != 1)
		exit_with_error12("Error: 'P' count != 1", -1);
	if (counts.c_count < 1)
		exit_with_error12("Error: 'C' count < 1", -1);
	if (counts.e_count != 1)
		exit_with_error12("Error: 'E' count != 1", -1);
}

void	update_counts(t_count *counts, char c, int fd)
{
	if (c == 'P')
		counts->p_count++;
	else if (c == 'C')
		counts->c_count++;
	else if (c == 'E')
		counts->e_count++;
	else if (c != '0' && c != '1' && c != '\n')
		exit_with_error12("Error: Invalid character found", fd);
}

void	check_characters(char *str)
{
	t_count	counts;
	int		fd;
	char	c;

	counts.p_count = 0;
	counts.c_count = 0;
	counts.e_count = 0;

	fd = open(str, O_RDONLY);
	if (fd < 0)
		exit_with_error12("Error opening file", fd);

	while (read(fd, &c, 1) == 1)
	{
		update_counts(&counts, c, fd);
	}

	close(fd);
	validate_counts(counts);
}

