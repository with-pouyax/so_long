/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 13:45:10 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/29 15:19:50 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/so_long.h"

char	*ft_strrchr(const char *s, int c)
{
	size_t	i;
	char	*p;

	i = 0;
	p = NULL;
	while (s[i])
	{
		if (s[i] == ((char)c))
		{
			p = (char *)&s[i];
		}
		i++;
	}
	if (s[i] == ((char)c))
		p = (char *)&s[i];
	return (p);
}

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
	int		len;
	char	*basename;

	basename = ft_strrchr(filename, '/');
	if (basename == NULL)
		basename = filename;
	else
		basename++;
	len = strlen(basename);
	if (len <= 4 || strcmp(basename + len - 4, ".ber") != 0)
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
	if (!make_window(av[1]))
	{
		ft_printf("Failed to create window or load resources.\n");
		return (1);
	}
	return (0);
}
