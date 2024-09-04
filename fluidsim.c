/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fluidsim.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:24:07 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/02 21:06:22 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/renderer.h"

void	render_fluidmap(t_data *data)
{
	int	y;
	int	x;

	y = -1;
	while (++y < WINY)
	{
		x = -1;
		while (++x < WINX)
		{
			if (data->fsim->map[y][x] == 'G')
				pixel_to_img(x, y, data, 0x555555);
			if (data->fsim->map[y][x] == 'P')
				pixel_to_img(x, y, data, 0x775555);
			else if (data->fsim->map[y][x] == MAT_ID_STONE)
				pixel_to_img(x, y, data, MAT_COL_STONE);
			else if (data->fsim->map[y][x] == MAT_ID_SAND)
				pixel_to_img(x, y, data, MAT_COL_SAND);
			else if (data->fsim->map[y][x] == MAT_ID_LAVA)
				pixel_to_img(x, y, data, MAT_COL_LAVA);
			else if (data->fsim->map[y][x] == MAT_ID_WATER)
				pixel_to_img(x, y, data, MAT_COL_WATER);
			else if (data->fsim->map[y][x] == MAT_ID_ACID)
				pixel_to_img(x, y, data, MAT_COL_ACID);
			else if (data->fsim->map[y][x] == MAT_ID_SALT)
				pixel_to_img(x, y, data, MAT_COL_SALT);
			else if (data->fsim->map[y][x] == MAT_ID_FIRE)
				pixel_to_img(x, y, data, MAT_COL_FIRE);
			else if (data->fsim->map[y][x] == MAT_ID_EMBER)
				pixel_to_img(x, y, data, MAT_COL_EMBER);
			else if (data->fsim->map[y][x] == MAT_ID_SMOKE)
				pixel_to_img(x, y, data, MAT_COL_SMOKE);
			else if (data->fsim->map[y][x] == MAT_ID_PROPANE)
				pixel_to_img(x, y, data, MAT_COL_PROPANE);
			else if (data->fsim->map[y][x] == MAT_ID_STEAM)
				pixel_to_img(x, y, data, MAT_COL_STEAM);
			else if (data->fsim->map[y][x] == MAT_ID_EMPTY)
				pixel_to_img(x, y, data, MAT_COL_EMPTY);
		}
	}
}

void	put_mat(int x, int y, t_data *data)
{
	int	i;
	int	j;

	if (x <= 0 || x >= WINX)
		return ;
	if (y <= 0 || y >= WINY)
		return ;
	i = y - data->brush_size;
	while (i <= y + data->brush_size)
	{
		j = x - data->brush_size;
		while(j <= x + data->brush_size)
		{
			if (i > 0 && i < WINY)
			{
				if (j > 0 && j < WINX)
				{
					if(data->fsim->map[i][j] != 'G')
					{
						if (data->click_fill == MAT_ID_EMPTY)
							data->fsim->map[i][j] = data->click_fill;
						else if (data->fsim->map[i][j] == MAT_ID_EMPTY)
							data->fsim->map[i][j] = data->click_fill;
					}
				}
			}
			j++;
		}
		i++;
	}
}

t_pt	find_id(int x, int y, t_data *data, char c)
{
	int	i;
	int	j;
	t_pt	pt;

	pt.z = 0;
	i = y + 1;
	while (i >= y - 1)
	{
		j = x - 1;
		while (j <= x + 1)
		{
			if (data->fsim->map[i][j] == c)
			{
				pt.x = j;
				pt.y = i;
				pt.z = 1;
				return (pt);
			}
			j++;
		}
		i--;
	}
	return (pt);
}

void	process_gravity(t_data *data)
{
	int	y;
	int	x;

	y = (int)WINY;
	while (--y >= 0)
	{
		x = -1;
		while (++x < (int)WINX)
		{
			if (data->fsim->map[y][x] == MAT_ID_FIRE)
				emulate_fire(x, y, data);
			else if (data->fsim->map[y][x] == MAT_ID_STEAM)
				emulate_steam(x, y, data, MAT_ID_STEAM);
			else if (data->fsim->map[y][x] == MAT_ID_ACID)
				emulate_acid(x, y, data, MAT_ID_ACID);
			else if (data->fsim->map[y][x] == MAT_ID_WATER)
				emulate_water(x, y, data, MAT_ID_WATER);
			else if (data->fsim->map[y][x] == MAT_ID_LAVA)
				emulate_lava(x, y, data, MAT_ID_LAVA);
			else if (data->fsim->map[y][x] == MAT_ID_PROPANE)
				emulate_propane(x, y, data);
			else if (data->fsim->map[y][x] == MAT_ID_SAND)
				emulate_sand(x, y, data, 2);
			else if (data->fsim->map[y][x] == MAT_ID_STONE)
				emulate_sand(x, y, data, 1);
		}
	}
}

void	fluidsim_start(t_data *data)
{
	static	int	i = 0;

	if (i <= 2000000000)
		i += 1;
	else
		i = 0;
	process_gravity(data);
	render_fluidmap(data);
}