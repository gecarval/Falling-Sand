/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_del.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/28 18:29:50 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/10 20:42:28 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/renderer.h"

t_pt	vec_add(t_pt pt1, t_pt pt2)
{
	t_pt	sum;

	sum.x = pt1.x + pt2.x;
	sum.y = pt1.y + pt2.y;
	sum.z = pt1.z + pt2.z;
	sum.color = pt1.color;
	return (sum);
}

t_pt	vec_sub(t_pt pt1, t_pt pt2)
{
	t_pt	diff;

	diff.x = pt1.x - pt2.x;
	diff.y = pt1.y - pt2.y;
	diff.z = pt1.z - pt2.z;
	diff.color = pt1.color;
	return (diff);
}

t_pt	mat_mult(t_matrix3x3 mat, t_matrix3x3 pt)
{
	t_pt	res;

	res.x = pt.c1r1 * mat.c1r1 + pt.c2r1 * mat.c2r1 + pt.c3r1 * mat.c3r1;
	res.y = pt.c1r2 * mat.c1r2 + pt.c2r2 * mat.c2r2 + pt.c3r2 * mat.c3r2;
	res.z = pt.c1r3 * mat.c1r3 + pt.c2r3 * mat.c2r3 + pt.c3r3 * mat.c3r3;
	res.color = 0xffffff;
	return (res);
}

t_pt	mtvc_mult(t_matrix3x3 mat, t_pt pt)
{
	t_pt	res;

	res.x = (pt.x * mat.c1r1) + (pt.y * mat.c2r1) + (pt.z * mat.c3r1);
	res.y = (pt.x * mat.c1r2) + (pt.y * mat.c2r2) + (pt.z * mat.c3r2);
	res.z = (pt.x * mat.c1r3) + (pt.y * mat.c2r3) + (pt.z * mat.c3r3);
	res.color = pt.color;
	return (res);
}

t_delta	retdel(float_t ini, float_t fin)
{
	t_delta a;

	a.ini = ini;
	a.fin = fin;
	return (a);
}

void	def_objinf(t_objinf *a, t_delta x, t_delta y, int color)
{
	a->pox = x.ini;
	a->poy = y.ini;
	a->sx = x.fin;
	a->sy = y.fin;
	a->color = color;
}

void	offset_scale(t_objinf *a, int scale)
{
	a->pox = (a->pox * scale) + ((WINX / 2) - scale / 2);
	a->poy = (a->poy * scale) + ((WINY / 2) - scale / 2);
}

void	offset_scale_cur(t_pt *a, int scale)
{
	a->x = (a->x * scale) + ((WINX / 2) - scale / 2);
	a->y = (a->y * scale) + ((WINY / 2) - scale / 2);
	a->z = (a->z * scale);
}
