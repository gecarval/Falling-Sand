/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   renderer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 17:05:06 by gecarval          #+#    #+#             */
/*   Updated: 2024/08/29 21:04:19 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDERER_H
# define RENDERER_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <math.h>
# include <X11/X.h>
# include <X11/keysym.h>
# include "../minilibx-linux/mlx.h"
# include "../libft/libft.h"

# define WINX 800
# define WINY 600
# define ESC 65307 
# define ISO 0.6153

// STRUCTS

typedef struct s_objinf
{
	float_t	pox;
	float_t	poy;
	float_t	sx;
	float_t	sy;
	float_t	color;
}	t_objinf;

typedef struct s_pos
{
	float_t	x;
	float_t	y;
}	t_pos;

typedef struct s_camera
{
	t_pos	pos;
	t_pos	dir;
	t_pos	x_dir;
	t_pos	plane;
}	t_camera;

typedef struct s_delta
{
	float_t	ini;
	float_t	fin;
}	t_delta;

typedef struct s_pt
{
	float_t	x;
	float_t	y;
	float_t	z;
	int	color;
}	t_pt;

typedef struct s_matrix3x3
{
	float_t	c1r1;
	float_t	c1r2;
	float_t	c1r3;
	float_t	c2r1;
	float_t	c2r2;
	float_t	c2r3;
	float_t	c3r1;
	float_t	c3r2;
	float_t	c3r3;
}	t_matrix3x3;	

typedef struct s_map
{
	char	**parsed_file;
	t_pt	*mapz;
	int		space;
	int		min_x;
	int		max_x;
	int		min_y;
	int		max_y;
	int		min_z;
	int		max_z;
	t_matrix3x3	*rotx;
	t_matrix3x3	*roty;
	t_matrix3x3	*rotz;
	t_matrix3x3	*prj;
}	t_map;

typedef struct s_img
{
	void	*img_ptr;
	char	*img_px;
	int		end;
	int		llen;
	int		bpp;
}	t_img;

typedef struct s_data
{
	void		*ini;
	void		*win;
	t_camera	*cam;
	t_img		*img;
	t_map		*map;
	t_map		*original_map;
	float_t		iso;
	float_t		anispeed;
	float_t		dist;
	int		ani;
}	t_data;

// DELTAS
t_delta	revdel(t_delta a);
t_delta	retdel(float_t ini, float_t fin);
void	defdel(t_delta *a, float_t ini, float_t fin);
void	def_objinf(t_objinf *a, t_delta x, t_delta y, int color);
void	offset_scale(t_objinf *a, int scale);
void	draw_rectang(t_objinf a, t_data *data);
void	offset_scale_cur(t_pt *a, int scale);
t_pt	vec_add(t_pt pt1, t_pt pt2);
t_pt	vec_sub(t_pt pt1, t_pt pt2);
t_pt	mtvc_mult(t_matrix3x3 mat, t_pt pt);
t_pt	mat_mult(t_matrix3x3 mat, t_matrix3x3 pt);
// WATER MARKS
void	water_mark(t_data *data);
//PIXEL PUTS
void	pixel_to_img(float_t x, float_t y, t_data *data, int color);
void	render_background(t_data *data, int color);
void	draw_line_y(t_delta x, t_delta y, t_data *data, int color);
void	draw_line_x(t_delta x, t_delta y, t_data *data, int color);
void	draw_line(t_delta x, t_delta y, t_data *data, int color);
void	draw_vertices(t_data *data, float_t ang);
// MAP READ
void	set_height_range(t_map *map, t_pt *cur);
void	fill_map(t_data *data, t_map *map);
void	backup_map(t_data *data);
void	fake_map(t_map *map);
void	create_mapz(t_data *data);
char	**get_matrix(t_data *data);
char	*get_vector(t_data *data);
int		get_y_size(const char *s);
int		get_x_size(const char *s);
// PROCESS CALLS
void	ft_free_tensor(char ***z);
void	ft_free_matrix(char **vec);
void	display_error(t_data *data, char *msg);
void	ft_init_program(t_data *data);
void	create_tensor(t_data *data);
int		exit_data(t_data *data, int fd);
// HOOKS
int		mlx_cooked(int	key, t_data *data);
int		mlx_anim(t_data *data);

#endif
