
#include <stdio.h>
#include "get_next_line.h"


int ft_char_counter(char *str, int size)
{
    int i;
    
    i = 0;
    while (str[i] && str[i] != '\n')
        i++;
    
    // Check if the length of the string exceeds 22 characters
    if (i > size)
        return 0; // Return 0 if the length exceeds the limit of 22 characters
    return 1;     // Return 1 if the length is within the limit
}

int main (int ac, char **av)
{
    int fd;
    
    if (ac != 2)
    {
        printf("Error\n");
        return (1);
    }
    fd = open(av[1], O_RDONLY);
    if ( fd < 0)
    {
        printf("Error\n");
        return (1);
    }
    close(fd);
}