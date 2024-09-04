/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 16:57:08 by gecarval          #+#    #+#             */
/*   Updated: 2024/08/30 11:53:10 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/renderer.h"

t_pt	*pt_dup(t_pt *a)
{
	t_pt	*b;

	b = (t_pt *)malloc(sizeof(t_pt));
	b->x = a->x;
	b->y = a->y;
	b->z = a->z;
	b->color = a->color;
	return (b);
}

void	connect_vertices(t_data *data, t_pt *cur, int i, t_matrix3x3 rotx, t_matrix3x3 roty, t_matrix3x3 rotz, t_matrix3x3 prj)
{
	t_pt	next1;
	t_pt	next2;
	t_pt	next3;
	float_t	pers;

	next1 = *(data->map->mapz + (i + ((i + (data->map->max_x - 1)) % data->map->max_x)));
	next2 = *(data->map->mapz + i);
	next3 = *(data->map->mapz + i);
	if (i + data->map->max_y < (data->map->max_x * data->map->max_y * data->map->max_z) && (data->map->mapz + i)->z == (data->map->mapz + (i + data->map->max_y))->z)
		next2 = *(data->map->mapz + (i + data->map->max_y));
	if (i + (data->map->max_x * data->map->max_y) < (data->map->max_x * data->map->max_y * data->map->max_z))
		next3 = *(data->map->mapz + (i + (data->map->max_x * data->map->max_y)));
	next1 = mtvc_mult(rotx, mtvc_mult(roty, mtvc_mult(rotz, next1)));
	next2 = mtvc_mult(rotx, mtvc_mult(roty, mtvc_mult(rotz, next2)));
	next3 = mtvc_mult(rotx, mtvc_mult(roty, mtvc_mult(rotz, next3)));
	pers = 1 / (data->dist - next1.z);
	prj = (t_matrix3x3){pers, 0, 0, 0, pers, 0, 0, 0, 1};
	next1 = mtvc_mult(prj, next1);
	pers = 1 / (data->dist - next2.z);
	prj = (t_matrix3x3){pers, 0, 0, 0, pers, 0, 0, 0, 1};
	next2 = mtvc_mult(prj, next2);
	pers = 1 / (data->dist - next3.z);
	prj = (t_matrix3x3){pers, 0, 0, 0, pers, 0, 0, 0, 1};
	next3 = mtvc_mult(prj, next3);
	offset_scale_cur(&next1, data->map->space);
	offset_scale_cur(&next2, data->map->space);
	offset_scale_cur(&next3, data->map->space);
	draw_line(retdel(cur->x, next1.x), retdel(cur->y, next1.y), data, cur->color);
	draw_line(retdel(cur->x, next2.x), retdel(cur->y, next2.y), data, cur->color);
	draw_line(retdel(cur->x, next3.x), retdel(cur->y, next3.y), data, cur->color);
}

void	draw_vertices(t_data *data, float_t ang)
{
	t_matrix3x3	rotx;
	t_matrix3x3	roty;
	t_matrix3x3	rotz;
	t_matrix3x3	prj;
	t_objinf	cube;
	t_pt		*cur;
	int		i;
	float_t		pers;

	i = -1;
	while (++i < data->map->max_x * data->map->max_y * data->map->max_z)
	{
		cur = pt_dup(data->map->mapz + i);
		rotx = (t_matrix3x3){1, 0, 0, 0, cos(ang), -sin(ang), 0, sin(ang), cos(ang)};
		roty = (t_matrix3x3){cos(ang), 0, sin(ang), 0 , 1, 0, -sin(ang), 0, cos(ang)};
		rotz = (t_matrix3x3){cos(ang), -sin(ang), 0, sin(ang), cos(ang), 0, 0, 0, 1};
		//rotx = (t_matrix3x3){1, 0, 0, 0, 1, 0, 0, 0, 1};
		//rotz = (t_matrix3x3){1, 0, 0, 0, 1, 0, 0, 0, 1};
		*cur = mtvc_mult(rotx, mtvc_mult(roty, mtvc_mult(rotz, *cur)));
		pers = 1 / (data->dist - cur->z);
		prj = (t_matrix3x3){pers, 0, 0, 0, pers, 0, 0, 0, 1};
		*cur = mtvc_mult(prj, *cur);
		def_objinf(&cube, retdel(cur->x, 5), retdel(cur->y, 5), 0xDD0000);
		offset_scale(&cube, data->map->space);
		offset_scale_cur(cur, data->map->space);
		connect_vertices(data, cur, i, rotx, roty, rotz, prj);
		draw_rectang(cube, data);
		free(cur);
	}
}

int	main(void)
{
	t_data *data;

	data = (t_data *)malloc(sizeof(t_data));
	if (!data)
		display_error(data, "malloc error\n");
	ft_init_program(data);
	create_mapz(data);
	data->iso = 0.0;
	data->ani = 0;
	data->anispeed = 0.01;
	render_background(data, 0x000000);
	draw_vertices(data, data->iso);
	mlx_put_image_to_window(data->ini, data->win, data->img->img_ptr, 0, 0);
	water_mark(data);
	mlx_hook(data->win, DestroyNotify, 0, exit_data, data);
	mlx_key_hook(data->win, mlx_cooked, data);
	mlx_loop_hook(data->ini, mlx_anim, data);
	mlx_loop(data->ini);
	return (0);
}
