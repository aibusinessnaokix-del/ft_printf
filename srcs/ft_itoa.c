#include "../ft_printf.h"

static char	*digit_allocate(long long int n, char *itoa, size_t index);
static char	*ft_strcpy(char *dest, const char *src);

char	*ft_itoa(long long int n)
{
	size_t				n_digit;
	long long int		temp;
	char				*itoa;

	temp = n;
	n_digit = 1;
	if (temp < 0)
	{
		temp = -temp;
		n_digit++;
	}
	while (temp >= 10)
	{
		n_digit++;
		temp = temp / 10;
	}
	itoa = (char *)malloc(sizeof(char) * (n_digit + 1));
	if (itoa == NULL)
		return (NULL);
	itoa = digit_allocate(n,  itoa, n_digit);
	return (itoa);
}

static char	*digit_allocate(long long int n, char *itoa, size_t index)
{
	if (n == LONG_MIN)
		return(ft_strcpy(itoa, "-9223372036854775808"));
	if (n == 0)
		return(ft_strcpy(itoa, "0"));
	if (n < 0)
	{
		itoa[0] = '-';
		n = -n;
	}
	itoa[index] = '\0';
	while (n > 0)
	{
		index--;
		itoa[index] = n % 10 + '0';
		n = n / 10;
	}
	return (itoa);
}

static char	*ft_strcpy(char *dest, const char *src)
{
	size_t	index;

	index = 0;
	while (src[index])
	{
		dest[index] = src[index];
		index++;
	}
	dest[index] = '\0';
	return (dest);
}


//int main(void)
//{
//	printf("%s\n", ft_itoa(10));
//	printf("%s\n", ft_itoa(1));
//	printf("%s\n", ft_itoa(0));
//	printf("%s\n", ft_itoa(-10));
//	printf("%s\n", ft_itoa(INT_MAX));
//	printf("%s\n", ft_itoa(INT_MIN));
//	printf("%ld\n", LONG_MIN);
//}