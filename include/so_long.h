/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pghajard <pghajard@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/17 16:26:32 by pghajard          #+#    #+#             */
/*   Updated: 2024/08/29 11:23:08 by pghajard         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H

# include <stdio.h>
# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <mlx.h>
# include "../src/ft_printf/ft_printf.h"

// Constants
# define MAX_WIDTH 200
# define MAX_HEIGHT 400
# define BUFFER_SIZE 1024

// Struct Definitions
typedef struct s_count
{
	int			p_count;
	int			c_count;
	int			e_count;
	int			p_x;
	int			p_y;
	int			width;
	int			height;
	int			exit_found;
}				t_count;

typedef struct s_game
{
	void		*mlx;
	void		*win;
	void		*wall;
	void		*ground;
	void		*collect;
	void		*end;
	void		*player;
	char		**map;
	int			width;
	int			height;
	int			p_x;
	int			p_y;
	int			collect_count;
	int			collected;
	int			moves;
}				t_game;

typedef struct s_map_params
{
	char	**map;
	int		width;
	int		height;
	int		fd;
	int		line_len;
	int		first_line;
	int		current_len;
	int		first_line_read;
}			t_map_params;

// Function Prototypes
void	process_key(int keycode, int *new_x, int *new_y);
int		is_move_valid(int new_x, int new_y, t_game *game);
void	update_position(int new_x, int new_y, t_game *game);
int		handle_keypress(int keycode, t_game *game);
void	process_map_char(t_game *game, char c, int x, int y);
void	read_and_draw_map(t_game *game, int fd, char *buffer);
void	allocate_map_memory(t_game *game, int fd, char *buffer);
void	handle_error(const char *message, int fd);
void	exit_with_error(char *message, int fd, char *line);
void	handle_error_and_exit(int fd, char **map, char *buffer, t_game *game);
void	cleanup(t_game *game);
void	cleanup_1(t_game *game);
void	free_images(t_game *game);
void	free_map(t_game *game);
void	handle_exit(t_game *game);
void	ft_error(const char *msg, int fd, char *buffer);
int		close_window(t_game *game);
void	free_map_memory(char **map, int height);
void	validate_counts(t_count counts);
void	update_counts(t_count *counts, char c, int fd);
void	check_characters(char *str);
void	check_middle_line(char *l, char *line, int fd);
int		is_line_all_ones(char *line);
void	check_first_last_line(char *line, int fd);
void	process_file_lines(int fd);
void	check_surrounded_by_1(char *str);
void	check_line_length(int current_len, int expected_len, int fd);
int		get_line_length(int fd);
void	check_rectangle(char *str);
void	check_map_height(char *str, int size);
int		validate_line_len(int current_len, int size);
void	validate_line(int current_len, int size, int fd);
void	process_lines(int fd, int size);
void	check_line_len(char *str, int size);
void	draw_map(t_game *game, const char *filename);
void	load_image(t_game *game, void **image, char *file_name);
void	load_images(t_game *game);
int		create_window(t_game *game, const char *filename);
void	init_game_struct(t_game *game);
void	update_dimensions(int *width, int *height, int \
line_length, int first_line);
void	process_line(int fd, int *width, int *height);
void	read_file_and_update(const char *filename, int *width, int *height);
void	get_map_dimensions(const char *filename, int *width, int *height);
int		init_game(const char *filename, t_game *game);
int		make_window(const char *filename);
void	validate_file_lines(int fd, int count);
size_t	ft_strlen(const char *s);
void	track_map_info(char c, int x, int y, t_count *counts);
char	*allocate_map_line(int y, int width);
void	finalize_line(t_map_params *params, int *y);
int		process_char(char c, t_map_params *params, t_count *counts, int y);
char	**read_map(int fd, t_count *counts, int *width, int *height);
void	validate_map_path(char *filename);
char	*read_line(int fd, int *flag);
void	exit_with_error33(char *message, int fd);
void	check_middle_line(char *l, char *line, int fd);
void	init_game_struct(t_game *game);

#endif
