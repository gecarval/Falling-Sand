/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   calls.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/26 20:32:15 by gecarval          #+#    #+#             */
/*   Updated: 2024/08/27 17:31:36 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./includes/renderer.h"

int	exit_data(t_data *data, int fd)
{
	if (data->img->img_ptr != NULL)
		mlx_destroy_image(data->ini, data->img->img_ptr);
	if (data->img != NULL)
		free(data->img);
	if (data->map->mapz != NULL)
		free(data->map->mapz);
	if (data->map->parsed_file != NULL)
		ft_free_matrix(data->map->parsed_file);
	if (data->map != NULL)
		free(data->map);
	if (data->original_map->mapz != NULL)
		free(data->original_map->mapz);
	if (data->original_map != NULL)
		free(data->original_map);
	if (data->win != NULL)
		mlx_clear_window(data->ini, data->win);
	if (data->win != NULL)
		mlx_destroy_window(data->ini, data->win);
	data->win = NULL;
	if (data->ini != NULL)
		mlx_destroy_display(data->ini);
	if (data->ini != NULL)
		free(data->ini);
	if (data != NULL)
		free(data);
	exit(fd);
	return (0);
}

void	display_error(t_data *data, char *msg)
{
	ft_putstr_fd(msg, 2);
	exit_data(data, 1);
}

void	ft_init_program(t_data *data)
{
	data->ini = mlx_init();
	if (!data->ini)
		display_error(data, "init error\n");
	data->win = mlx_new_window(data->ini, WINX, WINY, "render");
	if (!data->win)
		display_error(data, "window error\n");
	data->map = (t_map *)malloc(sizeof(t_map));
	if (!data->map)
		display_error(data, "map malloc error\n");
	data->map->parsed_file = NULL;
	data->map->mapz = NULL;
	data->cam = NULL;
	data->img = (t_img *)malloc(sizeof(t_img));
	if (!data->img)
		display_error(data, "img malloc error\n");
	data->img->img_ptr = mlx_new_image(data->ini, WINX, WINY);
	if (!data->img->img_ptr)
		display_error(data, "img ptr error\n");
	data->img->img_px = mlx_get_data_addr(data->img->img_ptr,
			&data->img->bpp, &data->img->llen, &data->img->end);
	if (!data->img->img_px)
		display_error(data, "img data error\n");
}
