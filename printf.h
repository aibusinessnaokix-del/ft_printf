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

typedef struct s_format
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
} t_format;

#endif