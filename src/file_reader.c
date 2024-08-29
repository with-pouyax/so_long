/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_reader.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 14:15:56 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/29 15:25:22 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/so_long.h"

void	check_middle_line(char *l, char *line, int fd)
{
	int	len;

	len = ft_strlen(line);
	if (len > 0 && line[len - 1] == '\n')
		len--;
	if (line[0] != '1' || line[len - 1] != '1')
	{
		free(l);
		exit_with_error("Error: Not surrounded by 1 (middle lines)", fd, line);
	}
}

char	*allocate_line(int fd)
{
	char	*line;

	if (fd <= 0)
		exit_with_error("Memory allocation failed", fd, NULL);
	line = malloc(BUFFER_SIZE + 1 * sizeof(char));
	if (!line)
		return (NULL);
	return (line);
}

char	*finalize_read_line(char *line, int i, ssize_t ret, int fd)
{
	if (ret == -1)
	{
		free(line);
		exit_with_error("Error reading file", fd, NULL);
	}
	if (i == 0)
	{
		free(line);
		return (NULL);
	}
	line[i] = '\0';
	return (line);
}

ssize_t	read_loop(int fd, char *line, int *i, int *flag)
{
	char	c;
	ssize_t	ret;

	ret = read(fd, &c, 1);
	while (ret == 1)
	{
		if (*i < BUFFER_SIZE)
		{
			line[(*i)++] = c;
			if (c == '\n')
				break ;
		}
		else
			exit_with_error("Line too long for buffer", fd, NULL);
		ret = read(fd, &c, 1);
	}
	if (ret == -1)
	{
		*flag = 1;
		free(line);
		return (-1);
	}
	return (ret);
}

char	*read_line(int fd, int *flag)
{
	char	*line;
	int		i;
	ssize_t	ret;

	if (fd <= 0)
		exit_with_error("Invalid file descriptor", fd, NULL);
	line = allocate_line(fd);
	if (!line)
	{
		*flag = 1;
		return (NULL);
	}
	i = 0;
	ret = read_loop(fd, line, &i, flag);
	if (ret == -1)
		return (NULL);
	return (finalize_read_line(line, i, ret, fd));
}
