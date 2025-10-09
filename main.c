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

t_flag	*allocate_flags(char **arglist, t_flag *flaglist)
{
	size_t	count;
	
	count = 0;
	while (arglist[count])
	{
		flaglist[count] = read_arglist(arglist[count], flaglist[count]);
		count++;
	}
	return (flaglist);
}

t_flag	read_arglist(char *percent, t_flag flag)
{
	flag = str_flag(percent, flag);
	percent += count_flag(percent);
	flag = str_width(percent, flag);
	percent += count_width(percent);
	flag = str_precision(percent, flag);
	percent += count_precision(percent);
	flag = str_length(percent, flag);
	percent += count_length(percent);
	flag = str_format(percent, flag);
	return (flag);
}

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

t_flag	str_flag(char *str, t_flag flag)
{
	size_t	count;

	count = 0;
	while (detect_flag(str[count]) == true)
	{
		flag = char_flag(str[count], flag);
		count++;
	}
	return (flag);
}

t_flag	str_width(char *str, t_flag flag)
{
	size_t	atoi;
	
	atoi = ft_atoi_alpha(str);
	flag.width = atoi;
	return (flag);
}

t_flag	str_precision(char *str, t_flag flag)
{
	size_t	atoi;

	if (*str != '.')
		return (flag);
	str++;
	atoi = ft_atoi_alpha(str);
	flag.precision = atoi;
	return (flag);
}

static size_t	str_length1(char *str)
{
	if (*str == 'h')
	{
		str++;
		if (*str == 'h')
			return (2);
		return(1);
	}
	if (*str == 'l')
	{
		str++;
		if (*str == 'l')
			return (4);
		return(3);
	}
	if (*str == 'L')
		return(5);
	return (0);
}

t_flag	str_length(char *str, t_flag flag)
{
	flag.length = str_length1(str);
	return (flag);
}

t_flag	str_format(char *str, t_flag flag)
{
	flag.format = *str;
	return (flag);
}

int main(void)
{
	char	*format;
	char	**arglist;
	t_flag	*flaglist;

	format = "ABdefg%-+ 0#.20hhd%%";
	arglist = split_pecent(format);
	flaglist = malloc_flags(format);
	flaglist = allocate_flags(arglist,flaglist);
	printf("%d\n",flaglist[0].minus);
	printf("%d\n",flaglist[0].plus);
	printf("%d\n",flaglist[0].space);
	printf("%d\n",flaglist[0].zero);
	printf("%d\n",flaglist[0].sharp);
	printf("%d\n",flaglist[0].quort);
	printf("%ld\n",flaglist[0].width);
	printf("%ld\n",flaglist[0].precision);	
	printf("%ld\n",flaglist[0].length);	
	printf("%c\n",flaglist[0].format);
	printf("\n");
	printf("%d\n",flaglist[1].minus);
	printf("%d\n",flaglist[1].plus);
	printf("%d\n",flaglist[1].space);
	printf("%d\n",flaglist[1].zero);
	printf("%d\n",flaglist[1].sharp);
	printf("%d\n",flaglist[1].quort);
	printf("%ld\n",flaglist[1].width);
	printf("%ld\n",flaglist[1].precision);	
	printf("%ld\n",flaglist[1].length);	
	printf("%c\n",flaglist[1].format);
}
