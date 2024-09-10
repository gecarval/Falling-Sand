/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cubix.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/30 12:01:15 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/10 20:42:20 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/renderer.h"

void	connect_vertices(t_data *data, t_pt *cur, int i)
{
	t_pt	next1;
	t_pt	next2;
	t_pt	next3;
	float_t		pers;

	next1 = *(data->map->mapz + (i + ((i + (data->map->max_x - 1)) % data->map->max_x)));
	next2 = *(data->map->mapz + i);
	next3 = *(data->map->mapz + i);
	if (i + data->map->max_y < (data->map->max_x * data->map->max_y * data->map->max_z) && (data->map->mapz + i)->z == (data->map->mapz + (i + data->map->max_y))->z)
		next2 = *(data->map->mapz + (i + data->map->max_y));
	if (i + (data->map->max_x * data->map->max_y) < (data->map->max_x * data->map->max_y * data->map->max_z))
		next3 = *(data->map->mapz + (i + (data->map->max_x * data->map->max_y)));
	next1 = mtvc_mult(*data->map->rotx, mtvc_mult(*data->map->roty, mtvc_mult(*data->map->rotz, next1)));
	next2 = mtvc_mult(*data->map->rotx, mtvc_mult(*data->map->roty, mtvc_mult(*data->map->rotz, next2)));
	next3 = mtvc_mult(*data->map->rotx, mtvc_mult(*data->map->roty, mtvc_mult(*data->map->rotz, next3)));
	pers = 1 / (data->dist - next1.z);
	*data->map->prj = (t_matrix3x3){pers, 0, 0, 0, pers, 0, 0, 0, 1};
	next1 = mtvc_mult(*data->map->prj, next1);
	pers = 1 / (data->dist - next2.z);
	*data->map->prj = (t_matrix3x3){pers, 0, 0, 0, pers, 0, 0, 0, 1};
	next2 = mtvc_mult(*data->map->prj, next2);
	pers = 1 / (data->dist - next3.z);
	*data->map->prj = (t_matrix3x3){pers, 0, 0, 0, pers, 0, 0, 0, 1};
	next3 = mtvc_mult(*data->map->prj, next3);
	offset_scale_cur(&next1, data->map->space);
	offset_scale_cur(&next2, data->map->space);
	offset_scale_cur(&next3, data->map->space);
	draw_line(retdel(cur->x, next1.x), retdel(cur->y, next1.y), data, cur->color);
	draw_line(retdel(cur->x, next2.x), retdel(cur->y, next2.y), data, cur->color);
	draw_line(retdel(cur->x, next3.x), retdel(cur->y, next3.y), data, cur->color);
}

void	draw_vertices(t_data *data, float_t ang)
{
	t_objinf	cube;
	t_pt		*cur;
	int		i;
	float_t		pers;

	i = -1;
	(void)ang;
	*data->map->rotx = (t_matrix3x3){1, 0, 0, 0, cos(data->iso), -sin(data->iso), 0, sin(data->iso), cos(data->iso)};
	*data->map->roty = (t_matrix3x3){cos(data->iso), 0, sin(data->iso), 0 , 1, 0, -sin(data->iso), 0, cos(data->iso)};
	*data->map->rotz = (t_matrix3x3){cos(data->iso), -sin(data->iso), 0, sin(data->iso), cos(data->iso), 0, 0, 0, 1};
	*data->map->prj = (t_matrix3x3){1, 0, 0, 0, 1, 0, 0, 0, 1};
	while (++i < data->map->max_x * data->map->max_y * data->map->max_z)
	{
		cur = pt_dup(data->map->mapz + i);
		//rotx = (t_matrix3x3){1, 0, 0, 0, 1, 0, 0, 0, 1};
		//rotz = (t_matrix3x3){1, 0, 0, 0, 1, 0, 0, 0, 1};
		*cur = mtvc_mult(*data->map->rotx, mtvc_mult(*data->map->roty, mtvc_mult(*data->map->rotz, *cur)));
		pers = 1 / (data->dist - cur->z);
		*data->map->prj = (t_matrix3x3){pers, 0, 0, 0, pers, 0, 0, 0, 1};
		*cur = mtvc_mult(*data->map->prj, *cur);
		def_objinf(&cube, retdel(cur->x, 5), retdel(cur->y, 5), 0xDD0000);
		offset_scale(&cube, data->map->space);
		offset_scale_cur(cur, data->map->space);
		connect_vertices(data, cur, i);
		draw_rectang(cube, data);
		free(cur);
	}
}
