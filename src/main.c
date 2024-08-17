/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 13:45:10 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/17 21:16:04 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

int	ft_strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

int	check_file_extension(char *filename)
{
	int	len;

	len = ft_strlen(filename);
	if (len < 4 || ft_strcmp(filename + len - 4, ".ber") != 0)
	{
		ft_printf("Error: Invalid file extension. Expected .ber\n");
		return (0);
	}
	return (1);
}

int	main(int ac, char **av)
{
	if (ac != 2)
	{
		ft_printf("Error");
		return (1);
	}
	if (!check_file_extension(av[1]))
		return (1);
	check_line_len(av[1], MAX_WIDTH);
	check_map_height(av[1], MAX_HEIGHT);
	check_rectangle(av[1]);
	check_surrounded_by_1(av[1]);
	check_characters(av[1]);
	validate_map_path(av[1]);
	make_window(av[1]);
	return (0);
}
