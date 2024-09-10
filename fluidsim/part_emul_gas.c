/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_emul_gas.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 20:13:35 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/10 20:36:55 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/renderer.h"

void	emulate_steam(int x, int y, t_data *data, char c)
{
	t_pt		pt;
	static int	time;
	static int	update[WINY][WINX];

	if (update[y][x] > 0)
	{
		update[y][x] = 0;
		return ;
	}
	if (y > 1 && emulate_gas(x, y, data) == 1)
		return ;
	if (rand() % 1000 == 0 && data->fsim->map[y][x] == c)
	{
		data->fsim->map[y][x] = MAT_ID_WATER;
		return ;
	}
	if (time >= 3000000)
		data->fsim->map[y][x] = MAT_ID_FOG;
	else
		time++;
	pt = find_id(x, y, data, MAT_ID_FOG);
	if (pt.z == 1)
		data->fsim->map[y][x] = MAT_ID_FOG;
	if (pt.z == 1)
	{
		time = 0;
		pt = find_id(x, y, data, MAT_ID_STEAM);
		if (pt.z == 1)
			emulate_steam((int)pt.x, (int)pt.y, data, c);
		return ;
	}
	if (data->fsim->map[y][x] == c)
	{
		if (rand() % 2 && (data->fsim->map[y][x - 1] < c && equal_flud(x, y, data, c, 5) == 0))
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

int	emulate_fog(int x, int y, t_data *data)
{
	t_pt		pt;
	char		swap;
	static int	iter[WINY][WINX];
	static int	update[WINY][WINX];

	if (update[y][x] > 0)
	{
		update[y][x] = 0;
		return (1);
	}
	if (rand() % 100 == 0)
	{
		pt = find_id(x, y, data, MAT_ID_WATER);
		if (pt.z == 1)
		{
			data->fsim->map[y][x] = MAT_ID_WATER;
			return (1);
		}
	}
	if (rand() % 3)
	{
		pt = find_around_id(x, y, data, MAT_ID_FOG, 1);
		if (pt.z == 0)
			return (1);
	}
	if (rand() % 4 == 0 && data->fsim->map[y + 1][x] < data->fsim->map[y][x])
	{
		swap = data->fsim->map[y + 1][x];
		data->fsim->map[y + 1][x] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		update[y][x] = 0;
		if (rand() % 3 && iter[y][x] < 3)
		{
			iter[y + 1][x] += iter[y][x] + 1;
			update[y + 1][x] = 0;
			iter[y][x] = 0;
			emulate_fall(x, y + 1, data, 2, 2, 0);
		}
		else
			iter[y][x] = 0;
		return (1);
	}
	if (rand() % 2 && data->fsim->map[y - 1][x] < data->fsim->map[y][x])
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
		if (rand() % 3 && iter[y][x] < 3)
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
		if (rand() % 3 && iter[y][x] < 3)
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
		if (rand() % 3 && iter[y][x] < 3)
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

int	emulate_fire(int x, int y, t_data *data)
{
	t_pt		pt;
	char		swap;
	static int	iter[WINY][WINX];
	static int	update[WINY][WINX];

	if (update[y][x] > 0)
	{
		update[y][x] = 0;
		return (1);
	}
	if (rand() % 100 == 0)
	{
		data->fsim->map[y][x] = MAT_ID_SMOKE;
		return (1);
	}
	if (rand() % 4 == 0)
	{
		pt = find_id(x, y, data, MAT_ID_FIRE);
		if (pt.z == 1)
			return (1);
	}
	pt = find_id(x, y, data, MAT_ID_WATER);
	if (pt.z == 1)
	{
		data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_STEAM;
		data->fsim->map[y][x] = MAT_ID_SMOKE;
		return (1);
	}
	if (rand() % 100 == 0)
	{
		pt = find_id(x, y, data, MAT_ID_STONE);
		if (pt.z == 1)
		{
			data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_LAVA;
			data->fsim->map[y][x] = MAT_ID_SMOKE;
			return (1);
		}
	}
	if (rand() % 100 == 0)
	{
		pt = find_id(x, y, data, MAT_ID_SAND);
		if (pt.z == 1)
		{
			data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_GLASSF;
			data->fsim->map[y][x] = MAT_ID_SMOKE;
			return (1);
		}
	}
	if (rand() % 4 && data->fsim->map[y - 1][x] < data->fsim->map[y][x])
	{
		swap = data->fsim->map[y - 1][x];
		data->fsim->map[y - 1][x] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		update[y - 1][x] += update[y][x] + 1;
		update[y][x] = 0;
		if (rand() % 4 && iter[y][x] < 3)
		{
			iter[y + 1][x] += iter[y][x] + 1;
			update[y - 1][x] = 0;
			iter[y][x] = 0;
			emulate_gas(x, y - 1, data);
		}
		else
			iter[y][x] = 0;
		return (1);
	}
	if (rand() % 5 == 0 && data->fsim->map[y + 1][x] < data->fsim->map[y][x])
	{
		if (data->fsim->map[y + 1][x - 1] == MAT_ID_EMPTY)
		{
			swap = data->fsim->map[y + 1][x - 1];
			data->fsim->map[y + 1][x - 1] = data->fsim->map[y + 1][x];
			data->fsim->map[y + 1][x] = swap;
		}
		else if (data->fsim->map[y + 1][x + 1] == MAT_ID_EMPTY)
		{
			swap = data->fsim->map[y + 1][x + 1];
			data->fsim->map[y + 1][x + 1] = data->fsim->map[y + 1][x];
			data->fsim->map[y + 1][x] = swap;
		}
		swap = data->fsim->map[y + 1][x];
		data->fsim->map[y + 1][x] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		update[y][x] = 0;
		if (rand() % 3 && iter[y][x] < 3)
		{
			iter[y + 1][x] += iter[y][x] + 1;
			update[y + 1][x] = 0;
			iter[y][x] = 0;
			emulate_gas(x, y + 1, data);
		}
		else
			iter[y][x] = 0;
		return (1);
	}	
	if (rand() % 2 && data->fsim->map[y + 1][x - 1] < data->fsim->map[y][x])
	{
		swap = data->fsim->map[y + 1][x - 1];
		data->fsim->map[y + 1][x - 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		update[y][x] = 0;
		if (rand() % 2 && iter[y][x] < 1)
		{
			iter[y + 1][x - 1] += iter[y][x] + 1;
			update[y + 1][x - 1] = 0;
			iter[y][x] = 0;
			emulate_gas(x - 1, y + 1, data);
		}
		else
			iter[y][x] = 0;
		return (1);
	}	
	if (rand() % 2 && data->fsim->map[y + 1][x + 1] < data->fsim->map[y][x])
	{
		swap = data->fsim->map[y + 1][x + 1];
		data->fsim->map[y + 1][x + 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		update[y][x] = 0;
		if (rand() % 2 && iter[y][x] < 1)
		{
			iter[y + 1][x + 1] += iter[y][x] + 1;
			update[y + 1][x + 1] = 0;
			iter[y][x] = 0;
			emulate_gas(x + 1, y + 1, data);
		}
		else
			iter[y][x] = 0;
		return (1);
	}
	return (0);
}

int	emulate_smoke(int x, int y, t_data *data)
{
	t_pt		pt;
	char		swap;
	static int	iter[WINY][WINX];
	static int	update[WINY][WINX];

	if (update[y][x] > 0)
	{
		update[y][x] = 0;
		return (1);
	}
	if (rand() % 2)
	{
		pt = find_around_id(x, y, data, MAT_ID_SMOKE, 1);
		if (pt.z == 1)
			return (1);
	}
	if (rand() % 4 == 0)
		return (1);
	if (rand() % 150 == 0)
	{
		data->fsim->map[y][x] = MAT_ID_EMPTY;
		return (1);
	}
	if (rand() % 4 && data->fsim->map[y - 1][x] < data->fsim->map[y][x])
	{
		swap = data->fsim->map[y - 1][x];
		data->fsim->map[y - 1][x] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		update[y - 1][x] += update[y][x] + 1;
		update[y][x] = 0;
		if (rand() % 4 && iter[y][x] < 3)
		{
			iter[y + 1][x] += iter[y][x] + 1;
			update[y - 1][x] = 0;
			iter[y][x] = 0;
			emulate_gas(x, y - 1, data);
		}
		else
			iter[y][x] = 0;
		return (1);
	}
	if (rand() % 2 && data->fsim->map[y + 1][x] < data->fsim->map[y][x])
	{
		if (data->fsim->map[y + 1][x - 1] == MAT_ID_EMPTY)
		{
			swap = data->fsim->map[y + 1][x - 1];
			data->fsim->map[y + 1][x - 1] = data->fsim->map[y + 1][x];
			data->fsim->map[y + 1][x] = swap;
		}
		else if (data->fsim->map[y + 1][x + 1] == MAT_ID_EMPTY)
		{
			swap = data->fsim->map[y + 1][x + 1];
			data->fsim->map[y + 1][x + 1] = data->fsim->map[y + 1][x];
			data->fsim->map[y + 1][x] = swap;
		}
		swap = data->fsim->map[y + 1][x];
		data->fsim->map[y + 1][x] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		update[y][x] = 0;
		if (rand() % 3 && iter[y][x] < 3)
		{
			iter[y + 1][x] += iter[y][x] + 1;
			update[y + 1][x] = 0;
			iter[y][x] = 0;
			emulate_gas(x, y + 1, data);
		}
		else
			iter[y][x] = 0;
		return (1);
	}	
	if (rand() % 2 && data->fsim->map[y + 1][x - 1] < data->fsim->map[y][x])
	{
		swap = data->fsim->map[y + 1][x - 1];
		data->fsim->map[y + 1][x - 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		update[y][x] = 0;
		if (rand() % 3 && iter[y][x] < 3)
		{
			iter[y + 1][x - 1] += iter[y][x] + 1;
			update[y + 1][x - 1] = 0;
			iter[y][x] = 0;
			emulate_gas(x - 1, y + 1, data);
		}
		else
			iter[y][x] = 0;
		return (1);
	}	
	if (rand() % 2 && data->fsim->map[y + 1][x + 1] < data->fsim->map[y][x])
	{
		swap = data->fsim->map[y + 1][x + 1];
		data->fsim->map[y + 1][x + 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		update[y][x] = 0;
		if (rand() % 3 && iter[y][x] < 3)
		{
			iter[y + 1][x + 1] += iter[y][x] + 1;
			update[y + 1][x + 1] = 0;
			iter[y][x] = 0;
			emulate_gas(x + 1, y + 1, data);
		}
		else
			iter[y][x] = 0;
		return (1);
	}
	return (0);
}

int	emulate_propane(int x, int y, t_data *data)
{
	t_pt		pt;
	char		swap;
	static int	iter[WINY][WINX];
	static int	update[WINY][WINX];

	if (update[y][x] > 0)
	{
		update[y][x] = 0;
		return (1);
	}
	pt = find_id(x, y, data, MAT_ID_LAVA);
	if (pt.z == 1)
		data->fsim->map[y][x] = MAT_ID_FIRE;
	pt = find_id(x, y, data, MAT_ID_FIRE);
	if (pt.z == 1)
		data->fsim->map[y][x] = MAT_ID_FIRE;
	if (pt.z == 1)
	{
		pt = find_id(x, y, data, MAT_ID_PROPANE);
		if (pt.z == 1)
			emulate_propane((int)pt.x, (int)pt.y, data);
		return (1);
	}
	if (rand() % 3 == 0)
		return (1);
	if (rand() % 2 && data->fsim->map[y - 1][x] < data->fsim->map[y][x])
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
		update[y - 1][x - 1] += update[y][x] + 1;
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

int	emulate_oxygen(int x, int y, t_data *data)
{
	t_pt		pt;
	char		swap;
	static int	iter[WINY][WINX];
	static int	update[WINY][WINX];

	if (update[y][x] > 0)
	{
		update[y][x] = 0;
		return (1);
	}
	pt = find_id(x, y, data, MAT_ID_LAVA);
	if (pt.z == 1)
		data->fsim->map[y][x] = MAT_ID_FIRE;
	pt = find_id(x, y, data, MAT_ID_FIRE);
	if (pt.z == 1)
		data->fsim->map[y][x] = MAT_ID_FIRE;
	if (pt.z == 1)
	{
		pt = find_id(x, y, data, MAT_ID_OXYGEN);
		if (pt.z == 1)
			emulate_oxygen((int)pt.x, (int)pt.y, data);
		return (1);
	}
	if (rand() % 100 == 0)
	{
		pt = find_id(x, y, data, MAT_ID_HIDROGEN);
		if (pt.z == 1)
		{
			iter[y][x] = 0;
			update[y][x] = 0;
			data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_EMPTY;
			data->fsim->map[y][x] = MAT_ID_WATER;
			return (1);
		}
	}
	if (rand() % 3)
	{
		pt = find_around_id(x, y, data, MAT_ID_OXYGEN, 1);
		iter[y][x] = 0;
		if (pt.z == 0)
			return (1);
	}
	if (rand() % 20 == 0 && data->fsim->map[y + 1][x] < data->fsim->map[y][x])
	{
		swap = data->fsim->map[y + 1][x];
		data->fsim->map[y + 1][x] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		update[y][x] = 0;
		if (rand() % 3 && iter[y][x] < 3)
		{
			iter[y + 1][x] += iter[y][x] + 1;
			update[y + 1][x] = 0;
			iter[y][x] = 0;
			emulate_fall(x, y + 1, data, 2, 2, 0);
		}
		else
			iter[y][x] = 0;
		return (1);
	}
	if (rand() % 2 && data->fsim->map[y - 1][x] < data->fsim->map[y][x])
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
		if (rand() % 3 && iter[y][x] < 3)
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
		if (rand() % 3 && iter[y][x] < 3)
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
		if (rand() % 3 && iter[y][x] < 3)
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

int	emulate_hidrogen(int x, int y, t_data *data)
{
	t_pt		pt;
	char		swap;
	static int	iter[WINY][WINX];
	static int	update[WINY][WINX];

	if (update[y][x] > 0)
	{
		update[y][x] = 0;
		return (1);
	}
	pt = find_id(x, y, data, MAT_ID_LAVA);
	if (pt.z == 1)
		data->fsim->map[y][x] = MAT_ID_FIRE;
	pt = find_id(x, y, data, MAT_ID_FIRE);
	if (pt.z == 1)
		data->fsim->map[y][x] = MAT_ID_FIRE;
	if (pt.z == 1)
	{
		pt = find_id(x, y, data, MAT_ID_HIDROGEN);
		iter[y][x] = 0;
		update[y][x] = 0;
		if (pt.z == 1)
			emulate_hidrogen((int)pt.x, (int)pt.y, data);
		return (1);
	}
	pt = find_around_id(x, y, data, MAT_ID_HIDROGEN, 1);
	if (pt.z == 0)
	{
		iter[y][x] = 0;
		if (rand() % 3)
			return (1);
	}
	if (rand() % 20 == 0 && data->fsim->map[y + 1][x] < data->fsim->map[y][x])
	{
		swap = data->fsim->map[y + 1][x];
		data->fsim->map[y + 1][x] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		update[y][x] = 0;
		if (rand() % 3 && iter[y][x] < 3)
		{
			iter[y + 1][x] += iter[y][x] + 1;
			update[y + 1][x] = 0;
			iter[y][x] = 0;
			emulate_fall(x, y + 1, data, 2, 2, 0);
		}
		else
			iter[y][x] = 0;
		return (1);
	}
	if (rand() % 2 && data->fsim->map[y - 1][x] < data->fsim->map[y][x])
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
		if (rand() % 3 && iter[y][x] < 3)
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
		if (rand() % 3 && iter[y][x] < 3)
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
		if (rand() % 3 && iter[y][x] < 3)
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
