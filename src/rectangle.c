/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rectangle.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 13:46:19 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/17 17:38:12 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	get_line_length(char *line)
{
	int	len;

	len = ft_strlen(line);
	if (line[len - 1] == '\n')
		len--;
	return (len);
}

void	check_line_length(char *line, int expected_len, int fd)
{
	int	line_len;

	line_len = get_line_length(line);
	if (line_len != expected_len)
		exit_with_error("Not a rectangle", fd, line);
}

void	validate_file_lines(int fd, int count)
{
	char	*line;
	char	*last_line;

	last_line = NULL;
	line = get_next_line(fd);
	while (line != NULL)
	{
		check_line_length(line, count, fd);
		free(last_line);
		last_line = line;
		line = get_next_line(fd);
	}
	if (last_line && last_line[ft_strlen(last_line) - 1] == '\n')
		exit_with_error("Error: Extra newline after last line", fd, last_line);
	free(last_line);
}

void	check_rectangle(char *str)
{
	int		fd;
	int		count;
	char	*line;

	fd = open(str, O_RDONLY);
	if (fd < 0)
		exit_with_error("Error opening file", fd, NULL);
	line = get_next_line(fd);
	if (line == NULL)
		exit_with_error("File is empty or error reading file", fd, NULL);
	count = get_line_length(line);
	free(line);
	validate_file_lines(fd, count);
	close(fd);
}
