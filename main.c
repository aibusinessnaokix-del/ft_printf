#include "libft/libft.h"
#include "printf.h"

bool	detect_flag(char c)
{
	if (c == '#' || c == ' ')
		return (true);
	if (c == '+' || c == '-' || c == '0')
		return (true);
	//if (c == '\'')
	//	return(true);
	return (false);
}

size_t	count_flag(char *str)
{
	size_t	count;

	count = 0;
	while (detect_flag(str[count]))
		count++;
	return (count);
}

bool	detect_width(char c)
{
	if ('0' <= c && c <= '9')
		return (true);
	//if ('*')
	//	return (true);
	return (false);
}

size_t	count_width(char *str)
{
	size_t	count;

	count = 0;
	//if (str[count] == '*')
	//	return (1);
	if ('1' <= str[count] && str[count] <= '9')
		count++;
	while (detect_width(str[count]))
		count++;
	return (count);
}

int	count_precision(char *str)
{
	size_t	count;
	
	count = 0;
	if (str[count] == '.')	
	{		
		count++;
		if (str[count] < '1' || str[count] > '9')
			return (-1);
		while ('0' <= str[count] && str[count] <= '9')
		count++;
	}
	return (count);
}

int	count_length(char *str)
{
	size_t	count;
	
	count = 0;
	if (str[count] == 'h')
	{
		count++;
		if (str[count] == 'h')
			return (2);
		return(1);
	}
	if (str[count] == 'l')
	{
		count++;
		if (str[count] == 'l')
			return (2);
		return(1);
	}
	if (str[count] == 'L')
		return(1);
	return (0);
}

bool	detect_format(char c)
{
	if (c == 'c' || c == 's' || c == 'p')
		return (true);
	if (c == 'd' || c == 'i' || c == 'u')
		return (true);
	if (c == 'o' || c == 'x' || c == 'X')
		return (true);
	if (c == '%' || c == 'f')
		return (true);
	//if (c == 'e' || c == 'g')
	//	return (true);
	return (false); 
}

size_t	percent_len(char *percent)
{
	size_t	index;

	index = 0;
	index += count_flag(percent);
	percent += count_flag(percent);
	index += count_width(percent);
	percent += count_width(percent);
	if (count_precision(percent) == -1)
		return (0);
	index += count_precision(percent);
	percent += count_precision(percent);
	index += count_length(percent);
	percent += count_length(percent);
	if (!detect_format(*percent))
		return (0);
	index++;
	return(index);
}

int	count_percent(char *format)
{
	size_t	count;

	count = 0;
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			count++;
			if (percent_len(format) == 0)
				return (-1);
			format += percent_len(format);
		}
		else
			format++;
	}
	return (count);
}

char	**malloc_persection(char *format)
{
	char	**arglist;
	size_t	pernbr;

	pernbr = count_percent(format);
	if (count_percent(format) == -1)
		return (NULL);
	arglist = (char **)malloc(sizeof(char *) * (pernbr + 1));
	if (!arglist)
		return (NULL);
	return (arglist);
}

char	**allocate_persection(char	*format, char **arglist)
{
	size_t	count;
	size_t	perlen;

	count = 0;
	while (*format)
	{
		if (*format == '%')
		{
			format++;
			perlen = percent_len(format);
			arglist[count] = ft_strndup(format, perlen);
			if (!arglist[count])
			{
				free_persection(arglist, count);
				return (NULL);
			}
			count++, format += perlen;
		}
		else
			format++;
	}
	return (arglist[count] = NULL, arglist);
}

char	**percent_execute(char *format)
{
	char	arglist;
	
	if (count_percent(format) == -1)
		return (NULL);
	arglist = malloc_persection(format);
	if (!arglist)
		return (NULL);
	argllist = allocate_persection(format, arglist);
	return (arglist);
}

int main(void)
{
	char	*format;
	char	**arglist;

	format = "ABCDEFG%d%%ABCDE%0000 00ls";
	arglist = malloc_persection(format);
	arglist = allocate_persection(format, arglist);
	printf("%s\n", arglist[0]);
	printf("%s\n", arglist[1]);
	printf("%s\n", arglist[2]);
	printf("%p\n", arglist[3]);
}