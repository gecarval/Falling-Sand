/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bresenham.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <gecarval@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 17:53:51 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/10 20:42:11 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/renderer.h"

void	draw_line_y(t_delta x, t_delta y, t_data *data, int color)
{
	float_t	dx;
	float_t	dy;
	int		step;
	int		xi;

	dx = x.fin - x.ini;
	dy = y.fin - y.ini;
	xi = 1;
	if (dx < 0)
		xi = -1;
	dx = fabsf(dx);
	step = (2 * dx) - dy;
	while (y.ini < y.fin)
	{
		pixel_to_img_float(x.ini, y.ini, data, color);
		if (step > 0)
			x.ini += xi;
		if (step > 0)
			step += 2 * (dx - dy);
		else
			step += (2 * dx);
		y.ini++;
	}
}

void	draw_line_x(t_delta x, t_delta y, t_data *data, int color)
{
	float_t	dx;
	float_t	dy;
	int		step;
	int		yi;

	dx = x.fin - x.ini;
	dy = y.fin - y.ini;
	yi = 1;
	if (dy < 0)
		yi = -1;
	dy = fabsf(dy);
	step = (2 * dy) - dx;
	while (x.ini < x.fin)
	{
		pixel_to_img_float(x.ini, y.ini, data, color);
		if (step > 0)
			y.ini += yi;
		if (step > 0)
			step += 2 * (dy - dx);
		else
			step += (2 * dy);
		x.ini++;
	}
}

void	draw_line(t_delta x, t_delta y, t_data *data, int color)
{
	if (fabsf(x.fin - x.ini) < fabsf(y.fin - y.ini))
	{
		if (y.ini > y.fin)
			draw_line_y(revdel(x), revdel(y), data, color);
		else
			draw_line_y(x, y, data, color);
	}
	else
	{
		if (x.ini > x.fin)
			draw_line_x(revdel(x), revdel(y), data, color);
		else
			draw_line_x(x, y, data, color);
	}
}

void	drawCircle(int xc, int yc, int x, int y, t_data *data, int color) 
{
	pixel_to_img(xc + x, yc + y, data, color); 
	pixel_to_img(xc - x, yc + y, data, color); 
	pixel_to_img(xc + x, yc - y, data, color); 
	pixel_to_img(xc - x, yc - y, data, color); 
	pixel_to_img(xc + y, yc + x, data, color); 
	pixel_to_img(xc - y, yc + x, data, color); 
	pixel_to_img(xc + y, yc - x, data, color); 
	pixel_to_img(xc - y, yc - x, data, color); 
}

void	circleBres(int xc, int yc, int r, t_data *data, int color) 
{
	int x = 0, y = r; 
	int d = 3 - 2 * r; 
	drawCircle(xc, yc, x, y, data, color); 
	while (y >= x) 
	{
		x++; 
		if (d > 0) 
		{
			y--;  
			d = d + 4 * (x - y) + 10; 
		} 
		else
			d = d + 4 * x + 6; 
		drawCircle(xc, yc, x, y, data, color); 
	} 
} 

void	draw_rectang(t_objinf a, t_data *data)
{
	int	i;
	int	j;

	i = -(a.sy / 2);
	while (++i < a.sy / 2)
	{
		j = -(a.sx / 2);
		while (++j < a.sx / 2)
			pixel_to_img((j + a.pox), (i + a.poy), data, a.color);
	}
}
