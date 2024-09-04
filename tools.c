/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 17:38:51 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/04 13:45:08 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/renderer.h"

// GET DIM
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

// FREE
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

// GRAPH
void	pixel_to_img(int x, int y, t_data *data, int color)
{
	char	*pixel;

	if ((x < 0 || x >= WINX) || (y < 0 || y >= WINY))
		return ;
	pixel = data->img->img_px + y * data->img->llen + x * (data->img->bpp / 8);
	*(int *)pixel = color;
}

void	pixel_to_img_float(float_t x, float_t y, t_data *data, int color)
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
		15, 15, 120000, "renders by gecarval");
	mlx_string_put(data->ini, data->win,
		15, 30, 120000, "Press 1 for Cube");
	mlx_string_put(data->ini, data->win,
		15, 45, 120000, "Press 2 for Fluid Sim");
}

void	controls_mark(t_data *data)
{
	mlx_string_put(data->ini, data->win, 15, 15, 0xFFFFFF, "'LMB'=Brush_On ; 'RMB'=Brush_Off");
	mlx_string_put(data->ini, data->win, 15, 30, 0xFFFFFF, "','=Decrease_Brush ; '.'=Increase_Brush");
	mlx_string_put(data->ini, data->win, 15, 45, 0xFFFFFF, "Q=Empty ; W=Fire ; E=Steam");
	mlx_string_put(data->ini, data->win, 15, 60, 0xFFFFFF, "R=Water ; A=Lava ; S=Sand");
	mlx_string_put(data->ini, data->win, 15, 75, 0xFFFFFF, "D=Stone ; F=Propane ; Z=Acid");
	mlx_string_put(data->ini, data->win, 15, 90, 0xFFFFFF, "X=WOOD ; C=OIL ; V=");
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

// HOOKS
int	mouse_released(int key, t_data *data)
{
	if (key == 1)
		data->click_hold = 0;
	ft_printf("%d\n", key);
	return (0);
}

int	mouse_click(int key, int x, int y, t_data *data)
{
	(void)x;
	(void)y;
	if (key == 1)
		data->click_hold = 1;
	if (key == 3)
		data->click_hold = 0;
	return (0);
}

int	mlx_anim(t_data *data)
{
	int	time;
	int	i;

	if (data->anicub == 1)
	{
		time = 100000000;
		render_background(data, 0x000000);
		draw_vertices(data, data->iso);
		mlx_put_image_to_window(data->ini, data->win, data->img->img_ptr, 0, 0);
		water_mark(data);
		data->iso += data->anispeed;
		if (data->iso > 6.28)
			data->iso = 0.0;
		i = 0;
		while (i < time)
			i++;
	}
	if (data->anifsim == 1)
	{
		fluidsim_start(data);
		water_mark(data);
		mlx_put_image_to_window(data->ini, data->win, data->img->img_ptr, 0, 0);
		controls_mark(data);
		i = 0;
		while (i < data->timing)
			i++;
	}
	if (data->click_hold == 1)
	{
		mlx_mouse_get_pos(data->ini, data->win, &data->mposx, &data->mposy);
		put_mat(data->mposx, data->mposy, data);
	}
	return (0);
}

int	mlx_cooked(int key, t_data *data)
{
	if (key == 'c')
		data->click_fill = MAT_ID_OIL;
	if (key == 'x')
		data->click_fill = MAT_ID_WOOD;
	if (key == 'q')
		data->click_fill = MAT_ID_EMPTY;
	if (key == 'w')
		data->click_fill = MAT_ID_FIRE;
	if (key == 'e')
		data->click_fill = MAT_ID_STEAM;
	if (key == 'r')
		data->click_fill = MAT_ID_WATER;
	if (key == 'a')
		data->click_fill = MAT_ID_LAVA;
	if (key == 's')
		data->click_fill = MAT_ID_SAND;
	if (key == 'd')
		data->click_fill = MAT_ID_STONE;
	if (key == 'f')
		data->click_fill = MAT_ID_PROPANE;
	if (key == 'z')
		data->click_fill = MAT_ID_ACID;
	if (key == ESC)
		exit_data(data, 0);
	if (key == '1')
		data->anicub *= -1;
	if (key == '2')
		data->anifsim *= -1;
	if (key == '.')
		data->brush_size += 1;
	if (key == ',')
		if (data->brush_size > 0)
			data->brush_size -= 1;
	if (key == ']')
		data->timing += 10000000;
	if (key == '[')
		if (data->timing > 9999999)
			data->timing -= 10000000;
	if (key == ' ')
	{
		render_background(data, 0x000000);
		mlx_put_image_to_window(data->ini, data->win, data->img->img_ptr, 0, 0);
		water_mark(data);
	}
	if (key == 65362)
		data->anispeed += 0.0025;
	if (key == 65364)
		data->anispeed -= 0.0025;
	return (0);
}

// DELTAS
t_pt	*pt_dup(t_pt *a)
{
	t_pt	*b;

	b = (t_pt *)malloc(sizeof(t_pt));
	b->x = a->x;
	b->y = a->y;
	b->z = a->z;
	b->color = a->color;
	return (b);
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

// OTHERS
void	print_matrix(char **vec)
{
	int	i;
	int	j;

	i = -1;
	while (vec[++i])
	{
		j = -1;
		while(vec[i][++j])
			ft_printf("%c|", vec[i][j]);
		ft_printf("\n");
	}
}
