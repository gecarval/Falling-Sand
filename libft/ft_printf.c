/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 14:08:15 by gecarval          #+#    #+#             */
/*   Updated: 2024/06/12 10:04:16 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_typehandler(char s, va_list *args, int *i, int *len)
{
	if (s == 'c')
		ft_printchar(va_arg(*args, int), len);
	else if (s == 's')
		ft_printstr(va_arg(*args, char *), len);
	else if (s == 'p')
		ft_printpointer(va_arg(*args, void *), len);
	else if (s == 'd' || s == 'i')
		ft_printnb(va_arg(*args, int), len);
	else if (s == 'u')
		ft_printunb(va_arg(*args, unsigned int), len);
	else if (s == 'x' || s == 'X')
		ft_printhex(va_arg(*args, unsigned int), s, len);
	else if (s == '%')
		ft_printchar('%', len);
	else
		(*i)--;
}

int	ft_printf(const char *str, ...)
{
	int		i;
	int		len;
	va_list	args;

	i = 0;
	len = 0;
	va_start(args, str);
	while (str[i])
	{
		if (str[i] == '%')
		{
			i++;
			ft_typehandler((char)str[i], &args, &i, &len);
		}
		else
			ft_printchar((char)str[i], &len);
		i++;
	}
	va_end(args);
	return (len);
}
