/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_emul_logic.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/10 19:47:59 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/10 20:37:14 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/renderer.h"

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

void	explosion_calc(int xc, int yc, int x, int y, t_data *data, int id, int res, int thougth)
{
	if (xc <= 0 || xc >= WINX)
		return ;
	if (yc <= 0 || yc >= WINX)
		return ;
	if ((yc + y > 0 && yc + y < WINY) && (xc + x > 0 && xc + x < WINX))
	{
		if(data->fsim->map[yc + y][xc + x] != 'z' && data->fsim->map[yc + y][xc + x] != thougth)
		{
			if (data->fsim->map[yc + y][xc + x] == MAT_ID_EMPTY)
				data->fsim->map[yc + y][xc + x] = id;
			else if (data->fsim->map[yc + y][xc + x] != MAT_ID_EMPTY && rand() % res == 0)
				data->fsim->map[yc + y][xc + x] = id;
		}
	}
	if ((yc + y > 0 && yc + y < WINY) && (xc - x > 0 && xc - x < WINX))
	{
		if(data->fsim->map[yc + y][xc - x] != 'z' && data->fsim->map[yc + y][xc - x] != thougth)
		{
			if (data->fsim->map[yc + y][xc - x] == MAT_ID_EMPTY)
				data->fsim->map[yc + y][xc - x] = id;
			else if (data->fsim->map[yc + y][xc - x] != MAT_ID_EMPTY && rand() % res == 0)
				data->fsim->map[yc + y][xc - x] = id;
		}
	}
	if ((yc - y > 0 && yc - y < WINY) && (xc + x > 0 && xc + x < WINX))
	{
		if(data->fsim->map[yc - y][xc + x] != 'z' && data->fsim->map[yc - y][xc + x] != thougth)
		{
			if (data->fsim->map[yc - y][xc + x] == MAT_ID_EMPTY)
				data->fsim->map[yc - y][xc + x] = id;
			else if (data->fsim->map[yc - y][xc + x] != MAT_ID_EMPTY && rand() % res == 0)
				data->fsim->map[yc - y][xc + x] = id;
		}
	}
	if ((yc - y > 0 && yc - y < WINY) && (xc - x > 0 && xc - x < WINX))
	{
		if(data->fsim->map[yc - y][xc - x] != 'z' && data->fsim->map[yc - y][xc - x] != thougth)
		{
			if (data->fsim->map[yc - y][xc - x] == MAT_ID_EMPTY)
				data->fsim->map[yc - y][xc - x] = id;
			else if (data->fsim->map[yc - y][xc - x] != MAT_ID_EMPTY && rand() % res == 0)
				data->fsim->map[yc - y][xc - x] = id;
		}
	}
	if ((yc + x > 0 && yc + x < WINY) && (xc + y > 0 && xc + y < WINX))
	{
		if(data->fsim->map[yc + x][xc + y] != 'z' && data->fsim->map[yc + x][xc + y] != thougth)
		{
			if (data->fsim->map[yc + x][xc + y] == MAT_ID_EMPTY)
				data->fsim->map[yc + x][xc + y] = id;
			else if (data->fsim->map[yc + x][xc + y] != MAT_ID_EMPTY && rand() % res == 0)
				data->fsim->map[yc + x][xc + y] = id;
		}
	}
	if ((yc + x > 0 && yc + x < WINY) && (xc - y > 0 && xc - y < WINX))
	{
		if(data->fsim->map[yc + x][xc - y] != 'z' && data->fsim->map[yc + x][xc - y] != thougth)
		{
			if (data->fsim->map[yc + x][xc - y] == MAT_ID_EMPTY)
				data->fsim->map[yc + x][xc - y] = id;
			else if (data->fsim->map[yc + x][xc - y] != MAT_ID_EMPTY && rand() % res == 0)
				data->fsim->map[yc + x][xc - y] = id;
		}
	}
	if ((yc - x > 0 && yc - x < WINY) && (xc + y > 0 && xc + y < WINX))
	{
		if(data->fsim->map[yc - x][xc + y] != 'z' && data->fsim->map[yc - x][xc + y] != thougth)
		{
			if (data->fsim->map[yc - x][xc + y] == MAT_ID_EMPTY)
				data->fsim->map[yc - x][xc + y] = id;
			else if (data->fsim->map[yc - x][xc + y] != MAT_ID_EMPTY && rand() % res == 0)
				data->fsim->map[yc - x][xc + y] = id;
		}
	}
	if ((yc - x > 0 && yc - x < WINY) && (xc - y > 0 && xc - y < WINX))
	{
		if(data->fsim->map[yc - x][xc - y] != 'z' && data->fsim->map[yc - x][xc - y] != thougth)
		{
			if (data->fsim->map[yc - x][xc - y] == MAT_ID_EMPTY)
				data->fsim->map[yc - x][xc - y] = id;
			else if (data->fsim->map[yc - x][xc - y] != MAT_ID_EMPTY && rand() % res == 0)
				data->fsim->map[yc - x][xc - y] = id;
		}
	}
}

void	explosion_circle(int xc, int yc, int r, t_data *data, int id, int res, int thought)
{
	int	x;
	int	y;
	int	d;

	if (r < 0)
		return ;
	x = 0;
	y = r;
	d = 3 - 2 * r;
	explosion_calc(xc, yc, x, y, data, id, res, thought);
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
		explosion_calc(xc, yc, x, y, data, id, res, thought);
	}
	explosion_circle(xc, yc, r - 1, data, id, res, thought);
}

void	explode(int x, int y, t_data *data, int radius, int chance, int id, int res, int thought)
{
	if (chance == 1)
		return ;
	explosion_circle(x, y, radius, data, id, res, thought);
	if (rand() % chance > 0)
		explode(x, y, data, radius + 1, chance - 1, id, res, thought);
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
