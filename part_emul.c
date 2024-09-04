/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_emul.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:28:33 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/04 10:45:06 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/renderer.h"

int	emulate_propane(int x, int y, t_data *data)
{
	char	swap;
	static int iter = 0;
	t_pt	pt;

	if (rand() % 3 == 0)
		return (1);
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
			data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_FIRE;
		return (1);
	}
	if (data->fsim->map[y - 1][x] < data->fsim->map[y][x] && rand() % 2)
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
		if (rand() % 3 && iter < 3)
		{
			iter++;
			emulate_sand(x, y - 1, data, 2);
		}
		else
		{
			iter = 0;
			return (1);
		}
	}	
	else if (data->fsim->map[y - 1][x - 1] < data->fsim->map[y][x] && rand() % 2)
	{
		swap = data->fsim->map[y - 1][x - 1];
		data->fsim->map[y - 1][x - 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		if (rand() % 3 && iter < 3)
		{
			iter++;
			emulate_sand(x - 1, y - 1, data, 2);
		}
		else
		{
			iter = 0;
			return (1);
		}
	}	
	else if (data->fsim->map[y - 1][x + 1] < data->fsim->map[y][x])
	{
		swap = data->fsim->map[y - 1][x + 1];
		data->fsim->map[y - 1][x + 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		if (rand() % 3 && iter < 3)
		{
			iter++;
			emulate_sand(x + 1, y - 1, data, 2);
		}
		else
		{
			iter = 0;
			return (1);
		}
	}
	return (0);
}

int	emulate_fire(int x, int y, t_data *data)
{
	char	swap;
	static int iter = 0;
	t_pt	pt;

	if (rand() % 2 == 0)
		return (1);
	if (rand() % 100 == 0)
	{
		data->fsim->map[y][x] = MAT_ID_EMPTY;
		return (1);
	}
	pt = find_id(x, y, data, MAT_ID_FIRE);
	if (pt.z == 1)
	{
		if (rand() % 2 == 0)
			return (1);
	}
	pt = find_id(x, y, data, MAT_ID_WATER);
	if (pt.z == 1)
	{
		data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_STEAM;
		data->fsim->map[y][x] = MAT_ID_EMPTY;
		return (1);
	}
	pt = find_id(x, y, data, MAT_ID_STONE);
	if (pt.z == 1 && rand() % 100 == 0)
	{
		data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_LAVA;
		data->fsim->map[y][x] = MAT_ID_EMPTY;
		return (1);
	}
	if (data->fsim->map[y - 1][x] < data->fsim->map[y][x] && rand() % 2)
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
		if (rand() % 2 && iter < 3)
		{
			iter++;
			emulate_sand(x, y - 1, data, 2);
		}
		else
		{
			iter = 0;
			return (1);
		}
	}	
	else if (data->fsim->map[y - 1][x - 1] < data->fsim->map[y][x] && rand() % 2)
	{
		swap = data->fsim->map[y - 1][x - 1];
		data->fsim->map[y - 1][x - 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		if (rand() % 2 && iter < 3)
		{
			iter++;
			emulate_sand(x - 1, y - 1, data, 2);
		}
		else
		{
			iter = 0;
			return (1);
		}
	}	
	else if (data->fsim->map[y - 1][x + 1] < data->fsim->map[y][x])
	{
		swap = data->fsim->map[y - 1][x + 1];
		data->fsim->map[y - 1][x + 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		if (rand() % 2 && iter < 3)
		{
			iter++;
			emulate_sand(x + 1, y - 1, data, 2);
		}
		else
		{
			iter = 0;
			return (1);
		}
	}
	return (0);
}

int	emulate_gas(int x, int y, t_data *data)
{
	char	swap;
	static int iter = 0;

	if (rand() % 2 == 0)
		return (1);
	if (data->fsim->map[y - 1][x] < data->fsim->map[y][x] && rand() % 2)
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
		if (rand() % 2 && iter < 3)
		{
			iter++;
			emulate_gas(x, y - 1, data);
		}
		else
		{
			iter = 0;
			return (1);
		}
	}	
	else if (data->fsim->map[y - 1][x - 1] < data->fsim->map[y][x] && rand() % 2)
	{
		swap = data->fsim->map[y - 1][x - 1];
		data->fsim->map[y - 1][x - 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		if (rand() % 2 && iter < 3)
		{
			iter++;
			emulate_gas(x - 1, y - 1, data);
		}
		else
		{
			iter = 0;
			return (1);
		}
	}	
	else if (data->fsim->map[y - 1][x + 1] < data->fsim->map[y][x])
	{
		swap = data->fsim->map[y - 1][x + 1];
		data->fsim->map[y - 1][x + 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		if (rand() % 2 && iter < 3)
		{
			iter++;
			emulate_gas(x + 1, y - 1, data);
		}
		else
		{
			iter = 0;
			return (1);
		}
	}	
	return (0);
}

int	emulate_sand(int x, int y, t_data *data, int randed)
{
	char	swap;
	static int iter = 0;
	static int	equal = 0;

	if (data->fsim->map[y + 1][x] < data->fsim->map[y][x] && rand() % randed == 0)
	{
		if (data->fsim->map[y + 1][x - 1] < data->fsim->map[y + 1][x])
		{
			swap = data->fsim->map[y + 1][x - 1];
			data->fsim->map[y + 1][x - 1] = data->fsim->map[y + 1][x];
			data->fsim->map[y + 1][x] = swap;
		}
		else if (data->fsim->map[y + 1][x + 1] < data->fsim->map[y + 1][x])
		{
			swap = data->fsim->map[y + 1][x + 1];
			data->fsim->map[y + 1][x + 1] = data->fsim->map[y + 1][x];
			data->fsim->map[y + 1][x] = swap;
		}
		swap = data->fsim->map[y + 1][x];
		data->fsim->map[y + 1][x] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		if (rand() % 2 && iter < 3)
		{
			iter++;
			emulate_sand(x, y + 1, data, randed);
		}
		else
		{
			iter = 0;
			return (1);
		}
		return (1);
	}
	if (data->fsim->map[y + 1][x - 1] < data->fsim->map[y][x] && equal == 0)
	{
		swap = data->fsim->map[y + 1][x - 1];
		data->fsim->map[y + 1][x - 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		equal = 1;
		if (rand() % 2 && iter < 3)
		{
			iter++;
			emulate_sand(x, y + 1, data, randed);
		}
		else
		{
			iter = 0;
			return (1);
		}
		return (1);
	}
	else if (data->fsim->map[y + 1][x + 1] < data->fsim->map[y][x] && equal == 1)
	{
		swap = data->fsim->map[y + 1][x + 1];
		data->fsim->map[y + 1][x + 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		equal = 0;
		if (rand() % 2 && iter < 3)
		{
			iter++;
			emulate_sand(x, y + 1, data, randed);
		}
		else
		{
			iter = 0;
			return (1);
		}
		return (1);
	}
	return (0);
}

void	emulate_water(int x, int y, t_data *data, char c)
{
	static int iter = 0;

	if (y < (int)WINY - 2 && emulate_sand(x, y, data, 10) == 1)
	{
		return ;
	}	
	if (data->fsim->map[y][x - 1] < c && rand() % 2 && data->fsim->map[y][x] == c)
	{
		data->fsim->map[y][x] = data->fsim->map[y][x - 1];
		data->fsim->map[y][x - 1] = c;
		if (rand() % 3 && iter < 3)
		{
			iter++;
			emulate_water(x - 1, y, data, c);
		}
		else
		{
			iter = 0;
			return ;
		}
		return ;
	}
	if (data->fsim->map[y][x + 1] < c && rand() % 2 && data->fsim->map[y][x] == c)
	{
		data->fsim->map[y][x] = data->fsim->map[y][x + 1];
		data->fsim->map[y][x + 1] = c;
		if (rand() % 3 && iter < 3)
		{
			iter++;
			emulate_water(x + 1, y, data, c);
		}
		else
		{
			iter = 0;
			return ;
		}
		return ;
	}
}

void	emulate_steam(int x, int y, t_data *data, char c)
{
	if (y > 1 && emulate_gas(x, y, data) == 1)
	{
		return ;
	}	
	else if (data->fsim->map[y][x - 1] < c && rand() % 2 && data->fsim->map[y][x] == c)
	{
		data->fsim->map[y][x] = data->fsim->map[y][x - 1];
		data->fsim->map[y][x - 1] = c;
		if (rand() % 1000 == 0)
			data->fsim->map[y][x - 1] = MAT_ID_WATER;
		return ;
	}
	else if (data->fsim->map[y][x + 1] < c && data->fsim->map[y][x] == c)
	{
		data->fsim->map[y][x] = data->fsim->map[y][x + 1];
		data->fsim->map[y][x + 1] = c;
		if (rand() % 1000 == 0)
			data->fsim->map[y][x + 1] = MAT_ID_WATER;
		return ;
	}
	else if (rand() % 1000 == 0 && data->fsim->map[y][x] == c)
	{
		data->fsim->map[y][x] = MAT_ID_WATER;
		return ;
	}
}

void	emulate_lava(int x, int y, t_data *data, char c)
{
	t_pt	pt;

	pt = find_id(x, y, data, MAT_ID_WATER);
	if (pt.z == 1)
	{
		data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_STEAM;
		data->fsim->map[y][x] = MAT_ID_STONE;
		return ;
	}
	if (y > 1 && rand() % 500 == 0 && data->fsim->map[y - 1][x] == MAT_ID_EMPTY)
		data->fsim->map[y - 1][x] = MAT_ID_FIRE;
	if (y < (int)WINY - 2 && emulate_sand(x, y, data, 1))
	{
		return ;
	}	
	else if (data->fsim->map[y][x - 1] < c && rand() % 2 && data->fsim->map[y][x] == c)
	{
		data->fsim->map[y][x] = data->fsim->map[y][x - 1];
		data->fsim->map[y][x - 1] = c;
	}
	else if (data->fsim->map[y][x + 1] < c && data->fsim->map[y][x] == c)
	{
		data->fsim->map[y][x] = data->fsim->map[y][x + 1];
		data->fsim->map[y][x + 1] = c;
	}
	else if (data->fsim->map[y][x] == c)
	{
		pt = find_id(x, y, data, MAT_ID_STONE);
		if (pt.z == 1 && rand() % 100 == 0)
		{
			data->fsim->map[(int)pt.y][(int)pt.x] = c;
			data->fsim->map[y][x] = c;
			return ;
		}
	}
}

void	emulate_acid(int x, int y, t_data *data, char c)
{
	static int	iter = 0;
	int	i;
	int	j;

	i = y + 2;
	while (--i >= y - 1)
	{
		j = x - 2;
		while (++j <= x + 1)
		{
			if (data->fsim->map[i][j] != MAT_ID_EMPTY
				&& data->fsim->map[i][j] != MAT_ID_ACID
				&& data->fsim->map[i][j] != 'G')
			{
				if (rand() % 500 == 0)
					data->fsim->map[i][j] = MAT_ID_EMPTY;
				if (rand() % 300 == 0)
				{
					data->fsim->map[y][x] = MAT_ID_EMPTY;
					return ;
				}
			}
		}
	}
	if (y < (int)WINY - 2 && emulate_sand(x, y, data, 10) == 1)
	{
		return ;
	}
	else if (data->fsim->map[y][x - 1] < c && rand() % 2 && data->fsim->map[y][x] == c)
	{
		data->fsim->map[y][x] = data->fsim->map[y][x - 1];
		data->fsim->map[y][x - 1] = c;
		if (rand() % 3 && iter < 3)
		{
			iter++;
			emulate_acid(x - 1, y, data, c);
		}
		else
		{
			iter = 0;
			return ;
		}
	}
	else if (data->fsim->map[y][x + 1] < c && data->fsim->map[y][x] == c)
	{
		data->fsim->map[y][x] = data->fsim->map[y][x + 1];
		data->fsim->map[y][x + 1] = c;
		if (rand() % 3 && iter < 3)
		{
			iter++;
			emulate_acid(x + 1, y, data, c);
		}
		else
		{
			iter = 0;
			return ;
		}
	}
}
