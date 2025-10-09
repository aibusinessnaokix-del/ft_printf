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
	if (c == '%' || c == 'x' || c == 'X')
		return (true);
	if (c == 'o' || c == 'f')
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
	char	**formlist;
	size_t	pernbr;

	pernbr = count_percent(format);
	if (count_percent(format) == -1)
		return (NULL);
	formlist = (char **)malloc(sizeof(char *) * (pernbr + 1));
	if (!formlist)
		return (NULL);
	return (formlist);
}

char	**allocate_persection(char	*format, char **formlist)
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
			formlist[count] = ft_strndup(format, perlen);
			if (!formlist[count])
			{
				free_persection(formlist, count);
				return (NULL);
			}
			count++, format += perlen;
		}
		else
			format++;
	}
	return (formlist[count] = NULL, formlist);
}

void	free_persection(char **formlist, size_t count)
{
	while (count-- > 0)
		free(formlist[count]);
	free(formlist);
}

char	**split_percent(char *format)
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
	flaglist = initialize_flaglist(format, flaglist);
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

t_flag	*allocate_flags(char **formlist, t_flag *flaglist)
{
	size_t	count;
	
	count = 0;
	while (formlist[count])
	{
		flaglist[count] = read_formlist(formlist[count], flaglist[count]);
		count++;
	}
	return (flaglist);
}

t_flag	read_formlist(char *percent, t_flag flag)
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

t_flag	*management_flaglist(char *format, char **formlist)
{
	t_flag	*flaglist;

	flaglist = malloc_flags(format);
	if (!flaglist)
		return (NULL);
	flaglist = allocate_flags(formlist, flaglist);
	return (flaglist);
}

// this is the end of struct section and start of treat argument

char	*return_c(va_list args)
{
	char	*str;
	
	str = (char *)malloc(sizeof(char) * 2);
	str[0] = va_arg(args, int);
	str[1] = '\0';
	return (str);
}

char	*return_s(va_list args)
{
	char	*str;
	
	str = ft_strdup(va_arg(args, const char *));
	return (str);
}

char	*return_di(va_list args)
{
	int		nbr;
	char	*itoa;
	
	nbr = va_arg(args, int);
	itoa = ft_itoa(nbr);
	return (itoa);
}

char	*return_u(va_list args)
{
	unsigned int	nbr;
	char			*itoa;
	
	nbr = va_arg(args, unsigned int);
	itoa = ft_itoa_unsigned(nbr);
	return (itoa);
}

char	*return_x(va_list args)
{
	unsigned int	nbr;
	char			*itoa;
	
	nbr = va_arg(args, unsigned int);
	itoa = ft_itoa_hexs(nbr);
	return (itoa);
}

char	*return_largex(va_list args)
{
	unsigned int	nbr;
	char			*itoa;
	
	nbr = va_arg(args, unsigned int);
	itoa = ft_itoa_hexb(nbr);
	return (itoa);
}

char	*return_p(va_list args)
{
	void			*ptr;
	uintptr_t		addr;
	char			*itoa;
	
	ptr = va_arg(args, void *);
	addr = (uintptr_t)ptr;
	itoa = ft_itoa_hexs(addr);
	return (itoa);
}

char	**malloc_arglist(size_t count)
{
	char	**arglist;

	arglist = (char **)malloc(sizeof(char *) * (count + 1));
	if (!arglist)
		return (NULL);
	return (arglist);
}

char	*allocate_argument(va_list args, t_flag flaglist)
{
	if (flaglist.format == 'c')
		return (return_c(args));
	if (flaglist.format == 's')
		return (return_s(args));
	if (flaglist.format == 'd' || flaglist.format == 'i')
		return (return_di(args));
	if (flaglist.format == 'u')
		return (return_u(args));
	if (flaglist.format == 'x')
		return (return_x(args));
	if (flaglist.format == 'X')
		return (return_largex(args));
	if (flaglist.format == 'p')
		return (return_p(args));
	//if (flaglist.format == 'o')
	//	return (return_o(args));
	//if (flaglist.format == 'f')
		//return (return_f(args));
	if (flaglist.format == '%')
		return (strdup("%"));
	return (NULL);
}

char	**allocate_arglist(va_list args, char **arglist, t_flag *flaglist)
{
	size_t	count;
	
	count = 0;
	while (flaglist[count].format)
	{
		arglist[count] = allocate_argument(args, flaglist[count]);
		if (!arglist[count])
		{
			//free_arglist(arglist, count);
			return (NULL);
		}
		count++;
	}
	arglist[count] = NULL;
	return (arglist);
}

char	**management_arglist(va_list args, char *format, t_flag *flaglist)
{
	size_t	count;
	char	**arglist;

	count = count_percent(format);
	arglist = malloc_arglist(count);
	if (!arglist)
		return (NULL);
	arglist = allocate_arglist(args, arglist, flaglist);
	return (arglist);
}

// this is the end of argument section and start of treat flags




char	**testf(char *format, ...)
{
	va_list	args;
	char	**formlist;
	t_flag	*flaglist;
	char	**arglist;

	if (count_percent(format) < 0)
		return (NULL);
	formlist = split_percent(format);
	flaglist = management_flaglist(format, formlist);
	va_start(args, format);
	arglist = management_arglist(args, format, flaglist);
	va_end(args);
	return (arglist);
}

//int main(void)
//{
//	char	**arglist;
//	char	*ptr;
	
//	ptr = "ABCDE";
//	arglist = testf("%000ls%####000d%.30c%p%X", "ABCDE", -123, 'f', ptr, 31);
//	printf("%s\n",arglist[0]);
//	printf("%s\n",arglist[1]);
//	printf("%s\n",arglist[2]);
//	printf("%s\n",arglist[3]);
//	printf("%s\n",arglist[4]);
//}
