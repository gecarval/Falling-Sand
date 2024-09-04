/*                                                        :::      ::::::::   */
/*   renderer.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: gecarval <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/21 17:05:06 by gecarval          #+#    #+#             */
/*   Updated: 2024/09/02 14:34:58 by gecarval         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RENDERER_H
# define RENDERER_H

// LIBRARIES
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <fcntl.h>
# include <time.h>
# include <math.h>
# include <aio.h>
# include <X11/X.h>
# include <X11/keysym.h>
# include "../minilibx-linux/mlx.h"
# include "../libft/libft.h"

// WINDOW MACROS
# define WINX (int)800
# define WINY (int)600
# define ESC 65307 
# define ISO 0.6153

// PARTICLES
#define MAT_ID_EMPTY		(char)48
#define MAT_ID_STEAM		(char)49
#define MAT_ID_PROPANE		(char)50
#define MAT_ID_SMOKE		(char)51
#define MAT_ID_EMBER		(char)52
#define MAT_ID_FIRE		(char)53
#define MAT_ID_SALT		(char)54
#define MAT_ID_ACID		(char)55
#define MAT_ID_OIL		(char)56
#define MAT_ID_OILF		(char)57
#define MAT_ID_WATER		(char)58
#define MAT_ID_LAVA		(char)59
#define MAT_ID_GUNPOWDER	(char)60
#define MAT_ID_SAND		(char)61
#define MAT_ID_STONE		(char)62
#define MAT_ID_WOOD		(char)63
#define MAT_ID_WOODF		(char)64
#define MAT_ID_WIND		(char)65

// COLOR
#define MAT_COL_EMPTY      0x000000
#define MAT_COL_SAND       0xC2B280
#define MAT_COL_SALT       0xC8B4BE
#define MAT_COL_WATER      0x1464AA
#define MAT_COL_STONE      0x787078
#define MAT_COL_WOOD       0xA1662F
#define MAT_COL_WOODF      0xFF662F
#define MAT_COL_FIRE       0x961400
#define MAT_COL_FIREGLOW   0x400000
#define MAT_COL_SMOKE      0x323232
#define MAT_COL_PROPANE    0xDDFFDD
#define MAT_COL_EMBER      0xC87814
#define MAT_COL_STEAM      0xDCDCFB
#define MAT_COL_GUNPOWDER  0x3C3C3C
#define MAT_COL_OIL        0x50463C
#define MAT_COL_OILF       0xA0463C
#define MAT_COL_LAVA       0xCF1020
#define MAT_COL_ACID       0x5AC83C

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

typedef struct s_part
{
	float_t	life;
	t_pt	vel;
	t_pt	pos;
	int	id;
	int		updt;
	int		color;
}	t_part;

typedef struct s_fluidsim
{
	char	**map;
	float_t		grav;
	int		visc;
	int		max_x;
	int		max_y;
}	t_fluidsim;

typedef struct s_data
{
	void		*ini;
	void		*win;
	t_camera	*cam;
	t_img		*img;
	t_map		*map;
	t_map		*original_map;
	t_fluidsim	*fsim;
	float_t		iso;
	float_t		anispeed;
	float_t		dist;
	int		anicub;
	int		anifsim;
	int		click_fill;
	int		click_hold;
	int		mposx;
	int		mposy;
	int		timing;
	int		brush_size;
}	t_data;

// DELTAS
t_delta	revdel(t_delta a);
t_delta	retdel(float_t ini, float_t fin);
t_pt	*pt_dup(t_pt *a);
void	defdel(t_delta *a, float_t ini, float_t fin);
//OBJS
void	def_objinf(t_objinf *a, t_delta x, t_delta y, int color);
void	offset_scale(t_objinf *a, int scale);
void	draw_rectang(t_objinf a, t_data *data);
void	offset_scale_cur(t_pt *a, int scale);
void	print_matrix(char **vec);
// MATRIX
t_pt	vec_add(t_pt pt1, t_pt pt2);
t_pt	vec_sub(t_pt pt1, t_pt pt2);
t_pt	mtvc_mult(t_matrix3x3 mat, t_pt pt);
t_pt	mat_mult(t_matrix3x3 mat, t_matrix3x3 pt);
// WATER MARKS
void	water_mark(t_data *data);
void	controls_mark(t_data *data);
//PIXEL PUTS
void	pixel_to_img(int x, int y, t_data *data, int color);
void	pixel_to_img_float(float_t x, float_t y, t_data *data, int color);
void	render_background(t_data *data, int color);
void	draw_line_y(t_delta x, t_delta y, t_data *data, int color);
void	draw_line_x(t_delta x, t_delta y, t_data *data, int color);
void	draw_line(t_delta x, t_delta y, t_data *data, int color);
//CUBE
void	draw_vertices(t_data *data, float_t ang);
//FLUIDSIM
void	render_fluidmap(t_data *data);
void	fluidsim_start(t_data *data);
void	put_mat(int x, int y, t_data *data);
void	process_gravity_r(t_data *data);
void	process_gravity(t_data *data);
t_pt	find_id(int x, int y, t_data *data, char c);
// MATS BEHAVIOR
void	emulate_water(int x, int y, t_data *data, char c);
void	emulate_steam(int x, int y, t_data *data, char c);
void	emulate_lava(int x, int y, t_data *data, char c);
void	emulate_acid(int x, int y, t_data *data, char c);
void	emulate_wood(int x, int y, t_data *data);
void	emulate_woodf(int x, int y, t_data *data);
void	emulate_oil(int x, int y, t_data *data, char c);
void	emulate_oilf(int x, int y, t_data *data, char c);
int		emulate_gas(int x, int y, t_data *data);
int		emulate_fire(int x, int y, t_data *data);
int		emulate_sand(int x, int y, t_data *data, int randed);
int		emulate_propane(int x, int y, t_data *data);
// MAP READ
void	set_height_range(t_map *map, t_pt *cur);
void	fill_map(t_data *data, t_map *map);
void	backup_map(t_data *data);
void	fake_map(t_map *map);
void	create_mapz(t_data *data);
void	create_fsim(t_data *data);
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
int		mouse_click(int key, int x, int y, t_data *data);
int		mouse_released(int key, t_data *data);
// PARTICLE CREATE
t_part particle_empty();
t_part particle_sand();
t_part particle_water();
t_part particle_salt();
t_part particle_wood();
t_part particle_fire();
t_part particle_lava();
t_part particle_smoke();
t_part particle_ember();
t_part particle_steam();
t_part particle_gunpowder();
t_part particle_oil();
t_part particle_stone();
t_part particle_acid();

#endif
