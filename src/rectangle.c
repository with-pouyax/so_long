/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rectangle.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 13:46:19 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/26 13:48:11 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/so_long.h"

void	exit_with_error11(char *message, int fd)
{
	if (fd >= 0)
		close(fd);
	ft_printf("%s\n", message);
	exit(EXIT_FAILURE);
}

int	get_line_length(int fd)
{
	int		len;
	char	c;

	len = 0;
	while (read(fd, &c, 1) == 1 && c != '\n')
		len++;
	return (len);
}

void	check_line_length(int current_len, int expected_len, int fd)
{
	if (current_len != expected_len)
		exit_with_error11("Not a rectangle", fd);
}

void	validate_file_lines(int fd, int expected_len)
{
	char	c;
	int		current_len;
	int		last_char_was_newline;

	current_len = 0;
	last_char_was_newline = 1;
	while (read(fd, &c, 1) == 1)
	{
		if (c == '\n')
		{
			check_line_length(current_len, expected_len, fd);
			current_len = 0;
			last_char_was_newline = 1;
		}
		else
		{
			current_len++;
			last_char_was_newline = 0;
		}
	}
	if (!last_char_was_newline)
		check_line_length(current_len, expected_len, fd);
	else
		exit_with_error11("Error: Extra newline after last line", fd);
}

void	check_rectangle(char *str)
{
	int		fd;
	int		count;

	fd = open(str, O_RDONLY);
	if (fd < 0)
		exit_with_error11("Error opening file", fd);

	count = get_line_length(fd);
	if (count == 0)
		exit_with_error11("File is empty or error reading file", fd);
	if (lseek(fd, 0, SEEK_SET) < 0)
		exit_with_error11("Error resetting file pointer", fd);

	validate_file_lines(fd, count);
	close(fd);
}
