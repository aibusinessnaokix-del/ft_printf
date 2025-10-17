#include "../ft_printf.h"

static char	*digit_allocate_hexb(long long unsigned int n, char *itoa, size_t index, char *buffa);
static char	*ft_strcpy(char *dest, const char *src);
static char	*make_buffa(char *buffa);

char	*ft_itoa_hexb(long long unsigned int n)
{
	size_t						n_digit;
	long long unsigned int		temp;
	char						*itoa;
	char				buffa[16];

	temp = n, n_digit = 1;
	if (temp < 0)
		temp = -temp, n_digit++;
	while (temp >= 16)
		n_digit++, temp = temp / 16;
	itoa = (char *)malloc(sizeof(char) * (n_digit + 1));
	if (itoa == NULL)
		return (NULL);
	itoa = digit_allocate_hexb(n,  itoa, n_digit, make_buffa(buffa));
	return (itoa);
}

char	*make_buffa(char *buffa)
{
	buffa[0] = '0';
	buffa[1] = '1';
	buffa[2] = '2';
	buffa[3] = '3';
	buffa[4] = '4';
	buffa[5] = '5';
	buffa[6] = '6';
	buffa[7] = '7';
	buffa[8] = '8';
	buffa[9] = '9';
	buffa[10] = 'A';
	buffa[11] = 'B';
	buffa[12] = 'C';
	buffa[13] = 'D';
	buffa[14] = 'E';
	buffa[15] = 'F';
	return (buffa);
}

static char	*digit_allocate_hexb(long long unsigned int n, char *itoa, size_t index, char *buffa)
{
	if (n == 0)
		return(ft_strcpy(itoa, "0"));
	itoa[index] = '\0';
	while (n > 0)
	{
		index--;
		itoa[index] = buffa[n % 16];
		n = n / 16;
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
//	printf("%s\n",ft_itoa_hexb(331));
//}