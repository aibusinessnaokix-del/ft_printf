#include "../libft/libft.h"
#include "../ft_printf.h"

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
	if (c == 'o')
		return (true);
	//if (c == 'e' || c == 'g' || c == 'f')
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
				free_formlist(formlist, count);
				return (NULL);
			}
			count++, format += perlen;
		}
		else
			format++;
	}
	return (formlist[count] = NULL, formlist);
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

t_flag	*malloc_flaglist(char *format)
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
	flags.prezero = false;
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
		flaglist[count] = read_format(formlist[count], flaglist[count]);
		count++;
	}
	return (flaglist);
}

t_flag	read_format(char *format, t_flag flag)
{
	flag = str_to_flag(format, flag);
	format += count_flag(format);
	flag = str_to_width(format, flag);
	format += count_width(format);
	flag = str_to_precision(format, flag);
	format += count_precision(format);
	flag = str_to_length(format, flag);
	format += count_length(format);
	flag = str_format(format, flag);
	return (flag);
}

t_flag	char_to_flag(char c, t_flag flag)
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

t_flag	str_to_flag(char *str, t_flag flag)
{
	size_t	count;

	count = 0;
	while (detect_flag(str[count]) == true)
	{
		flag = char_to_flag(str[count], flag);
		count++;
	}
	return (flag);
}

t_flag	str_to_width(char *str, t_flag flag)
{
	size_t	atoi;
	
	atoi = ft_atoi_alpha(str);
	flag.width = atoi;
	return (flag);
}

t_flag	str_to_precision(char *str, t_flag flag)
{
	size_t	atoi;

	if (*str != '.')
		return (flag);
	str++;
	atoi = ft_atoi_alpha(str);
	if (atoi == 0)
		flag.prezero = true;
	flag.precision = atoi;
	return (flag);
}

static size_t	str_to_length1(char *str)
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

t_flag	str_to_length(char *str, t_flag flag)
{
	flag.length = str_to_length1(str);
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

	flaglist = malloc_flaglist(format);
	if (!flaglist)
		return (NULL);
	flaglist = allocate_flags(formlist, flaglist);
	return (flaglist);
}

// this is the end of struct section and start of treat argument

char	*treat_flag_c1(char c, t_flag flag, char *dest, size_t size)
{
	size_t	count;

	count = 0;
	if (flag.minus)
	{
		dest[count++] = c;
		while (count < size)
			dest[count++] = ' ';
	}
	else
	{
		while (count < size - 1)
			dest[count++] = ' ';
		dest[count++] = c;
	}
	dest[count] = '\0';
	return (dest);
}

char	*treat_flag_c(char c, t_flag flag)
{
	size_t	size;
	char	*dest;

	size = flag.width;
	if (size < 1)
		size = 1;
	dest = (char *)malloc(sizeof(char) * (size + 1));
		if (dest == NULL)
			return (NULL);
	dest = treat_flag_c1(c, flag, dest, size);
	return (dest);
}

char	*return_c(va_list args, t_flag flag)
{
	char	c;
	char	*str;
	
	c =  va_arg(args, int);
	str = treat_flag_c(c, flag);
	(void)flag;
	return (str);
}

size_t	minimum(size_t d1, size_t d2)
{
	if (d1 > d2)
		return (d2);
	return (d1);
}

char	*treat_flag_s1(char *src, t_flag flag, char *dest, size_t size)
{
	size_t	dcount;
	size_t	scount;

	dcount = 0;
	scount = 0;
	if (flag.minus)
	{
		while (src[scount] && (scount < flag.precision || flag.precision == 0))
			dest[dcount++] = src[scount++];
		while (dcount < size)
			dest[dcount++] = ' ';
	}
	else
	{
		while ((flag.precision != 0 && dcount < size - minimum(ft_strlen(src), flag.precision))
				|| (flag.precision == 0 && dcount <size - ft_strlen(src)))
			dest[dcount++] = ' ';
		while (dcount < size)
			dest[dcount++] = src[scount++];
	}
	dest[dcount] = '\0';
	return (dest);
}

char	*treat_flag_prezero(char *src, t_flag flag, char *dest, size_t size)
{
	size_t	dcount;

	dcount = 0;
	while (dcount < size)
		dest[dcount++] = ' ';
	dest[dcount] = '\0';
	(void)src;
	(void)flag;
	return (dest);
}

char	*treat_flag_s(char *src, t_flag flag)
{
	size_t	size;
	char	*dest;

	size = ft_strlen(src);
	if (flag.precision && size > flag.precision)
		size = flag.precision;
	if (flag.prezero)
		size = 0;
	if (size < flag.width)
		size = flag.width;
	dest = (char *)malloc(sizeof(char) * (size + 1));
		if (dest == NULL)
			return (NULL);
	if (flag.prezero)
		dest = treat_flag_prezero(src, flag, dest, size);
	else
		dest = treat_flag_s1(src, flag, dest, size);
	return (dest);
}

char	*return_s(va_list args, t_flag flag)
{
	char	*src;
	char	*dest;
	
	src =  va_arg(args, char *);
	if (!src)
	{
		dest = strdup("null");
		return (dest);
	}
	dest = treat_flag_s(src, flag);
	return (dest);
}

char	*return_di(va_list args, t_flag flag)
{
	int		nbr;
	char	*itoa;
	char	*dest;
	
	if (flag.length <= 2)
		nbr = va_arg(args, int);
	if (flag.length == 1)
		nbr = (short int)nbr;
	if (flag.length == 2)
		nbr = (signed char)nbr;
	if (flag.length == 3)
		nbr = va_arg(args, long int);
	if (flag.length == 4)
		nbr = va_arg(args, long long int);
	itoa = ft_itoa(nbr);
	dest = treat_flag_di(itoa, flag);
	return (dest);
}

char	*treat_flag_di(char	*itoa, t_flag flag)
{
	size_t	size;
	char	*dest;

	if (itoa[0] == '-')
		size = 	return_maximum(flag.precision + 1, ft_strlen(itoa), flag.width);
	else if (flag.plus || flag.space)
		size = 	return_maximum(flag.precision + 1, ft_strlen(itoa) + 1, flag.width);
	else
		size = 	return_maximum(flag.precision, ft_strlen(itoa), flag.width);
	dest = (char *)malloc(sizeof(char) * (size + 1));
	if (dest == NULL)
		return (NULL);
	dest = treat_flag_di1(itoa, flag, dest, size);
	return (dest);
}

size_t	return_maximum(size_t s1, size_t s2, size_t s3)
{
	size_t	max;

	max = s1;
	if (max < s2)
		max = s2;
	if (max < s3)
		max = s3;
	return (max);
}

char	*treat_flag_di1(char *itoa, t_flag flag, char *dest, size_t size)
{
	if (itoa[0] == '-' || flag.space || flag.plus)
		dest = treat_flag_di_h(itoa, flag, dest, size);
	else
		dest = treat_flag_di_n(itoa, flag, dest, size);
	free(itoa);
	return (dest);
}

char	*treat_flag_di_h(char *itoa, t_flag flag, char *dest, size_t size)
{
	if (flag.minus)
		dest = treat_flag_di_hm(itoa, flag, dest, size);
	else if (flag.zero)
		dest = treat_flag_di_hz(itoa, flag, dest, size);
	else
		dest = treat_flag_di_hn(itoa, flag, dest, size);
	return (dest);
}

char	*treat_flag_di_n(char *itoa, t_flag flag, char *dest, size_t size)
{
	if (flag.minus)
		dest = treat_flag_di_nm(itoa, flag, dest, size);
	else if (flag.zero)
		dest = treat_flag_di_nz(itoa, flag, dest, size);
	else
		dest = treat_flag_di_nn(itoa, flag, dest, size);
	return (dest);
}

char	*treat_flag_di_hm(char *itoa, t_flag flag, char *dest, size_t size)
{
	size_t	icount;
	size_t	dcount;
	size_t	isize;
	
	icount = 0, dcount = 0, isize = ft_strlen(itoa);
	if (itoa[icount] == '-')
	{
		dest[dcount++] = itoa[icount++];
		isize--;
	}
	else if (flag.plus)
		dest[dcount++] = '+';
	else
		dest[dcount++] = ' ';
	while (dcount - 1  + isize< flag.precision)
		dest[dcount++] = '0';
	while (itoa[icount])
		dest[dcount++] = itoa[icount++];
	while (dcount < size)
		dest[dcount++] = ' ';
	dest[dcount] = '\0';
	return (dest);
}

char	*treat_flag_di_hz(char *itoa, t_flag flag, char *dest, size_t size)
{
	size_t	icount;
	size_t	dcount;
	size_t	isize;
	
	icount = 0, dcount = 0, isize = ft_strlen(itoa);
	if (itoa[icount] == '-')
	{
		dest[dcount++] = itoa[icount++];
		isize--;
	}
	else if (flag.plus)
		dest[dcount++] = '+';
	else
		dest[dcount++] = ' ';
	while (dcount + isize < size)
		dest[dcount++] = '0';
	while (itoa[icount])
		dest[dcount++] = itoa[icount++];
	dest[dcount] = '\0';
	return (dest);
}

char	*treat_flag_di_hn(char *itoa, t_flag flag, char *dest, size_t size)
{
	size_t	icount;
	size_t	dcount;
	size_t	isize;
	
	icount = 0, dcount = 0, isize = ft_strlen(itoa);
	while (dcount + flag.precision + 1 < size && dcount + isize < size)
		dest[dcount++] = ' ';
	if (itoa[icount] == '-')
	{
		dest[dcount++] = itoa[icount++];
		isize--;
	}
	else if (flag.plus)
		dest[dcount++] = '+';
	else
		dest[dcount++] = ' ';
	while (dcount + isize< size)
		dest[dcount++] = '0';
	while (itoa[icount])
		dest[dcount++] = itoa[icount++];
	dest[dcount] = '\0';
	return (dest);
}

char	*treat_flag_di_nm(char *itoa, t_flag flag, char *dest, size_t size)
{
	size_t	icount;
	size_t	dcount;

	dcount = 0, icount = 0;
	while (dcount + ft_strlen(itoa) < flag.precision)
		dest[dcount++] = '0';
	while (itoa[icount])
		dest[dcount++] = itoa[icount++];
	while (dcount < size)
		dest[dcount++] = ' ';
	dest[dcount] = '\0';
	return (dest);
}

char	*treat_flag_di_nz(char *itoa, t_flag flag, char *dest, size_t size)
{
	size_t	icount;
	size_t	dcount;
	
	icount = 0, dcount = 0;
	while (dcount + ft_strlen(itoa) < size)
		dest[dcount++] = '0';
	while (itoa[icount])
		dest[dcount++] = itoa[icount++];
	dest[dcount] = '\0';
	(void)flag;
	return (dest);
}

char	*treat_flag_di_nn(char *itoa, t_flag flag, char *dest, size_t size)
{
	size_t	icount;
	size_t	dcount;
	
	dcount = 0, icount = 0;
	while (dcount + flag.precision < size && dcount + ft_strlen(itoa) < size)
		dest[dcount++] = ' ';
	while (dcount < size - ft_strlen(itoa))
		dest[dcount++] = '0';
	while (itoa[icount])
		dest[dcount++] = itoa[icount++];
	dest[dcount] = '\0';
	return (dest);
}

char	*return_u(va_list args, t_flag flag)
{
	unsigned int	nbr;
	char			*itoa;
	char			*dest;
	
	if (flag.length <= 2)
		nbr = va_arg(args, unsigned int);
	if (flag.length == 1)
		nbr = (unsigned short int)nbr;
	if (flag.length == 2)
		nbr = (unsigned char)nbr;
	if (flag.length == 3)
		nbr = va_arg(args, unsigned long int);
	if (flag.length == 4)
		nbr = va_arg(args, unsigned long long int);
	itoa = ft_itoa_unsigned(nbr);
	dest = treat_flag_u(itoa, flag);
	return (dest);
}

char	*treat_flag_u(char	*itoa, t_flag flag)
{
	size_t	size;
	char	*dest;

	size = 	return_maximum(flag.precision, ft_strlen(itoa), flag.width);
	dest = (char *)malloc(sizeof(char) * (size + 1));
	if (dest == NULL)
		return (NULL);
	dest = treat_flag_u1(itoa, flag, dest, size);
	return (dest);
}

char	*treat_flag_u1(char *itoa, t_flag flag, char *dest, size_t size)
{
	if (flag.minus)
		dest = treat_flag_u_m(itoa, flag, dest, size);
	else if (flag.zero)
		dest = treat_flag_u_z(itoa, flag, dest, size);
	else
		dest = treat_flag_u_n(itoa, flag, dest, size);
	free(itoa);
	return (dest);
}

char	*treat_flag_u_m(char *itoa, t_flag flag, char *dest, size_t size)
{
	size_t	icount;
	size_t	dcount;

	dcount = 0, icount = 0;
	while (dcount + ft_strlen(itoa) < flag.precision)
		dest[dcount++] = '0';
	while (itoa[icount])
		dest[dcount++] = itoa[icount++];
	while (dcount < size)
		dest[dcount++] = ' ';
	dest[dcount] = '\0';
	return (dest);
}

char	*treat_flag_u_z(char *itoa, t_flag flag, char *dest, size_t size)
{
	size_t	icount;
	size_t	dcount;
	
	icount = 0, dcount = 0;
	while (dcount + ft_strlen(itoa) < size)
		dest[dcount++] = '0';
	while (itoa[icount])
		dest[dcount++] = itoa[icount++];
	dest[dcount] = '\0';
	(void)flag;
	return (dest);
}

char	*treat_flag_u_n(char *itoa, t_flag flag, char *dest, size_t size)
{
	size_t	icount;
	size_t	dcount;
	
	dcount = 0, icount = 0;
	while (dcount + flag.precision < size && dcount + ft_strlen(itoa) < size)
		dest[dcount++] = ' ';
	while (dcount + ft_strlen(itoa) < size)
		dest[dcount++] = '0';
	while (itoa[icount])
		dest[dcount++] = itoa[icount++];
	dest[dcount] = '\0';
	return (dest);
}

char	*return_x(va_list args, t_flag flag)
{
	unsigned int	nbr;
	char			*itoa;
	char			*dest;
	
	if (flag.length <= 2)
		nbr = va_arg(args, unsigned int);
	if (flag.length == 1)
		nbr = (unsigned short int)nbr;
	if (flag.length == 2)
		nbr = (unsigned char)nbr;
	if (flag.length == 3)
		nbr = va_arg(args, unsigned long int);
	if (flag.length == 4)
		nbr = va_arg(args, unsigned long long int);
	itoa = ft_itoa_hexs(nbr);
	dest = treat_flag_x(itoa, flag);
	return (dest);
}

char	*treat_flag_x(char	*itoa, t_flag flag)
{
	size_t	size;
	char	*dest;

	if (flag.sharp)
		size = 	return_maximum(flag.precision + 2, ft_strlen(itoa) + 2, flag.width);
	else
		size = 	return_maximum(flag.precision, ft_strlen(itoa), flag.width);
	dest = (char *)malloc(sizeof(char) * (size + 1));
	if (dest == NULL)
		return (NULL);
	dest = treat_flag_x1(itoa, flag, dest, size);
	return (dest);
}

char	*treat_flag_x1(char *itoa, t_flag flag, char *dest, size_t size)
{
	if (flag.minus)
		dest = treat_flag_x_m(itoa, flag, dest, size);
	else if (flag.zero)
		dest = treat_flag_x_z(itoa, flag, dest, size);
	else
		dest = treat_flag_x_n(itoa, flag, dest, size);
	free(itoa);
	return (dest);
}

char	*treat_flag_x_m(char *itoa, t_flag flag, char *dest, size_t size)
{
	size_t	icount;
	size_t	dcount;
	
	icount = 0, dcount = 0;
	if (flag.sharp)
	{
		dest[dcount++] = '0';
		dest[dcount++] = 'x';
		while (dcount - 2 + ft_strlen(itoa) < flag.precision)
			dest[dcount++] = '0';
	}
	else
	while (dcount + ft_strlen(itoa) < flag.precision)
		dest[dcount++] = '0';
	while (itoa[icount])
		dest[dcount++] = itoa[icount++];
	while (dcount < size)
		dest[dcount++] = ' ';
	dest[dcount] = '\0';
	return (dest);
}

char	*treat_flag_x_z(char *itoa, t_flag flag, char *dest, size_t size)
{
	size_t	icount;
	size_t	dcount;
	
	icount = 0, dcount = 0;
	if (flag.sharp)
	{
		dest[dcount++] = '0';
		dest[dcount++] = 'x';
	}
	while (dcount + ft_strlen(itoa) < size)
		dest[dcount++] = '0';
	while (itoa[icount])
		dest[dcount++] = itoa[icount++];
	dest[dcount] = '\0';
	return (dest);
}

char	*treat_flag_x_n(char *itoa, t_flag flag, char *dest, size_t size)
{
	size_t	icount;
	size_t	dcount;
	
	icount = 0, dcount = 0;
	if (!flag.sharp)
		while (dcount + flag.precision < size && dcount + ft_strlen(itoa) < size)
			dest[dcount++] = ' ';
	if (flag.sharp)
	{
		while (dcount + flag.precision + 2 < size && dcount + ft_strlen(itoa) + 2 < size)
			dest[dcount++] = ' ';
		dest[dcount++] = '0';
		dest[dcount++] = 'x';
	}
	while (dcount + ft_strlen(itoa) < size)
		dest[dcount++] = '0';
	while (itoa[icount])
		dest[dcount++] = itoa[icount++];
	dest[dcount] = '\0';
	return (dest);
}

char	*return_largex(va_list args, t_flag flag)
{
	unsigned int	nbr;
	char			*itoa;
	char			*dest;
	
	if (flag.length <= 2)
		nbr = va_arg(args, unsigned int);
	if (flag.length == 1)
		nbr = (unsigned short int)nbr;
	if (flag.length == 2)
		nbr = (unsigned char)nbr;
	if (flag.length == 3)
		nbr = va_arg(args, unsigned long int);
	if (flag.length == 4)
		nbr = va_arg(args, unsigned long long int);
	itoa = ft_itoa_hexb(nbr);
	dest = treat_flag_lx(itoa, flag);
	return (dest);
}

char	*treat_flag_lx(char	*itoa, t_flag flag)
{
	size_t	size;
	char	*dest;

	if (flag.sharp)
		size = 	return_maximum(flag.precision + 2, ft_strlen(itoa) + 2, flag.width);
	else
		size = 	return_maximum(flag.precision, ft_strlen(itoa), flag.width);
	dest = (char *)malloc(sizeof(char) * (size + 1));
	if (dest == NULL)
		return (NULL);
	dest = treat_flag_lx1(itoa, flag, dest, size);
	return (dest);
}

char	*treat_flag_lx1(char *itoa, t_flag flag, char *dest, size_t size)
{
	if (flag.minus)
		dest = treat_flag_lx_m(itoa, flag, dest, size);
	else if (flag.zero)
		dest = treat_flag_lx_z(itoa, flag, dest, size);
	else
		dest = treat_flag_lx_n(itoa, flag, dest, size);
	free(itoa);
	return (dest);
}

char	*treat_flag_lx_m(char *itoa, t_flag flag, char *dest, size_t size)
{
	size_t	icount;
	size_t	dcount;
	
	icount = 0, dcount = 0;
	if (flag.sharp)
	{
		dest[dcount++] = '0';
		dest[dcount++] = 'X';
		while (dcount - 2 + ft_strlen(itoa) < flag.precision)
			dest[dcount++] = '0';
	}
	else
	while (dcount + ft_strlen(itoa) < flag.precision)
		dest[dcount++] = '0';
	while (itoa[icount])
		dest[dcount++] = itoa[icount++];
	while (dcount < size)
		dest[dcount++] = ' ';
	dest[dcount] = '\0';
	return (dest);
}

char	*treat_flag_lx_z(char *itoa, t_flag flag, char *dest, size_t size)
{
	size_t	icount;
	size_t	dcount;
	
	icount = 0, dcount = 0;
	if (flag.sharp)
	{
		dest[dcount++] = '0';
		dest[dcount++] = 'X';
	}
	while (dcount + ft_strlen(itoa) < size)
		dest[dcount++] = '0';
	while (itoa[icount])
		dest[dcount++] = itoa[icount++];
	dest[dcount] = '\0';
	return (dest);
}

char	*treat_flag_lx_n(char *itoa, t_flag flag, char *dest, size_t size)
{
	size_t	icount;
	size_t	dcount;
	
	icount = 0, dcount = 0;
	if (!flag.sharp)
		while (dcount + flag.precision < size && dcount + ft_strlen(itoa) < size)
			dest[dcount++] = ' ';
	if (flag.sharp)
	{
		while (dcount + flag.precision + 2 < size && dcount + ft_strlen(itoa) + 2 < size)
			dest[dcount++] = ' ';
		dest[dcount++] = '0';
		dest[dcount++] = 'X';
	}
	while (dcount + ft_strlen(itoa) < size)
		dest[dcount++] = '0';
	while (itoa[icount])
		dest[dcount++] = itoa[icount++];
	dest[dcount] = '\0';
	return (dest);
}

char	*return_p(va_list args, t_flag flag)
{
	void			*ptr;
	uintptr_t		addr;
	char			*itoa;
	char			*dest;
	
	ptr = va_arg(args, void *);
	if (!ptr)
	{
		dest = strdup("nil");
		return (dest);
	}
	addr = (uintptr_t)ptr;
	itoa = ft_itoa_hexs(addr);
	dest = treat_flag_p(itoa, flag);
	return (dest);
}

char	*treat_flag_p(char	*itoa, t_flag flag)
{
	size_t	size;
	char	*dest;

	size = 	return_maximum(flag.precision + 2, ft_strlen(itoa) + 2, flag.width);
	dest = (char *)malloc(sizeof(char) * (size + 1));
	if (dest == NULL)
		return (NULL);
	dest = treat_flag_p1(itoa, flag, dest, size);
	return (dest);
}

char	*treat_flag_p1(char *itoa, t_flag flag, char *dest, size_t size)
{
	if (flag.minus)
		dest = treat_flag_p_m(itoa, flag, dest, size);
	else if (flag.zero)
		dest = treat_flag_p_z(itoa, flag, dest, size);
	else
		dest = treat_flag_p_n(itoa, flag, dest, size);
	free(itoa);
	return (dest);
}

char	*treat_flag_p_m(char *itoa, t_flag flag, char *dest, size_t size)
{
	size_t	icount;
	size_t	dcount;
	
	icount = 0, dcount = 0;
	dest[dcount++] = '0';
	dest[dcount++] = 'x';
	while (dcount - 2 + ft_strlen(itoa) < flag.precision)
		dest[dcount++] = '0';
	while (dcount + ft_strlen(itoa) < flag.precision)
		dest[dcount++] = '0';
	while (itoa[icount])
		dest[dcount++] = itoa[icount++];
	while (dcount < size)
		dest[dcount++] = ' ';
	dest[dcount] = '\0';
	return (dest);
}

char	*treat_flag_p_z(char *itoa, t_flag flag, char *dest, size_t size)
{
	size_t	icount;
	size_t	dcount;
	
	icount = 0, dcount = 0;
		dest[dcount++] = '0';
		dest[dcount++] = 'x';
	while (dcount + ft_strlen(itoa) < size)
		dest[dcount++] = '0';
	while (itoa[icount])
		dest[dcount++] = itoa[icount++];
	dest[dcount] = '\0';
	(void)flag;
	return (dest);
}

char	*treat_flag_p_n(char *itoa, t_flag flag, char *dest, size_t size)
{
	size_t	icount;
	size_t	dcount;
	
	icount = 0, dcount = 0;
	while (dcount + flag.precision + 2 < size && dcount + ft_strlen(itoa) + 2 < size)
		dest[dcount++] = ' ';
	dest[dcount++] = '0';
	dest[dcount++] = 'x';
	while (dcount + ft_strlen(itoa) < size)
		dest[dcount++] = '0';
	while (itoa[icount])
		dest[dcount++] = itoa[icount++];
	dest[dcount] = '\0';
	return (dest);
}

char	*return_o(va_list args, t_flag flag)
{
	unsigned int	nbr;
	char			*itoa;
	char			*dest;
	
	if (flag.length <= 2)
		nbr = va_arg(args, unsigned int);
	if (flag.length == 1)
		nbr = (unsigned short int)nbr;
	if (flag.length == 2)
		nbr = (unsigned char)nbr;
	if (flag.length == 3)
		nbr = va_arg(args, unsigned long int);
	if (flag.length == 4)
		nbr = va_arg(args, unsigned long long int);
	itoa = ft_itoa_oct(nbr);
	dest = treat_flag_o(itoa, flag);
	return (dest);
}

char	*treat_flag_o(char	*itoa, t_flag flag)
{
	size_t	size;
	char	*dest;

	if (flag.sharp)
		size = 	return_maximum(flag.precision, ft_strlen(itoa) + 1, flag.width);
	else
		size = 	return_maximum(flag.precision, ft_strlen(itoa), flag.width);
	dest = (char *)malloc(sizeof(char) * (size + 1));
	if (dest == NULL)
		return (NULL);
	dest = treat_flag_o1(itoa, flag, dest, size);
	return (dest);
}

char	*treat_flag_o1(char *itoa, t_flag flag, char *dest, size_t size)
{
	if (flag.minus)
		dest = treat_flag_o_m(itoa, flag, dest, size);
	else if (flag.zero)
		dest = treat_flag_o_z(itoa, flag, dest, size);
	else
		dest = treat_flag_o_n(itoa, flag, dest, size);
	free(itoa);
	return (dest);
}

char	*treat_flag_o_m(char *itoa, t_flag flag, char *dest, size_t size)
{
	size_t	icount;
	size_t	dcount;
	
	icount = 0, dcount = 0;
	if (flag.sharp)
	{
		dest[dcount++] = '0';
		while (dcount + ft_strlen(itoa) < flag.precision)
			dest[dcount++] = '0';
	}
	else
		while (dcount + ft_strlen(itoa) < flag.precision)
			dest[dcount++] = '0';
	while (itoa[icount])
		dest[dcount++] = itoa[icount++];
	while (dcount < size)
		dest[dcount++] = ' ';
	dest[dcount] = '\0';
	return (dest);
}

char	*treat_flag_o_z(char *itoa, t_flag flag, char *dest, size_t size)
{
	size_t	icount;
	size_t	dcount;
	
	icount = 0, dcount = 0;
	if (flag.sharp)
		dest[dcount++] = '0';
	while (dcount + ft_strlen(itoa) < size)
		dest[dcount++] = '0';
	while (itoa[icount])
		dest[dcount++] = itoa[icount++];
	dest[dcount] = '\0';
	return (dest);
}

char	*treat_flag_o_n(char *itoa, t_flag flag, char *dest, size_t size)
{
	size_t	icount;
	size_t	dcount;
	
	icount = 0, dcount = 0;
	if (!flag.sharp)
		while (dcount + flag.precision < size && dcount + ft_strlen(itoa) < size)
			dest[dcount++] = ' ';
	if (flag.sharp)
	{
		while (dcount + flag.precision < size && dcount + ft_strlen(itoa) + 1 < size)
			dest[dcount++] = ' ';
		dest[dcount++] = '0';
	}
	while (dcount + ft_strlen(itoa) < size)
		dest[dcount++] = '0';
	while (itoa[icount])
		dest[dcount++] = itoa[icount++];
	dest[dcount] = '\0';
	return (dest);
}

char	**malloc_arglist(size_t count)
{
	char	**arglist;

	arglist = (char **)malloc(sizeof(char *) * (count + 1));
	if (!arglist)
		return (NULL);
	return (arglist);
}

char	*allocate_argument(va_list args, t_flag flag)
{
	if (flag.format == 'c')
		return (return_c(args, flag));
	if (flag.format == 's')
		return (return_s(args, flag));
	if (flag.format == 'd' || flag.format == 'i')
		return (return_di(args, flag));
	if (flag.format == 'u')
		return (return_u(args, flag));
	if (flag.format == 'x')
		return (return_x(args, flag));
	if (flag.format == 'X')
		return (return_largex(args, flag));
	if (flag.format == 'p')
		return (return_p(args, flag));
	if (flag.format == 'o')
		return (return_o(args, flag));
	//if (flag.format == 'f')
		//return (return_f(args, flag));
	if (flag.format == '%') 				// you may have to check no flags set
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
			free_arglist(arglist, count);
			return (NULL);
		}
		count++;
	}
	arglist[count] = NULL;
	return (arglist);
}

void	free_arglist(char **arglist, size_t count)
{
	while (count--)
		free(arglist[count]);
	free(arglist);
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


char	**management_format(char *format, va_list args)
{
	char	**formlist;
	t_flag	*flaglist;
	char	**arglist;

	if (count_percent(format) < 0)
		return (NULL);
	formlist = split_percent(format);
	flaglist = management_flaglist(format, formlist);
	free_formlist(formlist, count_percent(format));
	arglist = management_arglist(args, format, flaglist);
	free(flaglist);
	return (arglist);
}

int	display_and_count(char *format, char **arglist)
{
	size_t	pindex;
	int		charcount;
	int		sum;
	
	pindex = 0;
	sum = 0;
	while (*format)
	{
		charcount = 0;
		if (*format == '%')
			charcount = ft_putstr_fd(arglist[pindex++], 1), format++, format += percent_len(format); 
		else
			charcount = ft_putchar_fd(*format++, 1);
		if (charcount < 0)
			break ;
		sum += charcount;
	}
	free_arglist(arglist, pindex);
	return (sum);
}

int	ft_printf(char *format, ...)
{
	va_list	args;
	char	**arglist;
	size_t	sum;		

	va_start(args, format);
	arglist = management_format(format, args);
	if (arglist == NULL)
		return (0);
	sum = display_and_count(format, arglist);
	return (va_end(args), sum);
}

void	free_formlist(char **formlist, size_t count)
{
	while (count-- > 0)
		free(formlist[count]);
	free(formlist);
}

int main(void)
{
	(ft_printf("%s", "abcdef"));
}