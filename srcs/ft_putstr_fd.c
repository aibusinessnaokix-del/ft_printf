#include "../ft_printf.h"

int ft_putstr_fd(char *s, int fd)
{
	return (write(fd, s , ft_strlen(s)));
}

//int main(int argc, char **argv)
//{
//	int	size;

//	size = ft_putstr_fd("abc", 1);
//	printf("%d", size);
//	(void)argc;
//	(void)argv;
//}