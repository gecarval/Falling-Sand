/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fluidsim.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 14:24:07 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/10 20:36:29 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/renderer.h"

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
				post_processing(x, y, data, MAT_ID_LAVA, MAT_COL_FIREG);
				post_processing(x, y, data, MAT_ID_MOLTENIRON, MAT_COL_MOLTENIRONG);
				post_processing(x, y, data, MAT_ID_HOTIRON, MAT_COL_HOTIRONG);
				post_processing(x, y, data, MAT_ID_WATER, MAT_COL_WATER);
				post_processing(x, y, data, MAT_ID_BUBBLE, MAT_COL_BUBBLE);
				post_processing(x, y, data, MAT_ID_HIDROGEN, MAT_COL_HIDROGENG);
				post_processing(x, y, data, MAT_ID_OXYGEN, MAT_COL_OXYGENG);
				post_processing(x, y, data, MAT_ID_FOG, MAT_COL_FOGG);
				post_processing(x, y, data, MAT_ID_GLASS, MAT_COL_GLASSF);
			}
			else if (data->fsim->map[y][x] == MAT_ID_HOTIRON)
				process_material(x, y, data, MAT_COL_HOTIRON);
			else if (data->fsim->map[y][x] == MAT_ID_IRON)
				process_material(x, y, data, MAT_COL_IRON);
			else if (data->fsim->map[y][x] == MAT_ID_RUST)
				process_material(x, y, data, MAT_COL_RUST);
			else if (data->fsim->map[y][x] == MAT_ID_GLASSF)
				process_material(x, y, data, MAT_COL_GLASSF);
			else if (data->fsim->map[y][x] == MAT_ID_GLASS)
			{
				if ((y + x) % 4)
					process_material(x, y, data, MAT_COL_GLASS);
				else
					process_material(x, y, data, MAT_COL_GLASS + 0x222233);
			}
			else if (data->fsim->map[y][x] == MAT_ID_WOODF)
				process_material(x, y, data, MAT_COL_WOODF);
			else if (data->fsim->map[y][x] == MAT_ID_WOOD)
				process_material(x, y, data, MAT_COL_WOOD);
			else if (data->fsim->map[y][x] == MAT_ID_STONE)
				process_material(x, y, data, MAT_COL_STONE);
			else if (data->fsim->map[y][x] == MAT_ID_WETSAND)
			{
				if (((y % 3) + x % 3) > 0)
					process_material(x, y, data, MAT_COL_WETSAND);
				else
					process_material(x, y, data, MAT_COL_WETSAND - 0x111111);
			}
			else if (data->fsim->map[y][x] == MAT_ID_SAND)
			{
				if (((y % 3) + x % 3) > 0)
					process_material(x, y, data, MAT_COL_SAND);
				else
					process_material(x, y, data, MAT_COL_SAND - 0x111111);
			}
			else if (data->fsim->map[y][x] == MAT_ID_SOAP)
				process_material(x, y, data, MAT_COL_SOAP);
			else if (data->fsim->map[y][x] == MAT_ID_GUNPOWDER)
				process_material(x, y, data, MAT_COL_GUNPOWDER);
			else if (data->fsim->map[y][x] == MAT_ID_MOLTENIRON)
				process_material(x, y, data, MAT_COL_MOLTENIRON);
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
			{
				if (rand() % 20)
				process_material(x, y, data, MAT_COL_FIRE);
				else
				process_material(x, y, data, MAT_COL_FIREF);

			}
			else if (data->fsim->map[y][x] == MAT_ID_EMBER)
				process_material(x, y, data, MAT_COL_EMBER);
			else if (data->fsim->map[y][x] == MAT_ID_SMOKE)
			{
				if (rand() % 7)
					process_material(x, y, data, MAT_COL_SMOKE);
				else
					process_material(x, y, data, MAT_COL_SMOKEG);
			}
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
			else if (data->fsim->map[y][x] == MAT_ID_MISSILE)
				process_material(x, y, data, MAT_COL_MISSILE);
		}
	}
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
			else if (data->fsim->map[y][x] == MAT_ID_GUNPOWDER)		//GUNPOWDER
				emulate_gunpowder(x, y, data);
			else if (data->fsim->map[y][x] == MAT_ID_SOAP)		//SOAP
				emulate_soap(x, y, data);
			else if (data->fsim->map[y][x] == MAT_ID_SAND)		//SAND
				emulate_solid(x, y, data, 2, 3, 4, 1, 10);
			else if (data->fsim->map[y][x] == MAT_ID_WETSAND)	//WET SAND
				emulate_wetsand(x, y, data, 2, 2, 5, 2, 6);
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
			else if (data->fsim->map[y][x] == MAT_ID_IRON)		//IRON
				emulate_iron(x, y, data);
			else if (data->fsim->map[y][x] == MAT_ID_HOTIRON)	//HOT IRON
				emulate_hotiron(x, y, data);
			else if (data->fsim->map[y][x] == MAT_ID_MOLTENIRON)	//MOLTEN IRON
				emulate_molteniron(x, y, data, MAT_ID_MOLTENIRON);
			else if (data->fsim->map[y][x] == MAT_ID_RUST)		//RUST
				emulate_rust(x, y, data);
			else if (data->fsim->map[y][x] == MAT_ID_MISSILE)		//MISSILE
				emulate_missile(x, y, data);
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

void	render_per_pixel(int x, int y, t_data *data)
{
	render_fluidmap_pp(x, y, data);
	if (x + 1 < WINX)
		render_fluidmap_pp(x + 1, y, data);
	if (x - 1 > 0)
		render_fluidmap_pp(x - 1, y, data);
	if (y + 1 < WINY)
		render_fluidmap_pp(x, y + 1, data);
	if (y - 1 > 0)
		render_fluidmap_pp(x, y - 1, data);
	if (y - 1 > 0 && x - 1 > 0)
		render_fluidmap_pp(x - 1, y - 1, data);
	if (y - 1 > 0 && x + 1 < WINX)
		render_fluidmap_pp(x + 1, y - 1, data);
	if (y + 1 < WINY && x - 1 > 0)
		render_fluidmap_pp(x - 1, y + 1, data);
	if (y + 1 < WINY && x + 1 < WINX)
		render_fluidmap_pp(x + 1, y + 1, data);
}

void	post_processing_pp(int x, int y, t_data *data, char id, int col)
{
	if (data->fsim->map[y][x] != id)
		return ;
	if (x + 1 < WINX)
	{
		if (data->fsim->map[y][x - 1] == MAT_ID_EMPTY)
		{
			pixel_to_img((x - 1) * 2 + 1, y * 2, data, col);
			pixel_to_img((x - 1) * 2 + 1, y * 2 + 1, data, col);
		}
	}
	if (x - 1 > 0)
	{
		if (data->fsim->map[y][x + 1] == MAT_ID_EMPTY)
		{
			pixel_to_img((x + 1) * 2, y * 2, data, col);
			pixel_to_img((x + 1) * 2, y * 2 + 1, data, col);
		}
	}
	if (y + 1 < WINY)
	{
		if (data->fsim->map[y - 1][x] == MAT_ID_EMPTY)
		{
			pixel_to_img(x * 2, (y - 1) * 2 + 1, data, col);
			pixel_to_img(x * 2 + 1, (y - 1) * 2 + 1, data, col);
		}
	}
	if (y - 1 > 0)
	{
		if (data->fsim->map[y + 1][x] == MAT_ID_EMPTY)
		{
			pixel_to_img(x * 2, (y + 1) * 2, data, col);
			pixel_to_img(x * 2 + 1, (y + 1) * 2, data, col);
		}
	}
}

void	render_fluidmap_pp(int x, int y, t_data *data)
{
	if (data->fsim->map[y][x] == 'z')
		process_material(x, y, data, 0x555555);
	else if (data->fsim->map[y][x] == MAT_ID_EMPTY)
		process_material(x, y, data, MAT_COL_EMPTY);
	else if (data->fsim->map[y][x] == MAT_ID_GLASSF)
		process_material(x, y, data, MAT_COL_GLASSF);
	else if (data->fsim->map[y][x] == MAT_ID_GLASS)
	{
		if ((y + x) % 4)
			process_material(x, y, data, MAT_COL_GLASS);
		else
			process_material(x, y, data, MAT_COL_GLASS + 0x222233);
	}
	else if (data->fsim->map[y][x] == MAT_ID_WOODF)
		process_material(x, y, data, MAT_COL_WOODF);
	else if (data->fsim->map[y][x] == MAT_ID_WOOD)
		process_material(x, y, data, MAT_COL_WOOD);
	else if (data->fsim->map[y][x] == MAT_ID_STONE)
		process_material(x, y, data, MAT_COL_STONE);
	else if (data->fsim->map[y][x] == MAT_ID_WETSAND)
	{
		if (((y % 3) + x % 3) > 0)
			process_material(x, y, data, MAT_COL_WETSAND);
		else
			process_material(x, y, data, MAT_COL_WETSAND - 0x111111);
	}
	else if (data->fsim->map[y][x] == MAT_ID_SAND)
	{
		if (((y % 3) + x % 3) > 0)
			process_material(x, y, data, MAT_COL_SAND);
		else
			process_material(x, y, data, MAT_COL_SAND - 0x111111);
	}
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
	{
		if (rand() % 20)
			process_material(x, y, data, MAT_COL_FIRE);
		else
			process_material(x, y, data, MAT_COL_FIREF);

	}
	else if (data->fsim->map[y][x] == MAT_ID_EMBER)
		process_material(x, y, data, MAT_COL_EMBER);
	else if (data->fsim->map[y][x] == MAT_ID_SMOKE)
	{
		if (rand() % 7)
			process_material(x, y, data, MAT_COL_SMOKE);
		else
			process_material(x, y, data, MAT_COL_SMOKEG);
	}
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
	post_processing_pp(x, y, data, MAT_ID_FIRE, MAT_COL_FIREG);
	post_processing_pp(x, y, data, MAT_ID_LAVA, MAT_COL_FIREG);
	post_processing_pp(x, y, data, MAT_ID_WATER, MAT_COL_WATER);
	post_processing_pp(x, y, data, MAT_ID_BUBBLE, MAT_COL_BUBBLE);
	post_processing_pp(x, y, data, MAT_ID_HIDROGEN, MAT_COL_HIDROGENG);
	post_processing_pp(x, y, data, MAT_ID_OXYGEN, MAT_COL_OXYGENG);
	post_processing_pp(x, y, data, MAT_ID_FOG, MAT_COL_FOGG);
	post_processing_pp(x, y, data, MAT_ID_GLASS, MAT_COL_GLASSF);
}

void	circle_calc(int xc, int yc, int x, int y, t_data *data)
{
	if (xc <= 0 || xc >= WINX)
		return ;
	if (yc <= 0 || yc >= WINX)
		return ;
	if ((yc + y > 0 && yc + y < WINY) && (xc + x > 0 && xc + x < WINX))
	{
		if(data->fsim->map[yc + y][xc + x] != 'z')
		{
			if (data->click_fill == MAT_ID_EMPTY)
				data->fsim->map[yc + y][xc + x] = data->click_fill;
			else if (data->fsim->map[yc + y][xc + x] == MAT_ID_EMPTY)
				data->fsim->map[yc + y][xc + x] = data->click_fill;
		}
	}
	if ((yc + y > 0 && yc + y < WINY) && (xc - x > 0 && xc - x < WINX))
	{
		if(data->fsim->map[yc + y][xc - x] != 'z')
		{
			if (data->click_fill == MAT_ID_EMPTY)
				data->fsim->map[yc + y][xc - x] = data->click_fill;
			else if (data->fsim->map[yc + y][xc - x] == MAT_ID_EMPTY)
				data->fsim->map[yc + y][xc - x] = data->click_fill;
		}
	}
	if ((yc - y > 0 && yc - y < WINY) && (xc + x > 0 && xc + x < WINX))
	{
		if(data->fsim->map[yc - y][xc + x] != 'z')
		{
			if (data->click_fill == MAT_ID_EMPTY)
				data->fsim->map[yc - y][xc + x] = data->click_fill;
			else if (data->fsim->map[yc - y][xc + x] == MAT_ID_EMPTY)
				data->fsim->map[yc - y][xc + x] = data->click_fill;
		}
	}
	if ((yc - y > 0 && yc - y < WINY) && (xc - x > 0 && xc - x < WINX))
	{
		if(data->fsim->map[yc - y][xc - x] != 'z')
		{
			if (data->click_fill == MAT_ID_EMPTY)
				data->fsim->map[yc - y][xc - x] = data->click_fill;
			else if (data->fsim->map[yc - y][xc - x] == MAT_ID_EMPTY)
				data->fsim->map[yc - y][xc - x] = data->click_fill;
		}
	}
	if ((yc + x > 0 && yc + x < WINY) && (xc + y > 0 && xc + y < WINX))
	{
		if(data->fsim->map[yc + x][xc + y] != 'z')
		{
			if (data->click_fill == MAT_ID_EMPTY)
				data->fsim->map[yc + x][xc + y] = data->click_fill;
			else if (data->fsim->map[yc + x][xc + y] == MAT_ID_EMPTY)
				data->fsim->map[yc + x][xc + y] = data->click_fill;
		}
	}
	if ((yc + x > 0 && yc + x < WINY) && (xc - y > 0 && xc - y < WINX))
	{
		if(data->fsim->map[yc + x][xc - y] != 'z')
		{
			if (data->click_fill == MAT_ID_EMPTY)
				data->fsim->map[yc + x][xc - y] = data->click_fill;
			else if (data->fsim->map[yc + x][xc - y] == MAT_ID_EMPTY)
				data->fsim->map[yc + x][xc - y] = data->click_fill;
		}
	}
	if ((yc - x > 0 && yc - x < WINY) && (xc + y > 0 && xc + y < WINX))
	{
		if(data->fsim->map[yc - x][xc + y] != 'z')
		{
			if (data->click_fill == MAT_ID_EMPTY)
				data->fsim->map[yc - x][xc + y] = data->click_fill;
			else if (data->fsim->map[yc - x][xc + y] == MAT_ID_EMPTY)
				data->fsim->map[yc - x][xc + y] = data->click_fill;
		}
	}
	if ((yc - x > 0 && yc - x < WINY) && (xc - y > 0 && xc - y < WINX))
	{
		if(data->fsim->map[yc - x][xc - y] != 'z')
		{
			if (data->click_fill == MAT_ID_EMPTY)
				data->fsim->map[yc - x][xc - y] = data->click_fill;
			else if (data->fsim->map[yc - x][xc - y] == MAT_ID_EMPTY)
				data->fsim->map[yc - x][xc - y] = data->click_fill;
		}
	}
}

void	circle_putmat(int xc, int yc, int r, t_data *data)
{
	int	x;
	int	y;
	int	d;

	if (r < 0)
		return ;
	x = 0;
	y = r;
	d = 3 - 2 * r;
	circle_calc(xc, yc, x, y, data);
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
		circle_calc(xc, yc, x, y, data);
	}
	circle_putmat(xc, yc, r - 1, data);
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
