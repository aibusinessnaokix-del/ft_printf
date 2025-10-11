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
#include <stdarg.h>

typedef struct s_flag
{
	bool	minus;
	bool	plus;
	bool	space;
	bool	zero;
	bool	sharp;
	bool	quort;
	bool	prezero;
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
char	**allocate_persection(char	*format, char **formlist);
char	**malloc_persection(char *format);
char	**split_percent(char *format);

t_flag	*malloc_flags(char *format);
t_flag	initialize_flags(t_flag flags);
t_flag	*initialize_flaglist(char *format, t_flag *flaglist);
t_flag	*allocate_flags(char **formlist, t_flag *flaglist);
t_flag	read_formlist(char *persecent, t_flag flag);
t_flag	char_flag(char c, t_flag flag);
t_flag	str_flag(char *str, t_flag flag);
t_flag	str_width(char *str, t_flag flag);
t_flag	str_precision(char *str, t_flag flag);
t_flag	str_length(char *str, t_flag flag);
t_flag	str_format(char *str, t_flag flag);
t_flag	*management_flaglist(char *format, char **formlist);

char	*return_c(va_list args, t_flag flaglist);
char	*return_s(va_list args, t_flag flaglist);
char	*return_di(va_list args, t_flag flaglist);
char	*return_u(va_list args, t_flag flaglist);
char	*return_x(va_list args, t_flag flaglist);
char	*return_largex(va_list args, t_flag flaglist);
char	*return_p(va_list args, t_flag flaglist);
char	*return_o(va_list args, t_flag flag);
char	*treat_flag_prezero(char *src, t_flag flag, char *dest, size_t size);
char	*treat_flag_di_nn(char *itoa, t_flag flag, char *dest, size_t size);
char	*treat_flag_di_nz(char *itoa, t_flag flag, char *dest, size_t size);
char	*treat_flag_di_nm(char *itoa, t_flag flag, char *dest, size_t size);
char	*treat_flag_di_hn(char *itoa, t_flag flag, char *dest, size_t size);
char	*treat_flag_di_hz(char *itoa, t_flag flag, char *dest, size_t size);
char	*treat_flag_di_hm(char *itoa, t_flag flag, char *dest, size_t size);
char	*treat_flag_di_n(char *itoa, t_flag flag, char *dest, size_t size);
char	*treat_flag_di_h(char *itoa, t_flag flag, char *dest, size_t size);
char	*treat_flag_di1(char *itoa, t_flag flag, char *dest, size_t size);
size_t	return_maximum(size_t s1, size_t s2, size_t s3);
char	*treat_flag_di(char	*itoa, t_flag flag);
char	*treat_flag_u(char	*itoa, t_flag flag);
char	*treat_flag_u1(char *itoa, t_flag flag, char *dest, size_t size);
char	*treat_flag_u_m(char *itoa, t_flag flag, char *dest, size_t size);
char	*treat_flag_u_z(char *itoa, t_flag flag, char *dest, size_t size);
char	*treat_flag_u_n(char *itoa, t_flag flag, char *dest, size_t size);
char	*treat_flag_x(char	*itoa, t_flag flag);
char	*treat_flag_x1(char *itoa, t_flag flag, char *dest, size_t size);
char	*treat_flag_x_m(char *itoa, t_flag flag, char *dest, size_t size);
char	*treat_flag_x_z(char *itoa, t_flag flag, char *dest, size_t size);
char	*treat_flag_x_n(char *itoa, t_flag flag, char *dest, size_t size);
char	*treat_flag_lx(char	*itoa, t_flag flag);
char	*treat_flag_lx1(char *itoa, t_flag flag, char *dest, size_t size);
char	*treat_flag_lx_m(char *itoa, t_flag flag, char *dest, size_t size);
char	*treat_flag_lx_z(char *itoa, t_flag flag, char *dest, size_t size);
char	*treat_flag_lx_n(char *itoa, t_flag flag, char *dest, size_t size);
char	*treat_flag_p(char	*itoa, t_flag flag);
char	*treat_flag_p1(char *itoa, t_flag flag, char *dest, size_t size);
char	*treat_flag_p_m(char *itoa, t_flag flag, char *dest, size_t size);
char	*treat_flag_p_z(char *itoa, t_flag flag, char *dest, size_t size);
char	*treat_flag_p_n(char *itoa, t_flag flag, char *dest, size_t size);
char	*treat_flag_o(char	*itoa, t_flag flag);
char	*treat_flag_o1(char *itoa, t_flag flag, char *dest, size_t size);
char	*treat_flag_o_m(char *itoa, t_flag flag, char *dest, size_t size);
char	*treat_flag_o_z(char *itoa, t_flag flag, char *dest, size_t size);
char	*treat_flag_o_n(char *itoa, t_flag flag, char *dest, size_t size);

void	free_formlist(char **formlist, size_t count);
void	free_arglist(char **arglist, size_t count);
#endif