/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_emul.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/02 15:28:33 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/10 20:36:47 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/renderer.h"

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
