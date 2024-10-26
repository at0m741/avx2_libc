/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: babonnet <babonnet@42angouleme.fr>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/17 16:39:57 by babonnet          #+#    #+#             */
/*   Updated: 2024/10/18 19:11:06 by bonsthie         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include <config.h>
#include <stdlib.h>

static int print_variable(va_list var, const char type)
{
	int count;

	count = 0;
	if (type == 'c' || type == 's')
		count = manage_char(var, type);
	else if (type == 'd' || type == 'i')
		ft_putnbr_count(va_arg(var, long long), &count);
	else if (type == 'u')
		ft_putnbr_base(va_arg(var, unsigned int), "0123456789", 10, &count);
	else if (type == 'x')
		ft_putnbr_base(va_arg(var, unsigned int), "0123456789abcdef", 16, &count);
	else if (type == 'X')
		ft_putnbr_base(va_arg(var, unsigned int), "0123456789ABCDEF", 16, &count);
	else if (type == 'p')
		count += print_pointer((unsigned long) va_arg(var, void *));
	else if (type == 'z')
		ft_putsizet(va_arg(var, size_t));
	else if (type == '%')
		count = write(1, "%", 1);
	else
		count = display_wrong_flag(type);
	return (count);
}

int ft_printf(const char *format, ...)
{
	va_list args;
	int     count;

	if (format == NULL)
		return (-1);
	va_start(args, format);
	count = 0;
	while (*format)
	{
		if (*format == '%')
		{
            ++format;
			if (*format)
				count += print_variable(args, *format);
			else
               return (-1);
		}
		else
            count += write(1, format, 1);
        ++format;
	}
	va_end(args);
	return (count);
}

simpl_weak_alias(printf, ft_printf)

