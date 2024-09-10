/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_emul_liquid.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 20:09:41 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/10 20:37:04 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/renderer.h"

void	emulate_water(int x, int y, t_data *data, char c)
{
	t_pt	pt;
	static int	iter[WINY][WINX];
	static int	flud[WINY][WINX];
	static int	update[WINY][WINX];

	if (update[y][x] > 0)
	{
		update[y][x] = 0;
		return ;
	}
	if (rand() % 150 == 0)
	{
		pt = find_id(x, y, data, MAT_ID_SAND);
		if (pt.z == 1)
		{
			data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_WETSAND;
			data->fsim->map[y][x] = MAT_ID_EMPTY;
			iter[y][x] = 0;
			return ;
		}
	}
	if (flud[y][x] == 0)
	{
		if (y < WINY - 2)
		{
			if (emulate_fall(x, y, data, 5, 1, 3) == 1)
			{
				iter[y][x] = 0;
				return ;
			}
		}
	}
	flud[y][x] = 0;
	if (data->fsim->map[y][x] == c)
	{
		if (rand() % 2 && (data->fsim->map[y][x - 1] < c && equal_flud(x, y, data, c, 5) == 0))
		{
			data->fsim->map[y][x] = data->fsim->map[y][x - 1];
			data->fsim->map[y][x - 1] = c;
			update[y][x] = 0;
			if (rand() % 5 && iter[y][x] < 5)
			{
				iter[y][x - 1] += iter[y][x] + 1;
				update[y][x - 1] = 0;
				iter[y][x] = 0;
				flud[y][x - 1] = 1;
				emulate_water(x - 1, y, data, c);
			}
			else
				iter[y][x] = 0;
			return ;
		}
		else if (rand() % 2 && data->fsim->map[y][x + 1] < c)
		{
			data->fsim->map[y][x] = data->fsim->map[y][x + 1];
			data->fsim->map[y][x + 1] = c;
			update[y][x + 1] += update[y][x] + 1;
			update[y][x] = 0;
			if (rand() % 5 && iter[y][x] < 5)
			{
				iter[y][x + 1] += iter[y][x] + 1;
				update[y][x + 1] = 0;
				iter[y][x] = 0;
				flud[y][x + 1] = 1;
				emulate_water(x + 1, y, data, c);
			}
			else
				iter[y][x] = 0;
			return ;
		}
	}
}

void	emulate_lava(int x, int y, t_data *data, char c)
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
			data->fsim->map[y][x] = MAT_ID_STONE;
			return ;
		}
	}
	if (rand() % 10 == 0)
	{
		pt = find_id(x, y, data, MAT_ID_SAND);
		if (pt.z == 1)
		{
			data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_GLASSF;
			data->fsim->map[y][x] = MAT_ID_STONE;
			return ;
		}
	}
	if (y > 1 && rand() % 500 == 0 && data->fsim->map[y - 1][x] == MAT_ID_EMPTY)
		data->fsim->map[y - 1][x] = MAT_ID_FIRE;
	if (rand() % 100 == 0)
	{
		pt = find_id(x, y, data, MAT_ID_STONE);
		if (pt.z == 1)
		{
			data->fsim->map[(int)pt.y][(int)pt.x] = c;
			data->fsim->map[y][x] = c;
			return ;
		}
	}
	if (rand() % 3 == 0)
		return ;
	if (y < (int)WINY - 2 && emulate_fall(x, y, data, 1, -2, 2))
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

void	emulate_acid(int x, int y, t_data *data, char c)
{
	int		i;
	int		j;
	static int	iter[WINY][WINX];
	static int	update[WINY][WINX];

	if (update[y][x] > 0)
	{
		update[y][x] = 0;
		return ;
	}
	i = y + 2;
	while (--i >= y - 1)
	{
		j = x - 2;
		while (++j <= x + 1)
		{
			if (data->fsim->map[i][j] != MAT_ID_EMPTY
					&& data->fsim->map[i][j] != MAT_ID_ACID
					&& data->fsim->map[i][j] != MAT_ID_GLASS
					&& data->fsim->map[i][j] != 'z')
			{
				if (rand() % 500 == 0)
					data->fsim->map[i][j] = MAT_ID_EMPTY;
				if (rand() % 300 == 0)
				{
					data->fsim->map[y][x] = MAT_ID_EMPTY;
					iter[y][x] = 0;
					return ;
				}
			}
		}
	}
	if (y < (int)WINY - 2 && emulate_fall(x, y, data, 10, 1, 1) == 1)
	{
		iter[y][x] = 0;
		return ;
	}
	if (data->fsim->map[y][x] == c)
	{
		if (rand() % 2 && data->fsim->map[y][x - 1] < c)
		{
			data->fsim->map[y][x] = data->fsim->map[y][x - 1];
			data->fsim->map[y][x - 1] = c;
			update[y][x] = 0;
			if (rand() % 3 && iter[y][x] < 3)
			{
				iter[y][x - 1] += iter[y][x] + 1;
				update[y][x - 1] = 0;
				iter[y][x] = 0;
				emulate_acid(x - 1, y, data, c);
			}
			else
				iter[y][x] = 0;
			return ;
		}
		if (rand() % 2 && data->fsim->map[y][x + 1] < c)
		{
			data->fsim->map[y][x] = data->fsim->map[y][x + 1];
			data->fsim->map[y][x + 1] = c;
			update[y][x + 1] += update[y][x] + 1;
			update[y][x] = 0;
			if (rand() % 3 && iter[y][x] < 3)
			{
				iter[y][x + 1] += iter[y][x] + 1;
				update[y][x + 1] = 0;
				iter[y][x] = 0;
				emulate_acid(x + 1, y, data, c);
			}
			else
				iter[y][x] = 0;
			return ;
		}
	}
}

void	emulate_oil(int x, int y, t_data *data, char c)
{
	t_pt		pt;
	static int	iter[WINY][WINX];
	static int	update[WINY][WINX];

	if (update[y][x] == 1)
	{
		update[y][x] = 0;
		return ;
	}
	pt = find_id(x, y, data, MAT_ID_FIRE);
	if (pt.z == 1)
	{
		update[y][x] = 0;
		iter[y][x] = 0;
		data->fsim->map[y][x] = MAT_ID_OILF;
		return ;
	}
	pt = find_id(x, y, data, MAT_ID_LAVA);
	if (pt.z == 1)
	{
		update[y][x] = 0;
		iter[y][x] = 0;
		data->fsim->map[y][x] = MAT_ID_OILF;
		return ;
	}
	pt = find_id(x, y, data, MAT_ID_MOLTENIRON);
	if (pt.z == 1)
	{
		update[y][x] = 0;
		iter[y][x] = 0;
		data->fsim->map[y][x] = MAT_ID_OILF;
		return ;
	}
	if (y < (int)WINY - 2 && emulate_fall(x, y, data, 4, 3, 2) == 1)
	{
		update[y][x] = 0;
		iter[y][x] = 0;
		return ;
	}	
	if (data->fsim->map[y][x] == c)
	{
		if (rand() % 2 && data->fsim->map[y][x - 1] < c)
		{
			data->fsim->map[y][x] = data->fsim->map[y][x - 1];
			data->fsim->map[y][x - 1] = c;
			update[y][x] = 0;
			if (rand() % 4 && iter[y][x] < 3)
			{
				iter[y][x - 1] += iter[y][x] + 1;
				update[y][x - 1] = 0;
				iter[y][x] = 0;
				emulate_oil(x - 1, y, data, c);
			}
			else
				iter[y][x] = 0;
			return ;
		}
		else if (data->fsim->map[y][x + 1] < c)
		{
			data->fsim->map[y][x] = data->fsim->map[y][x + 1];
			data->fsim->map[y][x + 1] = c;
			update[y][x + 1] += update[y][x] + 1;
			update[y][x] = 0;
			if (rand() % 4 && iter[y][x] < 3)
			{
				iter[y][x + 1] += iter[y][x] + 1;
				update[y][x + 1] = 0;
				iter[y][x] = 0;
				emulate_oil(x + 1, y, data, c);
			}
			else
				iter[y][x] = 0;
			return ;
		}
	}
}

void	emulate_oilf(int x, int y, t_data *data, char c)
{
	t_pt		pt;
	static int	iter[WINY][WINX];
	static int	update[WINY][WINX];

	if (update[y][x] > 0)
	{
		update[y][x] = 0;
		return ;
	}
	if (y > 1 && rand() % 10 == 0 && data->fsim->map[y - 1][x] == MAT_ID_EMPTY)
		data->fsim->map[y - 1][x] = MAT_ID_FIRE;
	if (rand() % 500 == 0)
		data->fsim->map[y][x] = MAT_ID_FIRE;
	if (rand() % 10 == 0)
	{
		pt = find_id(x, y, data, MAT_ID_OIL);
		if (pt.z == 1)
		{
			data->fsim->map[y][x] = MAT_ID_OILF;
		}
	}
	if (rand() % 500 == 0)
	{
		pt = find_id(x, y, data, MAT_ID_STONE);
		if (pt.z == 1)
		{
			data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_LAVA;
		}
	}
	if (rand() % 100 == 0)
	{
		pt = find_id(x, y, data, MAT_ID_SAND);
		if (pt.z == 1)
		{
			update[y][x] = 0;
			iter[y][x] = 0;
			data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_GLASSF;
			data->fsim->map[y][x] = MAT_ID_FIRE;
			return ;
		}
	}
	if (rand() % 300 == 0)
	{
		pt = find_id(x, y, data, MAT_ID_WATER);
		if (pt.z == 1)
		{
			data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_STEAM;
		}
	}
	if (y < (int)WINY - 2 && emulate_fall(x, y, data, 4, 4, 1) == 1)
	{
		update[y][x] = 0;
		iter[y][x] = 0;
		return ;
	}	
	if (data->fsim->map[y][x] == c)
	{
		if (rand() % 2 && data->fsim->map[y][x - 1] < c)
		{
			data->fsim->map[y][x] = data->fsim->map[y][x - 1];
			data->fsim->map[y][x - 1] = c;
			update[y][x] = 0;
			if (rand() % 4 && iter[y][x] < 3)
			{
				iter[y][x - 1] += iter[y][x] + 1;
				update[y][x - 1] = 0;
				iter[y][x] = 0;
				emulate_oilf(x - 1, y, data, c);
			}
			else
				iter[y][x] = 0;
			return ;
		}
		if (rand() % 2 && data->fsim->map[y][x + 1] < c)
		{
			data->fsim->map[y][x] = data->fsim->map[y][x + 1];
			data->fsim->map[y][x + 1] = c;
			update[y][x + 1] += update[y][x] + 1;
			update[y][x] = 0;
			if (rand() % 4 && iter[y][x] < 3)
			{
				iter[y][x + 1] += iter[y][x] + 1;
				update[y][x + 1] = 0;
				iter[y][x] = 0;
				emulate_oilf(x + 1, y, data, c);
			}
			else
				iter[y][x] = 0;
			return ;
		}
	}
}
