# Name of the executable
NAME = so_long

# Compiler
CC = cc

# Flags for the compiler
CFLAGS = -Wall -Wextra -Werror -g -Iinclude

# Libraries to link against
LIBS = -lmlx -lXext -lX11

# Source files
SRCS = src/chars_checker.c src/errors.c src/exit.c src/free.c src/game_controls.c \
       src/game_init.c src/get_next_line.c src/get_next_line_utils.c src/image_handling.c \
       src/main.c src/map_drawing.c src/path_validator.c src/rectangle.c \
       src/surrounded_checker.c src/w_h_checker.c src/map_utils.c

# Object files
OBJS = $(SRCS:.c=.o)

# Rule to make the program
all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBS)

# Rule to clean the object files
clean:
	rm -f $(OBJS)

# Rule to clean everything including the executable
fclean: clean
	rm -f $(NAME)

# Rule to recompile everything
re: fclean all

# Rule to check the code against 42 Norm
norm:
	norminette $(SRCS) include/*.h

.PHONY: all clean fclean re norm
