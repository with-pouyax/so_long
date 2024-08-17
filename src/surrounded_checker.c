/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   surrounded_checker.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 13:46:26 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/17 21:59:35 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	check_middle_line(char *line, int fd)
{
	int	len;

	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		len--;
	if (line[0] != '1' || line[len - 1] != '1')
		exit_with_error("Error: Not surrounded by 1 (middle lines)", fd, line);
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

void	process_file_lines(int fd)
{
	char	*line;
	char	*last_line;

	last_line = NULL;
	line = get_next_line(fd);
	if (line == NULL)
		exit_with_error("File is empty or error reading file", fd, NULL);
	check_first_last_line(line, fd);
	free(line);
	line = get_next_line(fd);
	while (line != NULL)
	{
		if (last_line)
			check_middle_line(last_line, fd);
		free(last_line);  
		last_line = line;
		line = get_next_line(fd);
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
