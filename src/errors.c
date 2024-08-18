/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 13:44:18 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/18 19:35:01 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	handle_error(const char *message, int fd)
{
	ft_printf("%s", message);
	if (fd >= 0)
		close(fd);
	exit(EXIT_FAILURE);
}

void	ft_error(const char *msg, int fd, char *buffer)
{
	if (fd >= 0)
		close(fd);
	if (buffer)
		free(buffer);
	ft_printf("%s", msg);
	exit(1);
}
