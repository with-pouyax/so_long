# Name of the executable
NAME = your_program_name

# Compiler
CC = cc

# Flags for the compiler
CFLAGS = -g -Wall -Wextra -Werror

# Libraries to link against
LIBS = -lmlx -lXext -lX11

# Source files
SRCS = $(wildcard *.c)

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

# Rule to run valgrind without suppression for mlx and output in terminal
valgrind: $(NAME)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=/dev/null ./$(NAME) a.ber

.PHONY: all clean fclean re
