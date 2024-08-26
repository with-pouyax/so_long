/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_reader.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 14:15:56 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/26 14:16:13 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_reader.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 13:57:11 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/26 14:07:23 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/so_long.h"

char	*allocate_line(int fd)
{
	char	*line;

	line = malloc(BUFFER_SIZE + 1 * sizeof(char));
	if (!line)
		exit_with_error("Memory allocation failed", fd, NULL);
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

char	*read_line(int fd)
{
	char	*line;
	char	c;
	int		i;
	ssize_t	ret;

	if (fd <= 0)
		exit_with_error("Invalid file descriptor", fd, NULL);
	line = allocate_line(fd);
	i = 0;
	while ((ret = read(fd, &c, 1)) == 1)
	{
		if (i < BUFFER_SIZE)
		{
			line[i++] = c;
			if (c == '\n')
				break ;
		}
		else
		{
			free(line);
			exit_with_error("Line too long for buffer", fd, NULL);
		}
	}
	return (finalize_read_line(line, i, ret, fd));
}
