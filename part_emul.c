/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_emul.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:28:33 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/10 14:17:23 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/renderer.h"

void	push_momentum(int x, int y, t_data *data, int force)
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
		if (data->fsim->map[y][i] == MAT_ID_WOOD
			|| data->fsim->map[y][i] == MAT_ID_GLASS
			|| data->fsim->map[y][i] == MAT_ID_IRON
			|| data->fsim->map[y][i] == MAT_ID_HOTIRON)
			break ;
		if (data->fsim->map[y][i] > data->fsim->map[y - 1][x])
			if (rand() % force == 0)
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
		if (data->fsim->map[y][i] == MAT_ID_WOOD
			|| data->fsim->map[y][i] == MAT_ID_GLASS
			|| data->fsim->map[y][i] == MAT_ID_IRON
			|| data->fsim->map[y][i] == MAT_ID_HOTIRON)
			break ;
		if (data->fsim->map[y][i] > data->fsim->map[y - 1][x])
			if (rand() % force == 0)
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
		render_fluidmap_pp(x, y, data);
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

int	ft_find_side_id(int x, int y, t_data *data, char c, int dist)
{
	int	i;
	int	l;
	int	r;

	//render_per_pixel(x, y, data);
	i = x - dist;
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
	while (i < x + dist + 1)
	{
		if (i < WINX)
			if (data->fsim->map[y][i] == c)
				r++;
		i++;
	}
	return (r + l);
}

int	equal_flud(int x, int y, t_data *data, char c, int dist)
{
	int	i;
	int	l;
	int	r;

	i = x - dist;
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
	while (i < x + dist + 1)
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

int	emulate_wetsand(int x, int y, t_data *data, int randed, int slide, int force, int inertialchance, int inertialres)
{
	t_pt	pt;

	pt = find_id(x, y, data, MAT_ID_FIRE);
	if (pt.z == 1)
	{
		data->fsim->map[(int)pt.y][(int)pt.x] = MAT_ID_STEAM;
		data->fsim->map[y][x] = MAT_ID_SAND;
		return (1);
	}
	if (emulate_solid(x, y, data, randed, slide, force, inertialchance, inertialres))
		return (1);
	return (0);
}

int	emulate_solid(int x, int y, t_data *data, int randed, int slide, int force, int inertialchance, int inertialres)
{
	int		i;
	int		direc;
	char		swap;
	static int	equal = 0;
	static int	stable[WINY][WINX];
	static int	iter[WINY][WINX];
	static int	update[WINY][WINX];

	if (update[y][x] > 0)
	{
		update[y][x] = 0;
		return (1);
	}
	if (data->fsim->map[y + 1][x] < data->fsim->map[y][x] && rand() % randed == 0)
	{
		stable[y][x] = 0;
		if (rand() % inertialres == 0)
		{
			if (stable[y][x] == 0 && data->fsim->map[y][x - 1] != MAT_ID_EMPTY)
			{
				if (stable[y][x - 1] == 1)
					stable[y][x - 1] = 0;
			}
			else if(stable[y][x] == 0 && data->fsim->map[y][x + 1] != MAT_ID_EMPTY)
			{
				if (stable[y][x + 1] == 1)
					stable[y][x + 1] = 0;
			}
		}
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
		update[y][x] = 0;
		if (rand() % 2 && iter[y][x] < 3)
		{
			iter[y + 1][x] += iter[y][x] + 1;
			update[y + 1][x] = 0;
			iter[y][x] = 0;
			emulate_solid(x, y + 1, data, randed, slide, force, inertialchance, inertialres);
		}
		else
			iter[y][x] = 0;
		return (1);
	}
	while (iter[y][x] > 0 && (data->fsim->map[y + 1][x] >= data->fsim->map[y][x]))
	{
		if (rand() % 3)
			push_momentum(x, y + 1, data, force);
		i = iter[y][x] + slide;
		iter[y][x] = 0;
		direc = equal_flud(x, y, data, data->fsim->map[y][x], 5);
		if (i <= 0)
			break ;
		while (i > 0)
		{
			if (x <= 0 && x >= WINX)
				break ;
			x += slide_sand(x, y, data, direc);
			update[y][x] = 0;
			i--;
		}
		if (direc == 1)
			update[y][x] += update[y][x] + 1;
		if (i == 0)
			return (1);
	}
	if (stable[y][x] == 0)
	{
		if (data->fsim->map[y + 1][x - 1] < data->fsim->map[y][x] && (equal == 0))
		{
			swap = data->fsim->map[y + 1][x - 1];
			data->fsim->map[y + 1][x - 1] = data->fsim->map[y][x];
			data->fsim->map[y][x] = swap;
			update[y][x] = 0;
			equal = 1;
			if (rand() % 2 && iter[y][x] < 3)
			{
				iter[y + 1][x - 1] += iter[y][x] + 1;
				update[y + 1][x - 1] = 0;
				iter[y][x] = 0;
				emulate_solid(x - 1, y + 1, data, randed, slide, force, inertialchance, inertialres);
			}
			else
				iter[y][x] = 0;
			return (1);
		}
		else if (data->fsim->map[y + 1][x + 1] < data->fsim->map[y][x] && (equal == 1))
		{
			swap = data->fsim->map[y + 1][x + 1];
			data->fsim->map[y + 1][x + 1] = data->fsim->map[y][x];
			data->fsim->map[y][x] = swap;
			update[y][x] = 0;
			equal = 0;
			if (rand() % 2 && iter[y][x] < 3)
			{
				iter[y + 1][x + 1] += iter[y][x] + 1;
				update[y + 1][x + 1] = 0;
				iter[y][x] = 0;
				emulate_solid(x + 1, y + 1, data, randed, slide, force, inertialchance, inertialres);
			}
			else
				iter[y][x] = 0;
			return (1);
		}
	}
	if (equal == 0)
		equal = 1;
	else
		equal = 0;
	if (rand() % inertialchance)
		stable[y][x] = 1;
	return (0);
}

int	emulate_fall(int x, int y, t_data *data, int randed, int slide, int force)
{
	int		i;
	int		direc;
	char		swap;
	static int	equal = 0;
	static int	iter[WINY][WINX];
	static int	update[WINY][WINX];

	if (update[y][x] > 0)
	{
		update[y][x] = 0;
		return (1);
	}
	while (iter[y][x] > 0 && (data->fsim->map[y + 1][x] >= data->fsim->map[y][x]))
	{
		if (rand() % 3)
			push_momentum(x, y + 1, data, force);
		i = iter[y][x] + slide;
		iter[y][x] = 0;
		direc = equal_flud(x, y, data, data->fsim->map[y][x], 5);
		if (i <= 0)
			break ;
		while (i > 0)
		{
			if (x <= 0 && x >= WINX)
				break ;
			x += slide_sand(x, y, data, direc);
			i--;
		}
		if (direc == 1)
			update[y][x] += update[y][x] + 1;
		if (i == 0)
			return (1);
	}
	if (rand() % randed > 0)
	{
		if (equal == 0 && data->fsim->map[y + 1][x - 1] < data->fsim->map[y][x])
		{
			swap = data->fsim->map[y + 1][x - 1];
			data->fsim->map[y + 1][x - 1] = data->fsim->map[y][x];
			data->fsim->map[y][x] = swap;
			update[y][x] = 0;
			equal = 1;
			if (rand() % 2 && iter[y][x] < 3)
			{
				iter[y + 1][x - 1] += iter[y][x] + 1;
				update[y + 1][x - 1] = 0;
				iter[y][x] = 0;
				emulate_fall(x - 1, y + 1, data, randed, slide, force);
			}
			else
				iter[y][x] = 0;
			return (1);
		}
		else if (equal == 1 && data->fsim->map[y + 1][x + 1] < data->fsim->map[y][x])
		{
			swap = data->fsim->map[y + 1][x + 1];
			data->fsim->map[y + 1][x + 1] = data->fsim->map[y][x];
			data->fsim->map[y][x] = swap;
			update[y][x] = 0;
			equal = 0;
			if (rand() % 2 && iter[y][x] < 3)
			{
				iter[y + 1][x + 1] += iter[y][x] + 1;
				update[y + 1][x + 1] = 0;
				iter[y][x] = 0;
				emulate_fall(x + 1, y + 1, data, randed, slide, force);
			}
			else
				iter[y][x] = 0;
			return (1);
		}
	}
	if (data->fsim->map[y + 1][x] < data->fsim->map[y][x])
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
		update[y][x] = 0;
		if (rand() % 2 && iter[y][x] < 3)
		{
			iter[y + 1][x] += iter[y][x] + 1;
			update[y + 1][x] = 0;
			iter[y][x] = 0;
			emulate_fall(x, y + 1, data, randed, slide, force);
		}
		else
			iter[y][x] = 0;
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

int	emulate_gas(int x, int y, t_data *data)
{
	char		swap;
	static int	iter[WINY][WINX];
	static int	equal = 0;
	static int	update[WINY][WINX];

	if (update[y][x] > 0)
	{
		update[y][x] = 0;
		return (1);
	}
	if (rand() % 2 == 0)
	{
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
		if (rand() % 2 && iter[y][x] < 3)
		{
			iter[y - 1][x] += iter[y][x] + 1;
			update[y - 1][x] = 0;
			iter[y][x] = 0;
			emulate_gas(x, y - 1, data);
		}
		else
			iter[y][x] = 0;
		return (1);
	}	
	if ((rand() % 2 && equal == 1) && data->fsim->map[y - 1][x - 1] < data->fsim->map[y][x])
	{
		swap = data->fsim->map[y - 1][x - 1];
		data->fsim->map[y - 1][x - 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		equal = 0;
		update[y - 1][x - 1] += update[y][x] + 1;
		update[y][x] = 0;
		if (rand() % 2 && iter[y][x] < 3)
		{
			iter[y - 1][x - 1] += iter[y][x] + 1;
			update[y - 1][x - 1] = 0;
			iter[y][x] = 0;
			emulate_gas(x - 1, y - 1, data);
		}
		else
			iter[y][x] = 0;
		return (1);
	}	
	if ((rand() % 2 && equal == 0) && data->fsim->map[y - 1][x + 1] < data->fsim->map[y][x])
	{
		swap = data->fsim->map[y - 1][x + 1];
		data->fsim->map[y - 1][x + 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		equal = 1;
		update[y - 1][x + 1] += update[y][x] + 1;
		update[y][x] = 0;
		if (rand() % 2 && iter[y][x] < 3)
		{
			iter[y - 1][x + 1] += iter[y][x] + 1;
			update[y - 1][x + 1] = 0;
			iter[y][x] = 0;
			emulate_gas(x + 1, y - 1, data);
		}
		else
			iter[y][x] = 0;
		return (1);
	}	
	if (equal == 0)
		equal = 1;
	else
		equal = 0;
	return (0);
}

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

int	conduction(int x, int y, t_data *data, char b, char c)
{
	if (data->fsim->map[y][x + 1] == b && data->fsim->map[y + 1][x + 1] != b)
	{
		data->fsim->map[y][x + 1] = c;
		data->fsim->map[y][x] = b;
		return (4);
	}
	if (data->fsim->map[y - 1][x] == b && data->fsim->map[y - 1][x + 1] != b)
	{
		data->fsim->map[y - 1][x] = c;
		data->fsim->map[y][x] = b;
		return (2);
	}
	if (data->fsim->map[y][x - 1] == b && data->fsim->map[y - 1][x - 1] != b)
	{
		data->fsim->map[y][x - 1] = c;
		data->fsim->map[y][x] = b;
		return (0);
	}
	if (data->fsim->map[y + 1][x] == b && data->fsim->map[y + 1][x - 1] != b)
	{
		data->fsim->map[y + 1][x] = c;
		data->fsim->map[y][x] = b;
		return (0);
	}
	return (10);
}

t_pt	ft_heat_sink(int x, int y, t_data *data)
{
	t_pt	pt;
	int		i;
	int		j;

	pt.x = x;
	pt.y = y;
	pt.z = 0;
	pt.color = 0;
	i = y - 2;
	while (++i <= y + 1)
	{
		j = x - 2;
		while (++j <= x + 1)
		{
			if (data->fsim->map[i][j] == MAT_ID_IRON)
			{
				if (rand() % 4 == 0)
				{
					data->fsim->map[i][j] = MAT_ID_HOTIRON;
					data->fsim->map[y][x] = MAT_ID_IRON;
					pt.x = j;
					pt.y = i;
					pt.z = 1;
					return (pt);
				}
			}
		}
	}
	return (pt);
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

int	emulate_fly(int x, int y, t_data *data)
{
	int		i;
	int		j;
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
		return (1);
	i = y + 2;
	if (rand() % 50)
	{
		pt = find_around_id(x, y, data, MAT_ID_WOOD, 1);
		if (pt.z == 1)
			return (1);
	}
	while (--i >= y - 1)
	{
		j = x - 2;
		while (++j <= x + 1)
		{
			if (data->fsim->map[i][j] == MAT_ID_WOOD)
			{
				if (rand() % 25 == 0)
				{
					data->fsim->map[i][j] = MAT_ID_FLY;
					data->fsim->map[y][x] = MAT_ID_SAND;
					if (j > x || i < y)
						update[i][j] = update[y][x] + 1;
					update[y][x] = 0;
					iter[y][x] = 0;
				}
				return (1);
			}
		}
	}
	if (rand() % 8 == 0 && data->fsim->map[y + 1][x] < data->fsim->map[y][x])
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
		if (rand() % 2 && iter[y][x] < 3)
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
		update[y][x] = 0;
		if (rand() % 2 && iter[y][x] < 3)
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
	if (rand() % 7 && data->fsim->map[y - 1][x + 1] < data->fsim->map[y][x])
	{
		swap = data->fsim->map[y - 1][x + 1];
		data->fsim->map[y - 1][x + 1] = data->fsim->map[y][x];
		data->fsim->map[y][x] = swap;
		update[y - 1][x + 1] += update[y][x] + 1;
		update[y][x] = 0;
		if (rand() % 2 && iter[y][x] < 3)
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
