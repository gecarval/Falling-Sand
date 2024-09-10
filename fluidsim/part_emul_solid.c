/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_emul_solid.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 20:01:00 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/10 20:37:29 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/renderer.h"

int	emulate_wetsand(int x, int y, t_data *data, int randed, int slide, int force, int inertialchance, int inertialres)
{
	t_pt	pt;

	if (rand() % 4 == 0)
	{
		pt = find_id(x, y, data, MAT_ID_FIRE);
		if (pt.z == 1)
		{
			data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_STEAM;
			data->fsim->map[y][x] = MAT_ID_SAND;
			return (1);
		}
	}
	if (rand() % 3 == 0)
	{
		pt = find_id(x, y, data, MAT_ID_LAVA);
		if (pt.z == 1)
		{
			pt = find_around_id(x, y, data, MAT_ID_EMPTY, 5);
			if (pt.z == 1)
			{
				data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_STEAM;
				data->fsim->map[y][x] = MAT_ID_SAND;
				pt = find_id(x, y, data, MAT_ID_LAVA);
				data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_STONE;
				return (1);
			}
		}
	}
	if (emulate_solid(x, y, data, randed, slide, force, inertialchance, inertialres))
		return (1);
	return (0);
}

void	emulate_soap(int x, int y, t_data *data)
{
	t_pt	pt;

	pt = find_id(x, y, data, MAT_ID_WATER);
	if (pt.z == 1)
	{
		if (rand() % 100 == 0)
		{
			data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_EMPTY;
			data->fsim->map[y][x] = MAT_ID_BUBBLE;
			return ;
		}
		if (rand() % 100 == 0)
		{
			data->fsim->map[y][x] = MAT_ID_BUBBLE;
			return ;
		}
	}
	if (y < (int)WINY - 2 && emulate_solid(x, y, data, 1, 2, 1, 3, 3) == 1)
		return ;
}

int	emulate_soap_bubble(int x, int y, t_data *data)
{
	int		i;
	int		j;
	char		swap;
	static int	iter[WINY][WINX];
	static int	update[WINY][WINX];

	if (update[y][x] > 0)
	{
		update[y][x] = 0;
		return (1);
	}
	i = y + 2;
	while (--i >= y - 1)
	{
		j = x - 2;
		while (++j <= x + 1)
		{
			if (data->fsim->map[i][j] != MAT_ID_EMPTY)
			{
				if (rand() % 75 == 0)
				{
					update[y][x] = 0;
					iter[y][x] = 0;
					data->fsim->map[y][x] = MAT_ID_WATER;
					return (1);
				}
			}
		}
	}
	if (rand() % 3 == 0)
	{
		iter[y][x] = 0;
		return (1);
	}
	if (rand() % 3 && data->fsim->map[y - 1][x] < data->fsim->map[y][x])
	{
		if (data->fsim->map[y - 1][x - 1] == MAT_ID_EMPTY)
		{
			swap = data->fsim->map[y - 1][x - 1];
			data->fsim->map[y - 1][x - 1] = data->fsim->map[y - 1][x];
			data->fsim->map[y - 1][x] = swap;
		}
		else if (data->fsim->map[y - 1][x + 1] == MAT_ID_EMPTY)
		{
			swap = data->fsim->map[y - 1][x + 1];
			data->fsim->map[y - 1][x + 1] = data->fsim->map[y - 1][x];
			data->fsim->map[y - 1][x] = swap;
		}
		swap = data->fsim->map[y - 1][x];
		data->fsim->map[y - 1][x] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		update[y - 1][x] += update[y][x] + 1;
		update[y][x] = 0;
		if (rand() % 4 && iter[y][x] < 3)
		{
			iter[y - 1][x] += iter[y][x] + 1;
			update[y - 1][x] = 0;
			iter[y][x] = 0;
			emulate_fall(x, y - 1, data, 2, 2, 0);
		}
		else
			iter[y][x] = 0;
		return (1);
	}	
	if (rand() % 2 && data->fsim->map[y - 1][x - 1] < data->fsim->map[y][x])
	{
		swap = data->fsim->map[y - 1][x - 1];
		data->fsim->map[y - 1][x - 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		update[y - 1][x - 1] += update[y][x] + 1;
		update[y][x] = 0;
		if (rand() % 4 && iter[y][x] < 3)
		{
			iter[y - 1][x - 1] += iter[y][x] + 1;
			update[y - 1][x - 1] = 0;
			iter[y][x] = 0;
			emulate_fall(x - 1, y - 1, data, 2, 2, 0);
		}
		else
			iter[y][x] = 0;
		return (1);
	}	
	if (rand() % 2 && data->fsim->map[y - 1][x + 1] < data->fsim->map[y][x])
	{
		swap = data->fsim->map[y - 1][x + 1];
		data->fsim->map[y - 1][x + 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		update[y - 1][x + 1] += update[y][x] + 1;
		update[y][x] = 0;
		if (rand() % 4 && iter[y][x] < 3)
		{
			iter[y - 1][x + 1] += iter[y][x] + 1;
			update[y - 1][x + 1] = 0;
			iter[y][x] = 0;
			emulate_fall(x + 1, y - 1, data, 2, 2, 0);
		}
		else
			iter[y][x] = 0;
		return (1);
	}
	return (0);
}

void	emulate_iron(int x, int y, t_data *data)
{
	t_pt	pt;

	if (rand() % 75)
	{
		pt = find_id(x, y, data, MAT_ID_OXYGEN);
		if (pt.z == 1)
		{
			data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_EMPTY;
			data->fsim->map[y][x] = MAT_ID_RUST;
			return ;
		}
	}
	if (rand() % 50)
	{
		pt = find_id(x, y, data, MAT_ID_FIRE);
		if (pt.z == 1)
		{
			data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_SMOKE;
			data->fsim->map[y][x] = MAT_ID_HOTIRON;
			return ;
		}
	}
	if (rand() % 30)
	{
		pt = find_id(x, y, data, MAT_ID_LAVA);
		if (pt.z == 1)
		{
			data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_STONE;
			data->fsim->map[y][x] = MAT_ID_HOTIRON;
			return ;
		}
	}
}

void	emulate_hotiron(int x, int y, t_data *data)
{
	t_pt	pt;
	static int	update[WINY][WINX];
	static int	iter[WINY][WINX];

	if (update[y][x] > 0)
	{
		update[y][x] = 0;
		return ;
	}
	if (rand() % 50 == 0)
	{
		pt = find_id(x, y, data, MAT_ID_OXYGEN);
		if (pt.z == 1)
		{
			data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_FIRE;
			if (rand() % 25 == 0)
				data->fsim->map[y][x] = MAT_ID_IRON;
			return ;
		}
	}
	if (rand() % 50 == 0)
	{
		pt = find_id(x, y, data, MAT_ID_HIDROGEN);
		if (pt.z == 1)
		{
			data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_FIRE;
			if (rand() % 25 == 0)
				data->fsim->map[y][x] = MAT_ID_IRON;
			return ;
		}
	}
	if (rand() % 30 == 0)
	{
		pt = find_id(x, y, data, MAT_ID_WATER);
		if (pt.z == 1)
		{
			data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_STEAM;
			if (rand() % 5)
				data->fsim->map[y][x] = MAT_ID_IRON;
			return ;
		}
	}
	if (rand() % 100 == 0)
	{
		pt = find_id(x, y, data, MAT_ID_SAND);
		if (pt.z == 1)
		{
			data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_GLASSF;
			data->fsim->map[y][x] = MAT_ID_IRON;
			return ;
		}
	}
	if (rand() % 750 == 0)
	{
		pt = find_id(x, y, data, MAT_ID_FIRE);
		if (pt.z == 1)
		{
			data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_SMOKE;
			data->fsim->map[y][x] = MAT_ID_MOLTENIRON;
			return ;
		}
	}
	if (rand() % 750 == 0)
	{
		pt = find_id(x, y, data, MAT_ID_LAVA);
		if (pt.z == 1)
		{
			data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_STONE;
			data->fsim->map[y][x] = MAT_ID_MOLTENIRON;
			return ;
		}
	}
	if (rand() % 1500 == 0)
	{
		data->fsim->map[y][x] = MAT_ID_IRON;
		return ;
	}
	if (iter[y][x] < 5)
	{
		if (iter[y][x] > 0)
		{
			if (rand() % 5 == 0)
			{
				iter[y][x] = 0;
				return ;
			}
		}
		pt = ft_heat_sink(x, y, data);
		update[(int)pt.y][(int)pt.x] += update[y][x] + 1;
		iter[(int)pt.y][(int)pt.x] += iter[y][x] + 1;
		iter[y][x] = 0;
		update[y][x] = 0;
	}
	else
		iter[y][x] = 0;
}

void	emulate_molteniron(int x, int y, t_data *data, char c)
{
	t_pt		pt;
	static int	update[WINY][WINX];

	if (update[y][x] > 0)
	{
		update[y][x] = 0;
		return ;
	}
	if (rand() % 4 == 0)
	{
		pt = find_id(x, y, data, MAT_ID_WATER);
		if (pt.z == 1)
		{
			data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_STEAM;
			data->fsim->map[y][x] = MAT_ID_HOTIRON;
			return ;
		}
	}
	if (rand() % 10 == 0)
	{
		pt = find_id(x, y, data, MAT_ID_SAND);
		if (pt.z == 1)
		{
			data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_GLASSF;
			data->fsim->map[y][x] = MAT_ID_HOTIRON;
			return ;
		}
	}
	if (rand() % 75 == 0)
	{
		pt = find_id(x, y, data, MAT_ID_IRON);
		if (pt.z == 1)
		{
			data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_HOTIRON;
			if (rand() % 15)
				data->fsim->map[y][x] = MAT_ID_HOTIRON;
			return ;
		}
	}
	if (rand() % 150 == 0)
	{
		pt = find_id(x, y, data, MAT_ID_HOTIRON);
		if (pt.z == 1)
		{
			data->fsim->map[(int)pt.y][(int)pt.x] = c;
			if (rand() % 50)
				data->fsim->map[y][x] = MAT_ID_HOTIRON;
			return ;
		}
	}
	if (rand() % 3 == 0)
		return ;
	if (y < WINY - 2 && emulate_fall(x, y, data, 1, -2, 2))
		return ;
	if (data->fsim->map[y][x] == c)
	{
		if (rand() % 2 && data->fsim->map[y][x - 1] < c)
		{
			data->fsim->map[y][x] = data->fsim->map[y][x - 1];
			data->fsim->map[y][x - 1] = c;
			update[y][x] = 0;
			return ;
		}
		if (rand() % 2 && data->fsim->map[y][x + 1] < c)
		{
			data->fsim->map[y][x] = data->fsim->map[y][x + 1];
			data->fsim->map[y][x + 1] = c;
			update[y][x + 1] += update[y][x] + 1;
			update[y][x] = 0;
			return ;
		}
	}
}

void	emulate_rust(int x, int y, t_data *data)
{
	t_pt	pt;

	pt = find_id(x, y, data, MAT_ID_FIRE);
	if (pt.z == 1)
	{
		data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_SMOKE;
		data->fsim->map[y][x] = MAT_ID_IRON;
		return ;
	}
	if (rand() % 75 == 0)
	{
		pt = find_id(x, y, data, MAT_ID_HOTIRON);
		if (pt.z == 1)
			data->fsim->map[y][x] = MAT_ID_IRON;
		return ;
	}
	if (rand() % 50 == 0)
	{
		pt = find_id(x, y, data, MAT_ID_MOLTENIRON);
		if (pt.z == 1)
			data->fsim->map[y][x] = MAT_ID_IRON;
		return ;
	}
	if (rand() % 500)
	{
		pt = find_id(x, y, data, MAT_ID_IRON);
		if (pt.z == 1)
			return ;
	}
	if (emulate_solid(x, y, data, 2, 2, 5, 10, 1) == 1)
		return ;
}

void	emulate_wood(int x, int y, t_data *data)
{
	t_pt	pt;

	pt = find_id(x, y, data, MAT_ID_LAVA);
	if (pt.z == 1)
	{
		data->fsim->map[y][x] = MAT_ID_WOODF;
		return ;
	}
	pt = find_id(x, y, data, MAT_ID_FIRE);
	if (pt.z == 1)
	{
		data->fsim->map[y][x] = MAT_ID_WOODF;
		return ;
	}
	if (rand() % 50)
	{
		pt = find_id(x, y, data, MAT_ID_HOTIRON);
		if (pt.z == 1)
		{
			data->fsim->map[y][x] = MAT_ID_WOODF;
			if (rand() % 25)
				data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_IRON;
			return ;
		}
	}
}

void	emulate_woodf(int x, int y, t_data *data)
{
	t_pt	pt;

	if (y > 1 && rand() % 35 == 0 && data->fsim->map[y - 1][x] == MAT_ID_EMPTY)
		data->fsim->map[y - 1][x] = MAT_ID_FIRE;
	if (rand() % 750 == 0)
		data->fsim->map[y][x] = MAT_ID_FIRE;
	if (rand() % 75 == 0)
	{
		pt = find_id(x, y, data, MAT_ID_WOOD);
		if (pt.z == 1)
		{
			data->fsim->map[y][x] = MAT_ID_WOODF;
			return ;
		}
	}
	pt = find_id(x, y, data, MAT_ID_WATER);
	if (pt.z == 1)
	{
		data->fsim->map[y][x] = MAT_ID_WOOD;
		return ;
	}
}

void	emulate_glassf(int x, int y, t_data *data)
{
	int	i;
	int	j;
	t_pt	pt;

	if (rand() % 3 == 0)
		return ;
	if (rand() % 30 == 0)
		data->fsim->map[y][x] = MAT_ID_GLASS;
	i = y + 2;
	while (--i >= y - 1)
	{
		j = x - 2;
		while (++j <= x + 1)
		{
			if (data->fsim->map[i][j] == MAT_ID_SAND)
			{
				if (rand() % 20 == 0)
					data->fsim->map[i][j] = MAT_ID_GLASSF;
				if (rand() % 30 == 0)
				{
					data->fsim->map[y][x] = MAT_ID_GLASS;
					return ;
				}
			}
		}
	}
	pt = find_id(x, y, data, MAT_ID_WATER);
	if (pt.z == 1)
	{
		data->fsim->map[y][x] = MAT_ID_GLASS;
		data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_STEAM;
		return ;
	}
	return ;
}

void	emulate_gunpowder(int x, int y, t_data *data)
{
	t_pt	pt;

	if (rand() % 3 == 0)
	{
		pt = find_id(x, y, data, MAT_ID_FIRE);
		if (pt.z == 1)
		{
			data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_SMOKE;
			if (rand() % 4 == 0)
				data->fsim->map[y][x] = MAT_ID_EMPTY;
			explode(x, y, data, 1, 4, MAT_ID_FIRE, 4, MAT_ID_GUNPOWDER);
			return ;
		}
	}
	if (emulate_solid(x, y, data, 2, 2, 0, 2, 3))
		return ;
}

void	emulate_missile(int x, int y, t_data *data)
{
	if (emulate_solid(x, y, data, 1, -3, 100, 10, 2))
		return ;
	data->fsim->map[y][x] = MAT_ID_EMPTY;
	explode(x, y, data, 100, 50, MAT_ID_FIRE, 6, 'z');
	return ;
}
