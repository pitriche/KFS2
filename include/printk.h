#pragma once

# include "types.h"

typedef __builtin_va_list va_list;

# define va_start(ap, last)	__builtin_va_start(ap, last)
# define va_arg(ap, type)	__builtin_va_arg(ap, type)
# define va_end(ap)			__builtin_va_end(ap)

// printk signature
void	printk(const char* fmt, ...);
