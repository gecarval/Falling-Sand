/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fluidsim.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:24:07 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/06 21:22:32 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/renderer.h"

void	post_processing(int x, int y, t_data *data, int id, int col)
{
	if (data->fsim->map[y][x + 1] == id)
	{
		pixel_to_img(x * 2 + 1, y * 2, data, col);
		pixel_to_img(x * 2 + 1, y * 2 + 1, data, col);
	}
	if (data->fsim->map[y][x - 1] == id)
	{
		pixel_to_img(x * 2, y * 2, data, col);
		pixel_to_img(x * 2, y * 2 + 1, data, col);
	}
	if (data->fsim->map[y + 1][x] == id)
	{
		pixel_to_img(x * 2, y * 2 + 1, data, col);
		pixel_to_img(x * 2 + 1, y * 2 + 1, data, col);
	}
	if (data->fsim->map[y - 1][x] == id)
	{
		pixel_to_img(x * 2, y * 2, data, col);
		pixel_to_img(x * 2 + 1, y * 2, data, col);
	}
}

void	process_material(int x, int y, t_data *data, int col)
{
	pixel_to_img(x * 2, y * 2, data, col);
	pixel_to_img(x * 2, y * 2 + 1, data, col);
	pixel_to_img(x * 2 + 1, y * 2, data, col);
	pixel_to_img(x * 2 + 1, y * 2 + 1, data, col);
}

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
			if (data->fsim->map[y][x] == 'z')
				process_material(x, y, data, 0x555555);
			else if (data->fsim->map[y][x] == MAT_ID_EMPTY)
			{
				process_material(x, y, data, MAT_COL_EMPTY);
				post_processing(x, y, data, MAT_ID_FIRE, MAT_COL_FIREG);
				post_processing(x, y, data, MAT_ID_BUBBLE, MAT_COL_BUBBLE);
				post_processing(x, y, data, MAT_ID_HIDROGEN, MAT_COL_HIDROGENG);
				post_processing(x, y, data, MAT_ID_OXYGEN, MAT_COL_OXYGENG);
				post_processing(x, y, data, MAT_ID_FOG, MAT_COL_FOGG);
				post_processing(x, y, data, MAT_ID_GLASS, MAT_COL_GLASSF);
			}
			else if (data->fsim->map[y][x] == MAT_ID_GLASSF)
				process_material(x, y, data, MAT_COL_GLASSF);
			else if (data->fsim->map[y][x] == MAT_ID_GLASS)
				process_material(x, y, data, MAT_COL_GLASS);
			else if (data->fsim->map[y][x] == MAT_ID_WOODF)
				process_material(x, y, data, MAT_COL_WOODF);
			else if (data->fsim->map[y][x] == MAT_ID_WOOD)
				process_material(x, y, data, MAT_COL_WOOD);
			else if (data->fsim->map[y][x] == MAT_ID_STONE)
				process_material(x, y, data, MAT_COL_STONE);
			else if (data->fsim->map[y][x] == MAT_ID_SAND)
				process_material(x, y, data, MAT_COL_SAND);
			else if (data->fsim->map[y][x] == MAT_ID_SOAP)
				process_material(x, y, data, MAT_COL_SOAP);
			else if (data->fsim->map[y][x] == MAT_ID_GUNPOWDER)
				process_material(x, y, data, MAT_COL_GUNPOWDER);
			else if (data->fsim->map[y][x] == MAT_ID_LAVA)
				process_material(x, y, data, MAT_COL_LAVA);
			else if (data->fsim->map[y][x] == MAT_ID_WATER)
				process_material(x, y, data, MAT_COL_WATER);
			else if (data->fsim->map[y][x] == MAT_ID_OIL)
				process_material(x, y, data, MAT_COL_OIL);
			else if (data->fsim->map[y][x] == MAT_ID_OILF)
				process_material(x, y, data, MAT_COL_OILF);
			else if (data->fsim->map[y][x] == MAT_ID_ACID)
				process_material(x, y, data, MAT_COL_ACID);
			else if (data->fsim->map[y][x] == MAT_ID_SALT)
				process_material(x, y, data, MAT_COL_SALT);
			else if (data->fsim->map[y][x] == MAT_ID_FLY)
				process_material(x, y, data, MAT_COL_FLY);
			else if (data->fsim->map[y][x] == MAT_ID_FOG)
				process_material(x, y, data, MAT_COL_FOG);
			else if (data->fsim->map[y][x] == MAT_ID_FIRE)
				process_material(x, y, data, MAT_COL_FIRE);
			else if (data->fsim->map[y][x] == MAT_ID_EMBER)
				process_material(x, y, data, MAT_COL_EMBER);
			else if (data->fsim->map[y][x] == MAT_ID_SMOKE)
				process_material(x, y, data, MAT_COL_SMOKE);
			else if (data->fsim->map[y][x] == MAT_ID_PROPANE)
				process_material(x, y, data, MAT_COL_PROPANE);
			else if (data->fsim->map[y][x] == MAT_ID_STEAM)
				process_material(x, y, data, MAT_COL_STEAM);
			else if (data->fsim->map[y][x] == MAT_ID_BUBBLE)
				process_material(x, y, data, MAT_COL_EMPTY);
			else if (data->fsim->map[y][x] == MAT_ID_OXYGEN)
				process_material(x, y, data, MAT_COL_OXYGEN);
			else if (data->fsim->map[y][x] == MAT_ID_HIDROGEN)
				process_material(x, y, data, MAT_COL_HIDROGEN);
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
			if ((i > 0 && i < WINY) && (j > 0 && j < WINX))
			{
				if(data->fsim->map[i][j] != 'z')
				{
					if (data->click_fill == MAT_ID_EMPTY)
						data->fsim->map[i][j] = data->click_fill;
					else if (data->fsim->map[i][j] == MAT_ID_EMPTY)
						data->fsim->map[i][j] = data->click_fill;
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

t_pt	find_around_id(int x, int y, t_data *data, char c, int dist)
{
	int	i;
	int	j;
	t_pt	pt;

	pt.z = 0;
	i = y + dist;
	while (i >= y - dist)
	{
		j = x - dist;
		while (j <= x + dist)
		{
			if (!(x == j && y == i))
			{
				if ((j > 0 && j < WINX) && (i > 0 && i < WINY))
				{
					if (data->fsim->map[i][j] == c)
					{
						pt.x = j;
						pt.y = i;
						pt.z = 1;
						return (pt);
					}
				}
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

	y = WINY;
	while (--y > 0)
	{
		x = 0;
		while (++x < WINX)
		{
			if (data->fsim->map[y][x] == MAT_ID_EMPTY)		//EMPTY
				;
			else if (data->fsim->map[y][x] == MAT_ID_HIDROGEN)	//HIDROGEN
				emulate_hidrogen(x, y, data);
			else if (data->fsim->map[y][x] == MAT_ID_OXYGEN)	//OXYGEN
				emulate_oxygen(x, y, data);
			else if (data->fsim->map[y][x] == MAT_ID_BUBBLE)	//BUBBLE
				emulate_soap_bubble(x, y, data);
			else if (data->fsim->map[y][x] == MAT_ID_STEAM)		//STEAM
				emulate_steam(x, y, data, MAT_ID_STEAM);
			else if (data->fsim->map[y][x] == MAT_ID_PROPANE)	//PROPANE
				emulate_propane(x, y, data);
			else if (data->fsim->map[y][x] == MAT_ID_FIRE)		//FIRE
				emulate_fire(x, y, data);
			else if (data->fsim->map[y][x] == MAT_ID_SMOKE)		//SMOKE
				emulate_smoke(x, y, data);
			else if (data->fsim->map[y][x] == MAT_ID_FOG)		//FOG
				emulate_fog(x, y, data);
			else if (data->fsim->map[y][x] == MAT_ID_FLY)		//FLY
				emulate_fly(x, y, data);
			else if (data->fsim->map[y][x] == MAT_ID_ACID)		//ACID
				emulate_acid(x, y, data, MAT_ID_ACID);
			else if (data->fsim->map[y][x] == MAT_ID_OIL)		//OIL
				emulate_oil(x, y, data, MAT_ID_OIL);
			else if (data->fsim->map[y][x] == MAT_ID_OILF)		//OIL ON FIRE
				emulate_oilf(x, y, data, MAT_ID_OILF);
			else if (data->fsim->map[y][x] == MAT_ID_WATER)		//WATER
				emulate_water(x, y, data, MAT_ID_WATER);
			else if (data->fsim->map[y][x] == MAT_ID_LAVA)		//LAVA
				emulate_lava(x, y, data, MAT_ID_LAVA);
			else if (data->fsim->map[y][x] == MAT_ID_SOAP)		//SOAP
				emulate_soap(x, y, data);
			else if (data->fsim->map[y][x] == MAT_ID_SAND)		//SAND
				emulate_solid(x, y, data, 2, 3, 4, 1, 10);
			else if (data->fsim->map[y][x] == MAT_ID_STONE)		//STONE
				emulate_solid(x, y, data, 1, -1, 8, 10, 1);
			else if (data->fsim->map[y][x] == MAT_ID_WOOD)		//WOOD
				emulate_wood(x, y, data);
			else if (data->fsim->map[y][x] == MAT_ID_WOODF)		//WOOD ON FIRE
				emulate_woodf(x, y, data);
			else if (data->fsim->map[y][x] == MAT_ID_GLASS)		//GLASS
				;
			else if (data->fsim->map[y][x] == MAT_ID_GLASSF)	//GLASS ON FIRE
				emulate_glassf(x, y, data);
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
