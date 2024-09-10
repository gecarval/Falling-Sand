/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part_call.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 19:50:15 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/10 20:36:39 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/renderer.h"

void init_random()
{
	srand((unsigned int)time(NULL));
}

int	ft_randomize_col(int ini, int fin)
{
	int	temp;

	if (ini > fin)
	{
		temp = ini;
		ini = fin;
		fin = temp;
	}
	return (ini + rand() % (fin - ini + 1));
}

t_part particle_empty()
{
	t_part p = {0};

	p.id = MAT_ID_EMPTY;
	p.color = MAT_COL_EMPTY;
	return (p);
}

t_part particle_sand()
{
	t_part p = {0};

	p.id = MAT_ID_SAND;
	// Random sand color
	int r = (int)(ft_randomize_col(0, 10)) / 10.f;
	p.color = MAT_COL_SAND + r;
	return (p);
}

t_part particle_water()
{
	t_part p = {0};

	p.id = MAT_ID_WATER;
	int r = (int)(ft_randomize_col(0, 1)) / 2.f;
	p.color = MAT_COL_WATER + r;
	return (p);
}

t_part particle_salt()
{
	t_part p = {0};

	p.id = MAT_ID_SALT;
	int r = (int)(ft_randomize_col(0, 1)) / 2.f;
	p.color = MAT_COL_SALT + r;
	return (p);
}

t_part particle_wood()
{
	t_part p = {0};

	p.id = MAT_ID_WOOD;
	int r = (int)(ft_randomize_col(0, 1)) / 2.f;
	p.color = MAT_COL_WOOD + r;
	return (p);
}

t_part particle_gunpowder()
{
	t_part p = {0};

	p.id = MAT_ID_GUNPOWDER;
	int r = (int)(ft_randomize_col(0, 1)) / 2.f;
	p.color = MAT_COL_GUNPOWDER + r;
	return (p);
}

t_part particle_oil()
{
	t_part p = {0};

	p.id = MAT_ID_OIL;
	int r = (int)(ft_randomize_col(0, 1)) / 2.f;
	p.color = MAT_COL_OIL + r;
	return (p);
}

t_part particle_fire()
{
	t_part p = {0};

	p.id = MAT_ID_FIRE;
	p.color = MAT_COL_FIRE;
	return (p);
}

t_part particle_lava()
{
	t_part p = {0};

	p.id = MAT_ID_LAVA;
	p.color = MAT_COL_LAVA;
	return (p);
}

t_part particle_ember()
{
	t_part p = {0};

	p.id = MAT_ID_EMBER;
	p.color = MAT_ID_EMBER;
	return (p);
}

t_part particle_smoke()
{
	t_part p = {0};

	p.id = MAT_ID_SMOKE;
	p.color = MAT_ID_SMOKE;
	return (p);
}

t_part particle_steam()
{
	t_part p = {0};

	p.id = MAT_ID_STEAM;
	p.color = MAT_ID_STEAM;
	return (p);
}

t_part particle_stone()
{
	t_part p = {0};

	p.id = MAT_ID_STONE;
	int r = (int)(ft_randomize_col(0, 1)) / 2.f;
	p.color = MAT_COL_STONE + r;
	return (p);
}

t_part particle_acid()
{
	t_part p = {0};

	p.id = MAT_ID_ACID;
	int r = (int)(ft_randomize_col(0, 1)) / 2.f;
	p.color = MAT_COL_ACID + r;
	return (p);
}
