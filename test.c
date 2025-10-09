#include "libft/libft.h"
#include "printf.h"

char *return_str(va_list args)
{
	char	*str;
	
	str = va_arg(args, char *);
	return (str);
}

void ft_test(const char *format, ...)
{
	va_list	args;
	char	*str;

	printf("%s\n", format);
	va_start(args, format);
	str = return_str(args);
	printf("%s\n", str);
	va_end(args);
}



int main(void)
{
	ft_test("ABCDE", "FGHIJ");
}