/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 17:38:51 by gecarval          #+#    #+#             */
/*   Updated: 2024/08/29 17:43:57 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/renderer.h"

int	get_x_size(const char *s)
{
	int	i;
	int	x;

	i = 0;
	x = 0;
	while (s[i] != '\0')
	{
		if (s[i] != '\0' && ft_isdigit(s[i]))
		{
			x++;
			while (s[i + 1] != '\0' && s[i] != ' ')
				i++;
		}
		i++;
	}
	return (x);
}

int	get_y_size(const char *s)
{
	int	i;
	int	y;

	i = -1;
	y = 1;
	if (!s)
		return (0);
	while (s[++i] != '\0')
		if (s[i + 1] != '\0' && s[i] == '\n')
			y++;
	return (y);
}

void	ft_free_matrix(char **vec)
{
	int	i;

	i = -1;
	while (vec[++i])
		free(vec[i]);
	free(vec);
}

void	ft_free_tensor(char ***z)
{
	int	i;

	i = -1;
	while (z[++i])
	{
		ft_free_matrix(z[i]);
	}
	free(z);
}

void	pixel_to_img(float_t x, float_t y, t_data *data, int color)
{
	char	*pixel;

	if ((x < 0 || x >= WINX) || (y < 0 || y >= WINY))
		return ;
	pixel = data->img->img_px + (int)y * data->img->llen + (int)x * (data->img->bpp / 8);
	*(int *)pixel = color;
}

void	water_mark(t_data *data)
{
	mlx_string_put(data->ini, data->win,
		15, 20, 120000, "renders by gecarval");
}

void	defdel(t_delta *a, float_t ini, float_t fin)
{
	a->ini = ini;
	a->fin = fin;
}

t_delta	revdel(t_delta a)
{
	int	tmp;

	tmp = a.ini;
	a.ini = a.fin;
	a.fin = tmp;
	return (a);
}

void	render_background(t_data *data, int color)
{
	int	x;
	int	y;

	x = -1;
	while (++x < WINX)
	{
		y = -1;
		while (++y < WINY)
			pixel_to_img(x, y, data, color);
	}
}

int	mlx_anim(t_data *data)
{
	if (data->ani == 1)
	{
		render_background(data, 0x000000);
		draw_vertices(data, data->iso);
		mlx_put_image_to_window(data->ini, data->win, data->img->img_ptr, 0, 0);
		water_mark(data);
		data->iso += data->anispeed;
		if (data->iso > 6.28)
			data->iso = 0.0;
		for(int i = 0; i < 100000000; i++)
			;
	}
	return (0);
}

int	mlx_cooked(int key, t_data *data)
{
	if (key == ESC)
		exit_data(data, 0);
	if (key == 32)
	{
		if (data->ani == 0)
			data->ani = 1;
		else
			data->ani = 0;
	}
	if (key == 65362)
		data->anispeed += 0.005;
	if (key == 65364)
		data->anispeed -= 0.005;
	return (0);
}
