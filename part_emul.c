/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_emul.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:28:33 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/06 17:52:26 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/renderer.h"

void	push_inertia(int x, int y, t_data *data, int force)
{
	int	i;
	int	l;
	int	r;
	int	tmp;
	int	emp;
	
	i = x;
	r = 0;
	emp = 0;
	tmp = force + 1;
	while (--tmp > 0)
	{
		if (i < WINX)
			if (data->fsim->map[y][i] == data->fsim->map[y - 1][x])
				r++;
		if (data->fsim->map[y][i] > data->fsim->map[y - 1][x])
			break ;
		if (data->fsim->map[y][i] == MAT_ID_EMPTY)
		{
			emp = 1;
			break ;
		}
		i++;
	}
	r *= emp;
	i = x;
	l = 0;
	emp = 0;
	tmp = force + 1;
	while (--tmp > 0)
	{
		if (i > 0)
			if (data->fsim->map[y][i] == data->fsim->map[y - 1][x])
				l++;
		if (data->fsim->map[y][i] > data->fsim->map[y - 1][x])
			break ;
		if (data->fsim->map[y][i] == MAT_ID_EMPTY)
		{
			emp = 1;
			break ;
		}
		i--;
	}
	l *= emp;
	if (l == 0 && r == 0)
		return ;
	if (l == r)
	{
		if (rand() % 2)
			r -= 1;
		else
			l -= 1;
	}
	while (l > 0)
	{
		tmp = data->fsim->map[y][x - l];
		data->fsim->map[y][x - l] = data->fsim->map[y][x - l + 1];
		tmp = data->fsim->map[y][x - l + 1] = tmp;
		l--;
	}
	while (r > 0)
	{
		tmp = data->fsim->map[y][x + r];
		data->fsim->map[y][x + r] = data->fsim->map[y][x + r - 1];
		tmp = data->fsim->map[y][x + r - 1] = tmp;
		r--;
	}
}

int	equal_flud(int x, int y, t_data *data, char c)
{
	int	i;
	int	l;
	int	r;

	i = x - 5;
	l = 0;
	while (i < x)
	{
		if (i > 0)
			if (data->fsim->map[y][i] == c)
				l++;
		i++;
	}
	i = x + 1;
	r = 0;
	while (i < x + 6)
	{
		if (i < WINX)
			if (data->fsim->map[y][i] == c)
				r++;
		i++;
	}
	if (l == r)
		return (rand() % 2);
	if (l < r)
		return (0);
	return (1);
}

int	slide_sand(int x, int y, t_data *data, int direc)
{
	char	swap;

	if (data->fsim->map[y][x - 1] == MAT_ID_EMPTY && direc == 0)
	{
		swap = data->fsim->map[y][x - 1];
		data->fsim->map[y][x - 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		return (-1);
	}
	else if (data->fsim->map[y][x + 1] == MAT_ID_EMPTY)
	{
		swap = data->fsim->map[y][x + 1];
		data->fsim->map[y][x + 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		return (1);
	}
	return (0);
}

int	emulate_propane(int x, int y, t_data *data)
{
	char	swap;
	static int iter[WINY][WINX];
	t_pt	pt;

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
		if (rand() % 4 && iter[y][x] < 3)
		{
			iter[y - 1][x] += iter[y][x] + 1;
			iter[y][x] = 0;
			emulate_sand(x, y - 1, data, 2, 2, 0);
		}
		else
		{
			iter[y][x] = 0;
			return (1);
		}
	}	
	else if (data->fsim->map[y - 1][x - 1] < data->fsim->map[y][x] && rand() % 3)
	{
		swap = data->fsim->map[y - 1][x - 1];
		data->fsim->map[y - 1][x - 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		if (rand() % 4 && iter[y][x] < 3)
		{
			iter[y - 1][x - 1] += iter[y][x] + 1;
			iter[y][x] = 0;
			emulate_sand(x - 1, y - 1, data, 2, 2, 0);
		}
		else
		{
			iter[y][x] = 0;
			return (1);
		}
	}	
	else if (data->fsim->map[y - 1][x + 1] < data->fsim->map[y][x])
	{
		swap = data->fsim->map[y - 1][x + 1];
		data->fsim->map[y - 1][x + 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		if (rand() % 4 && iter[y][x] < 3)
		{
			iter[y - 1][x + 1] += iter[y][x] + 1;
			iter[y][x] = 0;
			emulate_sand(x + 1, y - 1, data, 2, 2, 0);
		}
		else
		{
			iter[y][x] = 0;
			return (1);
		}
	}
	return (0);
}

int	emulate_fire(int x, int y, t_data *data)
{
	char	swap;
	static int iter[WINX][WINX];
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
		if (rand() % 2 && iter[y][x] < 3)
		{
			iter[y - 1][x] += iter[y][x] + 1;
			iter[y][x] = 0;
			emulate_sand(x, y - 1, data, 2, -3, 0);
		}
		else
		{
			iter[y][x] = 0;
			return (1);
		}
	}	
	else if (data->fsim->map[y - 1][x - 1] < data->fsim->map[y][x] && rand() % 2)
	{
		swap = data->fsim->map[y - 1][x - 1];
		data->fsim->map[y - 1][x - 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		if (rand() % 2 && iter[y][x] < 3)
		{
			iter[y - 1][x - 1] += iter[y][x] + 1;
			iter[y][x] = 0;
			emulate_sand(x - 1, y - 1, data, 2, -3, 0);
		}
		else
		{
			iter[y][x] = 0;
			return (1);
		}
	}	
	else if (data->fsim->map[y - 1][x + 1] < data->fsim->map[y][x])
	{
		swap = data->fsim->map[y - 1][x + 1];
		data->fsim->map[y - 1][x + 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		if (rand() % 2 && iter[y][x] < 3)
		{
			iter[y - 1][x + 1] += iter[y][x] + 1;
			iter[y][x] = 0;
			emulate_sand(x + 1, y - 1, data, 2, -3, 0);
		}
		else
		{
			iter[y][x] = 0;
			return (1);
		}
	}
	return (0);
}

int	emulate_gas(int x, int y, t_data *data)
{
	char	swap;
	static int iter[WINY][WINX];
	static int equal = 0;

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
		if (rand() % 2 && iter[y][x] < 3)
		{
			iter[y - 1][x] += iter[y][x] + 1;
			iter[y][x] = 0;
			emulate_gas(x, y - 1, data);
		}
		else
		{
			iter[y][x] = 0;
			return (1);
		}
	}	
	else if (data->fsim->map[y - 1][x - 1] < data->fsim->map[y][x] && equal == 1)
	{
		swap = data->fsim->map[y - 1][x - 1];
		data->fsim->map[y - 1][x - 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		equal = 0;
		if (rand() % 2 && iter[y][x] < 3)
		{
			iter[y - 1][x - 1] += iter[y][x] + 1;
			iter[y][x] = 0;
			emulate_gas(x - 1, y - 1, data);
		}
		else
		{
			iter[y][x] = 0;
			return (1);
		}
	}	
	else if (data->fsim->map[y - 1][x + 1] < data->fsim->map[y][x] && equal == 0)
	{
		swap = data->fsim->map[y - 1][x + 1];
		data->fsim->map[y - 1][x + 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		equal = 1;
		if (rand() % 2 && iter[y][x] < 3)
		{
			iter[y - 1][x + 1] += iter[y][x] + 1;
			iter[y][x] = 0;
			emulate_gas(x + 1, y - 1, data);
		}
		else
		{
			iter[y][x] = 0;
			return (1);
		}
	}	
	if (equal == 0)
		equal = 1;
	else
		equal = 0;
	return (0);
}

int	emulate_sand(int x, int y, t_data *data, int randed, int slide, int force)
{
	char	swap;
	static int iter[WINY][WINX];
	static int	equal = 0;
	int		i;
	int		direc;

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
		if (rand() % 2 && iter[y][x] < 3)
		{
			iter[y + 1][x] += iter[y][x] + 1;
			iter[y][x] = 0;
			emulate_sand(x, y + 1, data, randed, slide, force);
		}
		else
		{
			iter[y][x] = 0;
			return (1);
		}
		return (1);
	}
	while (iter[y][x] > 0 && (data->fsim->map[y + 1][x] >= data->fsim->map[y][x]))
	{
		if (rand() % 3)
			push_inertia(x, y + 1, data, force);
		i = iter[y][x] + slide;
		iter[y][x] = 0;
		direc = equal_flud(x, y, data, data->fsim->map[y][x]);
		if (i <= 0)
			break ;
		while (i > 0)
		{
			if (x <= 0 && x >= WINX)
				break ;
			x += slide_sand(x, y, data, direc);
			i--;
		}
		if (i == 0)
			return (1);
	}
	if (data->fsim->map[y + 1][x - 1] < data->fsim->map[y][x] && (equal == 0))
	{
		swap = data->fsim->map[y + 1][x - 1];
		data->fsim->map[y + 1][x - 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		equal = 1;
		if (rand() % 2 && iter[y][x] < 3)
		{
			iter[y + 1][x - 1] += iter[y][x] + 1;
			iter[y][x] = 0;
			emulate_sand(x - 1, y + 1, data, randed, slide, force);
		}
		else
		{
			iter[y][x] = 0;
			return (1);
		}
		return (1);
	}
	else if (data->fsim->map[y + 1][x + 1] < data->fsim->map[y][x] && (equal == 1))
	{
		swap = data->fsim->map[y + 1][x + 1];
		data->fsim->map[y + 1][x + 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		equal = 0;
		if (rand() % 2 && iter[y][x] < 3)
		{
			iter[y + 1][x + 1] += iter[y][x] + 1;
			iter[y][x] = 0;
			emulate_sand(x + 1, y + 1, data, randed, slide, force);
		}
		else
		{
			iter[y][x] = 0;
			return (1);
		}
		return (1);
	}
	if (equal == 0)
		equal = 1;
	else
		equal = 0;
	return (0);
}

void	emulate_water(int x, int y, t_data *data, char c)
{
	static int iter[WINY][WINX];
	static int flud[WINY][WINX];

	if (flud[y][x] == 0)
	{
		if (y < (int)WINY - 2 && emulate_sand(x, y, data, 10, -4, 1) == 1)
			return ;
	}
	flud[y][x] = 0;
	if (((data->fsim->map[y][x - 1] < c) && rand() % 3) && (data->fsim->map[y][x] == c && equal_flud(x, y, data, c) == 0))
	{
		data->fsim->map[y][x] = data->fsim->map[y][x - 1];
		data->fsim->map[y][x - 1] = c;
		if (rand() % 5 && iter[y][x] < 5)
		{
			iter[y][x - 1] += iter[y][x] + 1;
			iter[y][x] = 0;
			flud[y][x - 1] = 1;
			emulate_water(x - 1, y, data, c);
		}
		else
		{
			iter[y][x] = 0;
			return ;
		}
		return ;
	}
	else if ((data->fsim->map[y][x + 1] < c && rand() % 3) && (data->fsim->map[y][x] == c))
	{
		data->fsim->map[y][x] = data->fsim->map[y][x + 1];
		data->fsim->map[y][x + 1] = c;
		if (rand() % 5 && iter[y][x] < 5)
		{
			iter[y][x + 1] += iter[y][x] + 1;
			iter[y][x] = 0;
			flud[y][x + 1] = 1;
			emulate_water(x + 1, y, data, c);
		}
		else
		{
			iter[y][x] = 0;
			return ;
		}
		return ;
	}
}

void	emulate_steam(int x, int y, t_data *data, char c)
{
	if (y > 1 && emulate_gas(x, y, data) == 1)
		return ;
	if (rand() % 1000 == 0 && data->fsim->map[y][x] == c)
	{
		data->fsim->map[y][x] = MAT_ID_WATER;
		return ;
	}
	if ((data->fsim->map[y][x - 1] < c && rand() % 2) && (data->fsim->map[y][x] == c && equal_flud(x, y, data, c) == 0))
	{
		data->fsim->map[y][x] = data->fsim->map[y][x - 1];
		data->fsim->map[y][x - 1] = c;
		return ;
	}
	else if ((data->fsim->map[y][x + 1] < c && rand() % 2) && data->fsim->map[y][x] == c)
	{
		data->fsim->map[y][x] = data->fsim->map[y][x + 1];
		data->fsim->map[y][x + 1] = c;
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
	if (data->fsim->map[y][x] == c)
	{
		pt = find_id(x, y, data, MAT_ID_STONE);
		if (pt.z == 1 && rand() % 100 == 0)
		{
			data->fsim->map[(int)pt.y][(int)pt.x] = c;
			data->fsim->map[y][x] = c;
			return ;
		}
	}
	if (y < (int)WINY - 2 && emulate_sand(x, y, data, 1, -2, 2))
	{
		return ;
	}
	if (rand() % 3 == 0)
		return ;
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
}

void	emulate_acid(int x, int y, t_data *data, char c)
{
	static int	iter[WINY][WINX];
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
	if (y < (int)WINY - 2 && emulate_sand(x, y, data, 10, 1, 1) == 1)
	{
		return ;
	}
	else if (data->fsim->map[y][x - 1] < c && rand() % 2 && data->fsim->map[y][x] == c)
	{
		data->fsim->map[y][x] = data->fsim->map[y][x - 1];
		data->fsim->map[y][x - 1] = c;
		if (rand() % 3 && iter[y][x] < 3)
		{
			iter[y][x - 1] += iter[y][x] + 1;
			iter[y][x] = 0;
			emulate_acid(x - 1, y, data, c);
		}
		else
		{
			iter[y][x] = 0;
			return ;
		}
	}
	else if (data->fsim->map[y][x + 1] < c && data->fsim->map[y][x] == c)
	{
		data->fsim->map[y][x] = data->fsim->map[y][x + 1];
		data->fsim->map[y][x + 1] = c;
		if (rand() % 3 && iter[y][x] < 3)
		{
			iter[y][x + 1] += iter[y][x] + 1;
			iter[y][x] = 0;
			emulate_acid(x + 1, y, data, c);
		}
		else
		{
			iter[y][x] = 0;
			return ;
		}
	}
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
}

void	emulate_woodf(int x, int y, t_data *data)
{
	t_pt	pt;

	if (y > 1 && rand() % 50 == 0 && data->fsim->map[y - 1][x] == MAT_ID_EMPTY)
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

void	emulate_oil(int x, int y, t_data *data, char c)
{
	static int iter[WINY][WINX];
	t_pt	pt;

	pt = find_id(x, y, data, MAT_ID_FIRE);
	if (pt.z == 1)
	{
		data->fsim->map[y][x] = MAT_ID_OILF;
		return ;
	}
	pt = find_id(x, y, data, MAT_ID_LAVA);
	if (pt.z == 1)
	{
		data->fsim->map[y][x] = MAT_ID_OILF;
		return ;
	}
	if (y < (int)WINY - 2 && emulate_sand(x, y, data, 4, 3, 2) == 1)
	{
		return ;
	}	
	if (data->fsim->map[y][x - 1] < c && rand() % 2 && data->fsim->map[y][x] == c)
	{
		data->fsim->map[y][x] = data->fsim->map[y][x - 1];
		data->fsim->map[y][x - 1] = c;
		if (rand() % 4 && iter[y][x] < 3)
		{
			iter[y][x - 1] += iter[y][x] + 1;
			iter[y][x] = 0;
			emulate_water(x - 1, y, data, c);
		}
		else
		{
			iter[y][x] = 0;
			return ;
		}
		return ;
	}
	if (data->fsim->map[y][x + 1] < c && rand() % 2 && data->fsim->map[y][x] == c)
	{
		data->fsim->map[y][x] = data->fsim->map[y][x + 1];
		data->fsim->map[y][x + 1] = c;
		if (rand() % 4 && iter[y][x] < 3)
		{
			iter[y][x + 1] += iter[y][x] + 1;
			iter[y][x] = 0;
			emulate_water(x + 1, y, data, c);
		}
		else
		{
			iter[y][x] = 0;
			return ;
		}
		return ;
	}
}

void	emulate_oilf(int x, int y, t_data *data, char c)
{
	static int iter[WINY][WINX];
	t_pt	pt;

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
	if (rand() % 300 == 0)
	{
		pt = find_id(x, y, data, MAT_ID_WATER);
		if (pt.z == 1)
		{
			data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_STEAM;
		}
	}
	if (y < (int)WINY - 2 && emulate_sand(x, y, data, 4, 4, 1) == 1)
	{
		return ;
	}	
	if (data->fsim->map[y][x - 1] < c && rand() % 2 && data->fsim->map[y][x] == c)
	{
		data->fsim->map[y][x] = data->fsim->map[y][x - 1];
		data->fsim->map[y][x - 1] = c;
		if (rand() % 4 && iter[y][x] < 3)
		{
			iter[y][x - 1] += iter[y][x] + 1;
			iter[y][x] = 0;
			emulate_water(x - 1, y, data, c);
		}
		else
		{
			iter[y][x] = 0;
			return ;
		}
		return ;
	}
	if (data->fsim->map[y][x + 1] < c && rand() % 2 && data->fsim->map[y][x] == c)
	{
		data->fsim->map[y][x] = data->fsim->map[y][x + 1];
		data->fsim->map[y][x + 1] = c;
		if (rand() % 4 && iter[y][x] < 3)
		{
			iter[y][x + 1] += iter[y][x] + 1;
			iter[y][x] = 0;
			emulate_water(x + 1, y, data, c);
		}
		else
		{
			iter[y][x] = 0;
			return ;
		}
		return ;
	}
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
	if (y < (int)WINY - 2 && emulate_sand(x, y, data, 1, 2, 1))
	{
		return ;
	}
}

int	emulate_soap_bubble(int x, int y, t_data *data)
{
	char	swap;
	static int iter[WINY][WINX];
	int	i;
	int	j;

	i = y + 2;
	while (--i >= y - 1)
	{
		j = x - 2;
		while (++j <= x + 1)
		{
			if (data->fsim->map[i][j] != MAT_ID_EMPTY)
			{
				if (rand() % 50 == 0)
				{
					data->fsim->map[y][x] = MAT_ID_EMPTY;
					return (1);
				}
			}
		}
	}
	if (rand() % 3 == 0)
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
		if (rand() % 4 && iter[y][x] < 3)
		{
			iter[y - 1][x] += iter[y][x] + 1;
			iter[y][x] = 0;
			emulate_sand(x, y - 1, data, 2, 2, 0);
		}
		else
		{
			iter[y][x] = 0;
			return (1);
		}
	}	
	else if (data->fsim->map[y - 1][x - 1] < data->fsim->map[y][x] && rand() % 3)
	{
		swap = data->fsim->map[y - 1][x - 1];
		data->fsim->map[y - 1][x - 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		if (rand() % 4 && iter[y][x] < 3)
		{
			iter[y - 1][x - 1] += iter[y][x] + 1;
			iter[y][x] = 0;
			emulate_sand(x - 1, y - 1, data, 2, 2, 0);
		}
		else
		{
			iter[y][x] = 0;
			return (1);
		}
	}	
	else if (data->fsim->map[y - 1][x + 1] < data->fsim->map[y][x])
	{
		swap = data->fsim->map[y - 1][x + 1];
		data->fsim->map[y - 1][x + 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		if (rand() % 4 && iter[y][x] < 3)
		{
			iter[y - 1][x + 1] += iter[y][x] + 1;
			iter[y][x] = 0;
			emulate_sand(x + 1, y - 1, data, 2, 2, 0);
		}
		else
		{
			iter[y][x] = 0;
			return (1);
		}
	}
	return (0);
}

int	emulate_oxygen(int x, int y, t_data *data)
{
	char	swap;
	static int iter[WINY][WINX];
	t_pt	pt;

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
	pt = find_id(x, y, data, MAT_ID_HIDROGEN);
	if (pt.z == 1)
	{
		if (rand() % 100 == 0)
		{

			data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_EMPTY;
			data->fsim->map[y][x] = MAT_ID_WATER;
			return (1);
		}
	}
	pt = find_around_id(x, y, data, MAT_ID_OXYGEN);
	if (pt.z == 0)
		if (rand() % 3)
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
		if (rand() % 3 && iter[y][x] < 3)
		{
			iter[y - 1][x] += iter[y][x] + 1;
			iter[y][x] = 0;
			emulate_sand(x, y - 1, data, 2, 2, 0);
		}
		else
		{
			iter[y][x] = 0;
			return (1);
		}
	}	
	else if (data->fsim->map[y - 1][x - 1] < data->fsim->map[y][x] && rand() % 2)
	{
		swap = data->fsim->map[y - 1][x - 1];
		data->fsim->map[y - 1][x - 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		if (rand() % 3 && iter[y][x] < 3)
		{
			iter[y - 1][x - 1] += iter[y][x] + 1;
			iter[y][x] = 0;
			emulate_sand(x - 1, y - 1, data, 2, 2, 0);
		}
		else
		{
			iter[y][x] = 0;
			return (1);
		}
	}	
	else if (data->fsim->map[y - 1][x + 1] < data->fsim->map[y][x])
	{
		swap = data->fsim->map[y - 1][x + 1];
		data->fsim->map[y - 1][x + 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		if (rand() % 3 && iter[y][x] < 3)
		{
			iter[y - 1][x + 1] += iter[y][x] + 1;
			iter[y][x] = 0;
			emulate_sand(x + 1, y - 1, data, 2, 2, 0);
		}
		else
		{
			iter[y][x] = 0;
			return (1);
		}
	}
	return (0);
}

int	emulate_hidrogen(int x, int y, t_data *data)
{
	char	swap;
	static int iter[WINY][WINX];
	t_pt	pt;

	pt = find_id(x, y, data, MAT_ID_LAVA);
	if (pt.z == 1)
		data->fsim->map[y][x] = MAT_ID_FIRE;
	pt = find_id(x, y, data, MAT_ID_FIRE);
	if (pt.z == 1)
		data->fsim->map[y][x] = MAT_ID_FIRE;
	if (pt.z == 1)
	{
		pt = find_id(x, y, data, MAT_ID_HIDROGEN);
		if (pt.z == 1)
			emulate_hidrogen((int)pt.x, (int)pt.y, data);
		return (1);
	}
	pt = find_around_id(x, y, data, MAT_ID_HIDROGEN);
	if (pt.z == 0)
		if (rand() % 3)
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
		if (rand() % 3 && iter[y][x] < 3)
		{
			iter[y - 1][x] += iter[y][x] + 1;
			iter[y][x] = 0;
			emulate_sand(x, y - 1, data, 2, 2, 0);
		}
		else
		{
			iter[y][x] = 0;
			return (1);
		}
	}	
	else if (data->fsim->map[y - 1][x - 1] < data->fsim->map[y][x] && rand() % 2)
	{
		swap = data->fsim->map[y - 1][x - 1];
		data->fsim->map[y - 1][x - 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		if (rand() % 3 && iter[y][x] < 3)
		{
			iter[y - 1][x - 1] += iter[y][x] + 1;
			iter[y][x] = 0;
			emulate_sand(x - 1, y - 1, data, 2, 2, 0);
		}
		else
		{
			iter[y][x] = 0;
			return (1);
		}
	}	
	else if (data->fsim->map[y - 1][x + 1] < data->fsim->map[y][x])
	{
		swap = data->fsim->map[y - 1][x + 1];
		data->fsim->map[y - 1][x + 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		if (rand() % 3 && iter[y][x] < 3)
		{
			iter[y - 1][x + 1] += iter[y][x] + 1;
			iter[y][x] = 0;
			emulate_sand(x + 1, y - 1, data, 2, 2, 0);
		}
		else
		{
			iter[y][x] = 0;
			return (1);
		}
	}
	return (0);
}

int	emulate_fly(int x, int y, t_data *data)
{
	char	swap;
	static int iter[WINY][WINX];
	t_pt	pt;

	pt = find_id(x, y, data, MAT_ID_LAVA);
	if (pt.z == 1)
		data->fsim->map[y][x] = MAT_ID_FIRE;
	pt = find_id(x, y, data, MAT_ID_FIRE);
	if (pt.z == 1)
		data->fsim->map[y][x] = MAT_ID_FIRE;
	if (pt.z == 1)
	{
		pt = find_id(x, y, data, MAT_ID_HIDROGEN);
		if (pt.z == 1)
			emulate_hidrogen((int)pt.x, (int)pt.y, data);
		return (1);
	}
	pt = find_around_id(x, y, data, MAT_ID_WOOD);
	if (pt.z == 1)
		if (rand() % 1000)
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
		if (rand() % 3 && iter[y][x] < 3)
		{
			iter[y - 1][x] += iter[y][x] + 1;
			iter[y][x] = 0;
			emulate_sand(x, y - 1, data, 2, 2, 0);
		}
		else
		{
			iter[y][x] = 0;
			return (1);
		}
	}	
	else if (data->fsim->map[y - 1][x - 1] < data->fsim->map[y][x] && rand() % 2)
	{
		swap = data->fsim->map[y - 1][x - 1];
		data->fsim->map[y - 1][x - 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		if (rand() % 3 && iter[y][x] < 3)
		{
			iter[y - 1][x - 1] += iter[y][x] + 1;
			iter[y][x] = 0;
			emulate_sand(x - 1, y - 1, data, 2, 2, 0);
		}
		else
		{
			iter[y][x] = 0;
			return (1);
		}
	}	
	else if (data->fsim->map[y - 1][x + 1] < data->fsim->map[y][x])
	{
		swap = data->fsim->map[y - 1][x + 1];
		data->fsim->map[y - 1][x + 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		if (rand() % 3 && iter[y][x] < 3)
		{
			iter[y - 1][x + 1] += iter[y][x] + 1;
			iter[y][x] = 0;
			emulate_sand(x + 1, y - 1, data, 2, 2, 0);
		}
		else
		{
			iter[y][x] = 0;
			return (1);
		}
	}
	return (0);
}
