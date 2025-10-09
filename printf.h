#ifndef PRINTF_H
# define PRINTF_H

#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

typedef struct s_flag
{
	bool	minus;
	bool	plus;
	bool	space;
	bool	zero;
	bool	sharp;
	bool	quort;
	size_t	width;
	size_t	precision;
	size_t	length;
	char	format;
} t_flag;

//length h = 1, hh = 2, l = 3, ll = 4 L = 5

bool	detect_flag(char c);
size_t	count_flag(char *str);
bool	detect_width(char c);
size_t	count_width(char *str);
int		count_length(char *str);
int		count_precision(char *str);
bool	detect_format(char c);
size_t	percent_len(char *percent);
int		count_percent(char *format);
void	free_persection(char **arglist, size_t count);
char	**allocate_persection(char	*format, char **arglist);
char	**malloc_persection(char *format);

t_flag	*malloc_flags(char *format);
t_flag	initialize_flags(t_flag flags);
t_flag	*initialize_flaglist(char *format, t_flag *flaglist);
t_flag	*allocate_flags(char **arglist, t_flag *flaglist);
t_flag	read_arglist(char *persecent, t_flag flag);
t_flag	char_flag(char c, t_flag flag);
t_flag	str_flag(char *str, t_flag flag);
t_flag	str_width(char *str, t_flag flag);
t_flag	str_precision(char *str, t_flag flag);
t_flag	str_length(char *str, t_flag flag);
t_flag	str_format(char *str, t_flag flag);

#endif