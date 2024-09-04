/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   reads.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 20:37:31 by gecarval          #+#    #+#             */
/*   Updated: 2024/08/30 11:52:49 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/renderer.h"

void	create_mapz(t_data *data)
{
	t_map	*map;

	map = data->map;
	map->min_x = 1;
	map->max_x = 2;
	map->min_y = 1;
	map->max_y = 2;
	map->min_z = 1;
	map->max_z = 2;
	data->map->rotx = (t_matrix3x3 *)malloc(sizeof(t_matrix3x3));
	data->map->roty = (t_matrix3x3 *)malloc(sizeof(t_matrix3x3));
	data->map->rotz = (t_matrix3x3 *)malloc(sizeof(t_matrix3x3));
	data->map->prj = (t_matrix3x3 *)malloc(sizeof(t_matrix3x3));
	*data->map->rotx = (t_matrix3x3){1, 0, 0, 0, cos(data->iso), -sin(data->iso), 0, sin(data->iso), cos(data->iso)};
	*data->map->roty = (t_matrix3x3){cos(data->iso), 0, sin(data->iso), 0 , 1, 0, -sin(data->iso), 0, cos(data->iso)};
	*data->map->rotz = (t_matrix3x3){cos(data->iso), -sin(data->iso), 0, sin(data->iso), cos(data->iso), 0, 0, 0, 1};
	*data->map->prj = (t_matrix3x3){1, 0, 0, 0, 1, 0, 0, 0, 1};
	map->mapz = (t_pt *)malloc(sizeof(t_pt) * map->max_y * map->max_x * map->max_z);
	if (!map->mapz)
		display_error(data, "mapi malloc error\n");
	map->space = 150;
	data->dist = 3;
	fake_map(map);
	backup_map(data);
}

void	fake_map(t_map *map)
{
	int		i;
	int		j;
	int		k;
	t_pt	*cur;
	t_pt	pt;

	pt.z = -(map->max_z / 2);
	k = 0;
	while (k < map->max_z)
	{
		pt.y = -(map->max_y / 2);
		j = 0;
		while (j < map->max_y)
		{
			pt.x = -(map->max_x / 2);
			i = 0;
			while (i < map->max_x)
			{
				cur = map->mapz + (k * map->max_y + (k + j) * map->max_x + i);
				*cur = pt;
				cur->color = 0xffffff;
				pt.x += 1;
				if (pt.x == 0)
					pt.x = pt.x + 1;
				i++;
			}
			pt.y += 1;
			if (pt.y == 0)
				pt.y = pt.y + 1;
			j++;
		}
		pt.z += 1;
		if (pt.z == 0)
			pt.z = pt.z + 1;
		k++;
	}
}

void	set_height_range(t_map *map, t_pt *cur)
{
	if (cur->z < map->min_z)
		map->min_z = cur->z;
	if (cur->z > map->max_z)
		map->max_z = cur->z;
}

void	fill_map(t_data *data, t_map *map)
{
	int		i;
	int		j;
	t_pt	*cur;
	t_pt	pt;

	pt.z = 0;
	pt.y = map->space * map->max_y / 2;
	i = 0;
	while (i < map->max_y)
	{
		pt.x = -map->space * map->max_x / 2;
		j = 0;
		while (j < map->max_x)
		{	
			cur = map->mapz + i * map->max_x + j;
			*cur = pt;
			cur->z = ft_atoi(data->map->parsed_file[i * map->max_x + j]);
			set_height_range(map, cur);
			cur->color = 0xffffff;
			pt.x += map->space;
			j++;
		}
		pt.y += map->space;
		i++;
	}
}

void	backup_map(t_data *data)
{
	t_pt	*cur;
	int		i;

	data->original_map = malloc(sizeof(t_map));
	if (data->original_map == NULL)
		display_error(data, "backup malloc error\n");
	data->original_map->mapz = malloc(data->map->max_x
			* data->map->max_y * sizeof(t_pt));
	if (data->original_map->mapz == NULL)
		display_error(data, "backup malloc error\n");
	i = 0;
	while (i < data->map->max_x * data->map->max_y)
	{
		cur = data->map->mapz + i;
		*(data->original_map->mapz + i) = *cur;
		i++;
	}
}

/*char	*get_vector(t_data *data)
{
	char	*xt;
	int	x;
	int	i;

	x = 2;
	xt = (char *)malloc(sizeof(char) * (x + 1));
	if (!xt)
		display_error(data, "tensor malloc error\n");

	xt[x] = '\0';
	i = 0;
	while (xt[i] != '\0')
	{
		xt[i] = '0';
		i++;
	}
	return (xt);
}

char	**get_matrix(t_data *data)
{
	char	**yt;
	int	y;
	int	i;

	y = 2;
	yt = (char **)malloc(sizeof(char *) * (y + 1));
	if (!yt)
		display_error(data, "tensor malloc error\n");
	yt[y] = NULL;
	i = 0;
	while (yt[i] != NULL)
	{
		yt[i] = get_vector(data);
		i++;
	}
	return (yt);
}

void	create_tensor(t_data *data)
{
	int	i;
	int	z;

	z = 2;
	data->map->tensor = (char ***)malloc(sizeof(char **) * (z + 1));
	if (!data->map->tensor)
		display_error(data, "tensor malloc error\n");
	data->map->tensor[z] = NULL;
	i = 0;
	while (data->map->tensor[i] != NULL)
	{
		data->map->tensor[i] = get_matrix(data);
		i++;
	}
}*/
