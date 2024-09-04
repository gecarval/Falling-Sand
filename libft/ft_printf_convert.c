/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_convert.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/21 18:32:55 by gecarval          #+#    #+#             */
/*   Updated: 2024/06/12 10:04:11 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_printhex(unsigned int nb, char c, int *len)
{
	if (nb > 15)
		ft_printhex(nb / 16, c, len);
	if (c == 'x')
		ft_printchar("0123456789abcdef"[nb % 16], len);
	else
		ft_printchar("0123456789ABCDEF"[nb % 16], len);
}

static void	ft_printhexaux(unsigned long long nb, int *len)
{
	if (nb > 15)
		ft_printhexaux(nb / 16, len);
	ft_printchar("0123456789abcdef"[nb % 16], len);
}

void	ft_printpointer(void *pt, int *len)
{
	if (pt == NULL || pt == 0)
	{
		ft_printstr("(nil)", len);
		return ;
	}
	write(1, "0x", 2);
	(*len) += 2;
	ft_printhexaux((unsigned long long)pt, len);
}
