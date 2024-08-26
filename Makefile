# Name of the executable
NAME = so_long

# Compiler
CC = cc

# Flags for the compiler
CFLAGS = -Wall -Wextra -Werror -g 

# Libraries to link against
LIBS = -lmlx -lXext -lX11

# Directories
FT_PRINTF_DIR = src/ft_printf
FT_PRINTF_LIB = $(FT_PRINTF_DIR)/libftprintf.a

# Source files
SRCS = src/chars_checker.c src/errors.c src/exit.c src/free.c src/game_controls.c \
       src/game_init.c src/image_handling.c \
       src/main.c src/map_drawing.c src/path_validator.c src/rectangle.c \
       src/surrounded_checker.c src/w_h_checker.c src/ft_strlen.c src/map_reader.c src/file_reader.c

# Object files
OBJS = $(SRCS:.c=.o)

# Rule to make the program
all: $(FT_PRINTF_LIB) $(NAME)

# Build ft_printf library
$(FT_PRINTF_LIB):
	$(MAKE) -C $(FT_PRINTF_DIR)

$(NAME): $(OBJS) $(FT_PRINTF_LIB)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBS) $(FT_PRINTF_LIB)

# Rule to clean the object files
clean:
	rm -f $(OBJS)
	$(MAKE) -C $(FT_PRINTF_DIR) clean

# Rule to clean everything including the executable
fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(FT_PRINTF_DIR) fclean

# Rule to recompile everything
re: fclean all

# Rule to check the code against 42 Norm
norm:
	norminette $(SRCS) include/*.h

.PHONY: all clean fclean re norm
