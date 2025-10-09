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

void	free_persection(char **arglist, size_t count)
{
	while (count-- > 0)
		free(arglist[count]);
	free(arglist);
}

char	**split_pecent(char *format)
{
	int		count;
	char	**arglst;

	count = count_percent(format);
	if (count < 0)
		return (NULL);
	arglst = malloc_persection(format);
	if (!arglst)
		return (NULL);
	arglst = allocate_persection(format, arglst);
	if (!arglst)
		return (NULL);
	return (arglst);
}

// this is the end of read section and start of flag management

t_flag	*malloc_flags(char *format)
{
	t_flag	*flaglist;
	size_t	count;

	count = count_percent(format);
	flaglist = (t_flag *)malloc(sizeof(t_flag) * (count + 1));
	if (!flaglist)
		return (NULL);
	return (flaglist);
}

t_flag	initialize_flags(t_flag flags)
{
	flags.minus = false;
	flags.plus = false;
	flags.space = false;
	flags.zero = false;
	flags.sharp = false;
	flags.quort = false;
	flags.width = 0;
	flags.precision = 0;
	flags.length = 0;
	flags.format = '\0';
	return (flags);
}

t_flag	*initialize_flaglist(char *format, t_flag *flaglist)
{
	size_t	count;
	size_t	index;

	count = count_percent(format);
	index = 0;
	while (index <= count)
	{
		flaglist[index] = initialize_flags(flaglist[index]);
		index++;
	}
	return (flaglist);
}

//t_flag	*allocate_flags(char **arglist, t_flag *flaglist)
//{
//	size_t	count;
	
//	count = 0;
//	while (arglist[count])
//	{
//		flaglist[count] = read_arglist(arglist[count]);
//		count++;
//	}
//	flaglist[count].raw = NULL;
//	return (flaglist);
//}

//t_flag	read_arglist(char *persection, t_flag flag)
//{

//}

t_flag	char_flag(char c, t_flag flag)
{	
	if (c == '-')
		flag.minus = true;
	if (c == '+')
		flag.plus = true;
	if (c == ' ')
		flag.space = true;
	if (c == '#')
		flag.sharp = true;
	if (c == '0')
		flag.zero = true;
	return (flag);
}

t_flag	str_flag(char *format, char *str, t_flag flag)
{
	size_t	size;
	size_t	count;

	size = count_flag(format);
	count = 0;
	while (count < size)
	{
		flag = char_flag(str[count], flag);
		count++;
	}
	return (flag);
}

t_flag	str_width(char *str, t_flag flag)
{
	size_t	atoi;
	

}

//int main(void)
//{
//	char	*format;
//	char	**arglist;
//	t_flag	*flaglist;

//	format = "ABCDEFG%d%%ABCDE%0000 00ls";
//	arglist = split_pecent(format);
//	//flaglist = malloc_flags(format);
//	//flaglist = allocate_flags(arglist,flaglist);

//}
