/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_display.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/20 20:23:55 by gecarval          #+#    #+#             */
/*   Updated: 2024/06/12 10:04:06 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_printchar(char c, int *len)
{
	write(1, &c, 1);
	(*len)++;
}

void	ft_printstr(char *s, int *len)
{
	int	i;

	i = 0;
	if (!s)
	{
		write(1, "(null)", 6);
		(*len) += 6;
		return ;
	}
	while (s[i])
	{
		ft_printchar(s[i], len);
		i++;
	}
}

void	ft_printnb(int n, int *len)
{
	int	nb;

	nb = n;
	if (nb == -2147483648)
	{
		write(1, "-2147483648", 11);
		(*len) += 11;
		return ;
	}
	if (nb < 0)
	{
		ft_printchar('-', len);
		nb = (nb * -1);
	}
	if (nb < 10)
		ft_printchar(nb + 48, len);
	if (nb > 9)
	{
		ft_printnb(nb / 10, len);
		ft_printchar((nb % 10) + '0', len);
	}
}

void	ft_printunb(unsigned int nb, int *len)
{
	if (nb > 9)
		ft_printnb(nb / 10, len);
	ft_printchar((nb % 10) + 48, len);
}
