# Name of the executable that will be created
NAME = so_long

# The compiler we are going to use
CC = cc

# Compiler flags:
# -Wall: Enable all warnings about potential issues in your code
# -Wextra: Enable some extra warning flags not covered by -Wall
# -Werror: Treat all warnings as errors, stopping compilation
# -g: Include debugging information in the executable
CFLAGS = -Wall -Wextra -Werror -g

# Libraries that the program needs to link against.
# These libraries are for handling graphics:
# -lmlx: MiniLibX (a graphics library)
# -lXext: Extension library for X11
# -lX11: X Window System protocol client library
LIBS = -lmlx -lXext -lX11

# Directory where the ft_printf library is located
FT_PRINTF_DIR = src/ft_printf

# The full path to the ft_printf library file
FT_PRINTF_LIB = $(FT_PRINTF_DIR)/libftprintf.a

# The list of source files (.c files) that will be compiled
SRCS = src/chars_checker.c src/errors.c src/exit.c src/free.c \
       src/game_controls.c src/init_game_struct.c src/game_init.c \
       src/image_handling.c src/main.c src/map_drawing.c src/map_drawing2.c\
       src/path_validator.c src/rectangle.c src/surrounded_checker.c \
       src/line_len_checker.c src/line_count_checker.c \
       src/ft_strlen.c src/map_reader.c src/file_reader.c

# The corresponding object files (.o files) will be generated from the source files
# This changes each .c file extension to .o
OBJS = $(SRCS:.c=.o)

# The default rule that is executed when you run 'make'
# It will first build the ft_printf library and then the program itself
all: $(FT_PRINTF_LIB) $(NAME)

# Rule to build the ft_printf library
# This runs 'make' inside the ft_printf directory
$(FT_PRINTF_LIB):
	$(MAKE) -C $(FT_PRINTF_DIR)

# Rule to build the main program
# It takes the object files and the ft_printf library and links them together
$(NAME): $(OBJS) $(FT_PRINTF_LIB)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBS) $(FT_PRINTF_LIB)

# Rule to clean up the object files (.o files) that were created during compilation
# This doesn't delete the executable or the ft_printf library
clean:
	rm -f $(OBJS)
	$(MAKE) -C $(FT_PRINTF_DIR) clean

# Rule to clean up everything, including the executable and the ft_printf library
fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(FT_PRINTF_DIR) fclean

# Rule to clean up everything and then rebuild the entire project from scratch
re: fclean all

# .PHONY is used to declare "phony" targets. These are not actual files but just names
# for commands to run. This prevents make from confusing them with files of the same name.
.PHONY: all clean fclean re
