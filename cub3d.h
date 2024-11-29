// /* ************************************************************************** */
// /*                                                                            */
// /*                                                        :::      ::::::::   */
// /*   cub3d.h                                            :+:      :+:    :+:   */
// /*                                                    +:+ +:+         +:+     */
// /*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
// /*                                                +#+#+#+#+#+   +#+           */
// /*   Created: 2024/11/20 15:15:45 by nkarpilo          #+#    #+#             */
// /*   Updated: 2024/11/29 15:54:57 by istasheu         ###   ########.fr       */
// /*                                                                            */
// /* ************************************************************************** */

// #ifndef CUB3D_H
// # define CUB3D_H

// # include "libft/libft.h"
// # include "MLX42/include/MLX42/MLX42.h"
// # include "../src/vector/vector.h"
// # include <fcntl.h>
// # include <stdlib.h>
// # include <stdio.h>
// # include <math.h>
// # include <stdbool.h>

// # ifndef M_PI
// #  define M_PI 3.14159265358979323846
// # endif

// # define WIDTH 1200
// # define HEIGHT 800
// # define MAX_DEPTH 16
// # define MAX_RENDER_DISTANCE 1000.0f
// # define MAX_VIEW_DISTANCE 600.0f 

// # define FOV 75
// # define NUM_RAYS WIDTH

// # define TILE_SIZE 32

// # define MAX_MAP_LINES 100
// # define MAX_LINE_LENGTH 64
// # define MAX_FILE_NODES 500

// typedef struct s_textures
// {
// 	char				*path_no;
// 	char				*path_so;
// 	char				*path_ea;
// 	char				*path_we;
// 	mlx_texture_t		*no;
// 	mlx_texture_t		*so;
// 	mlx_texture_t		*ea;
// 	mlx_texture_t		*we;
// }	t_textures;

// typedef struct s_colors
// {
// 	int	rgb_c[3];
// 	int	rgb_f[3];
// }	t_colors;

// typedef struct s_assets
// {
// 	t_textures			textures;
// 	t_colors			colors;
// 	char				view_direction;
// 	struct s_game		*game; 
// }	t_assets;

// typedef struct s_player {
// 	float	x;
// 	float	y;
// 	float	angle;
// }	t_player;

// typedef enum e_line_type
// {
// 	LINE_TOP,
// 	LINE_BOTTOM,
// 	LINE_MIDDLE
// }	t_line_type;

// typedef struct s_keys
// {
// 	bool	w;
// 	bool	a;
// 	bool	s;
// 	bool	d;
// 	bool	left;
// 	bool	right;
// }	t_keys;

// typedef struct s_game {
// 	mlx_t		*mlx;
// 	mlx_image_t	*image;
// 	t_player	player;
// 	t_vector	*map_grid;
// 	t_assets	assets;
// 	t_keys		keys;
// }	t_game;

// typedef struct s_assets
// {
//     t_textures          textures;
//     t_colors            colors;
//     char                view_direction;
//     struct s_game       *game; 
// }   t_assets;

// typedef struct s_map_coords
// {
// 	int	map_x1;
// 	int	map_y1;
// 	int	map_x2;
// 	int	map_y2;
// }	t_map_coords;

// typedef struct s_minimap
// {
// 	int		map_height;
// 	int		map_width;
// 	float	scale;
// 	int		offset_x;
// 	int		offset_y;
// }	t_minimap;

// typedef struct s_tile
// {
// 	int			x0;
// 	int			y0;
// 	int			x1;
// 	int			y1;
// 	uint32_t	color;
// }	t_tile;

// typedef struct s_point
// {
// 	int	x;
// 	int	y;
// }	t_point;

// typedef struct s_line_vars
// {
// 	int	dx;
// 	int	dy;
// 	int	sx;
// 	int	sy;
// 	int	err;
// 	int	e2;
// 	int	x1;
// 	int	y1;
// }	t_line_vars;

// typedef struct s_ray
// {
// 	float	ray_dir_x;
// 	float	ray_dir_y;
// 	int		map_x;
// 	int		map_y;
// 	float	delta_dist_x;
// 	float	delta_dist_y;
// 	float	side_dist_x;
// 	float	side_dist_y;
// 	int		step_x;
// 	int		step_y;
// 	int		side;
// 	float	perp_wall_dist;
// }	t_ray;

// typedef struct s_ray_data
// {
// 	float			ray_angle;
// 	float			ray_dir_x;
// 	float			ray_dir_y;
// 	int				map_x;
// 	int				map_y;
// 	int				side;
// 	float			perp_wall_dist;
// 	float			corrected_perp_wall_dist;
// 	int				line_height;
// 	int				draw_start;
// 	int				draw_end;
// 	mlx_texture_t	*texture;
// 	float			wall_x;
// 	int				tex_x;
// 	float			step;
// 	float			tex_pos;
// }	t_ray_data;

// typedef struct s_ray_info
// {
// 	float	ray_x;
// 	float	ray_y;
// 	float	ray_dir_x;
// 	float	ray_dir_y;
// 	int		hit;
// 	float	total_distance;
// 	float	step_size;
// 	int		map_x;
// 	int		map_y;
// }	t_ray_info;

// typedef struct s_texture_data
// {
// 	uint32_t	*pixels;
// 	int			width;
// 	int			height;
// }	t_texture_data;

// /* allocate.c */
// t_game		*initialize_game(t_vector *map, t_player player, t_assets *assets);

// /* boundary_lines_validation.c */
// bool		is_valid_boundaries(t_vector *map);

// /* config_order_validation.c */
// void		validate_map_file_structure(t_vector *map);

// /* error_handling.c */
// void		log_error_message(char *msg);
// void		error_exit_cleanup(char *msg, t_vector *map, t_assets *assets);
// t_player	set_player_not_found(char *error_message);
// bool		is_valid_line_len_and_nl(const char *line, t_line_type line_type);
// bool		is_valid_line_characters(const char *line,
// 				const char *valid_chars, t_line_type line_type);

// /* flood_fill.c */
// void		validate_map_with_flood_fill(t_vector *map);

// /* free.c */
// void		free_assets_struct(t_assets *assets);
// void		free_split_rgb_array(char **rgb_arr);
// void		free_split_rgb_array(char **rgb_arr);

// /* cleanup.c */
// void		cleanup(t_game *game);

// /* main.c */
// int			main(int argc, char **argv);

// /* map_validation.c */
// bool		is_cub(const char *str);
// void		space_to_wall(t_vector *map);
// bool		validate_map(t_vector *map, t_assets *assets);

// /* parser_config.c */
// t_assets	*initialize_assets(t_vector *map);

// /* parser_config_rgb.c */
// char		*get_rgb(const char *id, t_vector *map, t_assets *assets);

// /* player_position.c */
// int			count_players(t_vector *map);
// t_player	find_player_pos(t_vector *map);

// /*read_map.c */
// t_vector	*read_map(char **argv);

// /* utils.c */
// char		*trim_and_extract(char *str, unsigned int prefix_len);
// bool		is_empty_string(const char *line);
// int			remove_empty_line(t_vector *map);
// bool		validate_map_presence(t_vector *map);
// bool		is_blank_line(const char *str);

// /* utils_symbol.c */
// float		assign_player_angle(char symbol);
// bool		is_player_symbol(char c);
// bool		is_valid_map_symbol(char c);
// bool		check_file_height(t_vector *map);
// void		initialize_keys(t_keys *keys);

// // raycasting.c
// void		initialize_ray(t_game *game, int ray, t_ray_data *ray_data);
// void		perform_dda(t_game *game, t_ray_data *ray_data);
// void		correct_perp_wall_dist(t_game *game, t_ray_data *ray_data);
// void		calculate_line_height(t_ray_data *ray_data);
// void		choose_wall_texture(t_game *game, t_ray_data *ray_data);

// // ray_utils.c
// void		init_ray_angle(t_game *game, int ray, float fov_rad,
// 				float *ray_angle);
// void		init_ray_direction(float ray_angle, float *ray_dir_x,
// 				float *ray_dir_y);
// void		calculate_delta_distances(t_ray *ray);
// void		calculate_step_and_side_dist(t_game *game, t_ray *ray);
// void		correct_and_compute_wall(float *perp_wall_dist, float ray_angle,
// 				t_game *game, int *line_height);

// // draw.c
// void		draw_ceiling_and_floor(mlx_image_t *image, uint32_t ceilingColor,
// 				uint32_t floorColor);
// void		draw_square(t_game *game, int center_x, int center_y, int size);
// void		draw_wall_slice(t_game *game, int ray, t_ray_data *ray_data);
// void		draw_tile(t_game *game, int tile_x, int tile_y, uint32_t color);
// void		draw_map(t_game *game);

// // movement.c
// void		handle_rotation(t_game *game, float rot_speed);
// void		calculate_movement(t_game *game, float move_speed, float *move_x,
// 				float *move_y);
// void		update_position(t_game *game, float move_x, float move_y);
// void		handle_movement(t_game *game, float move_speed);
// bool		can_move_to(float x, float y, t_game *game);

// // minimap.c
// void		draw_minimap(t_game *game);
// void		draw_minimap_grid(t_game *game, t_minimap *minimap);
// void		draw_minimap_player(t_game *game, t_minimap *minimap);
// void		draw_minimap_rays(t_game *game, t_minimap *minimap, int ray);
// void		process_minimap_ray(t_game *game, t_minimap *minimap,
// 				float ray_angle);

// // dda.c
// int			perform_dda_loop(t_game *game, t_ray *ray);
// int			check_out_of_bounds(t_game *game, t_ray *ray);
// int			check_wall_hit(t_game *game, t_ray *ray);
// void		update_ray_position(t_ray *ray);

// // input.c
// void		handle_key_press(mlx_key_data_t keydata, t_game *game);
// void		handle_key_release(mlx_key_data_t keydata, t_game *game);
// void		key_press(mlx_key_data_t keydata, void *param);

// // line.c
// void		initialize_line_vars(t_line_vars *vars, t_point p0, t_point p1);
// void		update_line_vars(t_line_vars *vars, int *x, int *y);
// void		draw_line_loop(mlx_image_t *image, t_line_vars *vars, t_point p0,
// 				uint32_t color);
// void		draw_line(mlx_image_t *image, t_point p0, t_point p1,
// 				uint32_t color);
// void		process_minimap_tile(t_game *game, t_minimap *minimap, int x,
// 				int y);

// // rendering.c
// void		render_frame(t_game *game);
// void		clear_image(mlx_image_t *image, uint32_t color);
// void		cast_rays(t_game *game);
// void		draw_minimap_tile(t_game *game, t_tile *tile);

// // ray_info.c
// void		initialize_ray_info(t_game *game, float ray_angle,
// 				t_ray_info *ray_info);
// void		perform_ray_casting(t_game *game, t_minimap *minimap,
// 				t_ray_info *ray_info);
// void		compute_end_coordinates(t_minimap *minimap, t_ray_info *ray_info,
// 				t_point *end_point);
// void		calculate_ray_end(t_game *game, t_minimap *minimap, float ray_angle,
// 				t_point *end_point);

// // game.c
// int			raycast_engine(t_vector *map, t_player player, t_assets *assets);
// void		update(void *param);
// uint32_t	retrieve_color_at_coordinates(int x, int y,
// 				t_texture_data *texture);

// // aux_funcs.c
// int			get_map_width(t_game *game);
// bool		is_walkable(char c);
// void		calculate_texture_coordinates(t_game *game, t_ray_data *ray_data);
// void		calculate_texture_position(t_ray_data *ray_data);

// // aux_funcs_2.c
// void		calculate_step_x(t_game *game, t_ray *ray);
// void		calculate_step_y(t_game *game, t_ray *ray);
// uint32_t	rgb_to_uint32(int rgb[3]);
// void		process_ray(t_game *game, int ray);
// void		draw_minimap_player_square(t_game *game, int player_px,
// 				int player_py);

// // utils_split.c
// bool		validate_comma_count(const char *str, size_t expected_count);

// #endif


#ifndef CUB3D_H
# define CUB3D_H

# include "libft/libft.h"
# include "MLX42/include/MLX42/MLX42.h"
# include "../src/vector/vector.h"
# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <stdbool.h>

# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif

# define WIDTH 1200
# define HEIGHT 800
# define MAX_DEPTH 16
# define MAX_RENDER_DISTANCE 1000.0f
# define MAX_VIEW_DISTANCE 600.0f 

# define FOV 75
# define NUM_RAYS WIDTH

# define TILE_SIZE 32

# define MAX_MAP_LINES 100
# define MAX_LINE_LENGTH 64
# define MAX_FILE_NODES 500

typedef struct s_textures
{
	char				*path_no;
	char				*path_so;
	char				*path_ea;
	char				*path_we;
	mlx_texture_t		*no;
	mlx_texture_t		*so;
	mlx_texture_t		*ea;
	mlx_texture_t		*we;
}	t_textures;

typedef struct s_colors
{
	int	rgb_c[3];
	int	rgb_f[3];
}	t_colors;

typedef struct s_assets
{
	t_textures			textures;
	t_colors			colors;
	char				view_direction;
	struct s_game		*game; 
}	t_assets;

typedef struct s_player {
	float	x;
	float	y;
	float	angle;
}	t_player;

typedef enum e_line_type
{
	LINE_TOP,
	LINE_BOTTOM,
	LINE_MIDDLE
}	t_line_type;

typedef struct s_keys
{
	bool	w;
	bool	a;
	bool	s;
	bool	d;
	bool	left;
	bool	right;
}	t_keys;

typedef struct s_game {
	mlx_t		*mlx;
	mlx_image_t	*image;
	t_player	player;
	t_vector	*map_grid;
	t_assets	*assets;
	t_keys		keys;
}	t_game;

typedef struct s_map_coords
{
	int	map_x1;
	int	map_y1;
	int	map_x2;
	int	map_y2;
}	t_map_coords;

typedef struct s_minimap
{
	int		map_height;
	int		map_width;
	float	scale;
	int		offset_x;
	int		offset_y;
}	t_minimap;

typedef struct s_tile
{
	int			x0;
	int			y0;
	int			x1;
	int			y1;
	uint32_t	color;
}	t_tile;

typedef struct s_point
{
	int	x;
	int	y;
}	t_point;

typedef struct s_line_vars
{
	int	dx;
	int	dy;
	int	sx;
	int	sy;
	int	err;
	int	e2;
	int	x1;
	int	y1;
}	t_line_vars;

typedef struct s_ray
{
	float	ray_dir_x;
	float	ray_dir_y;
	int		map_x;
	int		map_y;
	float	delta_dist_x;
	float	delta_dist_y;
	float	side_dist_x;
	float	side_dist_y;
	int		step_x;
	int		step_y;
	int		side;
	float	perp_wall_dist;
}	t_ray;

typedef struct s_ray_data
{
	float			ray_angle;
	float			ray_dir_x;
	float			ray_dir_y;
	int				map_x;
	int				map_y;
	int				side;
	float			perp_wall_dist;
	float			corrected_perp_wall_dist;
	int				line_height;
	int				draw_start;
	int				draw_end;
	mlx_texture_t	*texture;
	float			wall_x;
	int				tex_x;
	float			step;
	float			tex_pos;
}	t_ray_data;

typedef struct s_ray_info
{
	float	ray_x;
	float	ray_y;
	float	ray_dir_x;
	float	ray_dir_y;
	int		hit;
	float	total_distance;
	float	step_size;
	int		map_x;
	int		map_y;
}	t_ray_info;

typedef struct s_texture_data
{
	uint32_t	*pixels;
	int			width;
	int			height;
}	t_texture_data;

/* allocate.c */
t_game		*initialize_game(t_vector *map, t_player player, t_assets *assets);

/* boundary_lines_validation.c */
bool		is_valid_boundaries(t_vector *map);

/* config_order_validation.c */
void		validate_map_file_structure(t_vector *map);

/* error_handling.c */
void		log_error_message(char *msg);
void		error_exit_cleanup(char *msg, t_vector *map, t_assets *assets);
t_player	set_player_not_found(char *error_message);
bool		is_valid_line_len_and_nl(const char *line, t_line_type line_type);
bool		is_valid_line_characters(const char *line,
				const char *valid_chars, t_line_type line_type);

/* flood_fill.c */
void		validate_map_with_flood_fill(t_vector *map);

/* free.c */
void		free_assets_struct(t_assets *assets);
void		free_split_rgb_array(char **rgb_arr);
void		free_split_rgb_array(char **rgb_arr);

/* cleanup.c */
void		cleanup(t_game *game);

/* main.c */
int			main(int argc, char **argv);

/* map_validation.c */
bool		is_cub(const char *str);
void		space_to_wall(t_vector *map);
bool		validate_map(t_vector *map, t_assets *assets);

/* parser_config.c */
t_assets	*initialize_assets(t_vector *map);

/* parser_config_rgb.c */
char		*get_rgb(const char *id, t_vector *map, t_assets *assets);

/* player_position.c */
int			count_players(t_vector *map);
t_player	find_player_pos(t_vector *map);

/* read_map.c */
t_vector	*read_map(char **argv);

/* utils.c */
char		*trim_and_extract(char *str, unsigned int prefix_len);
bool		is_empty_string(const char *line);
int			remove_empty_line(t_vector *map);
bool		validate_map_presence(t_vector *map);
bool		is_blank_line(const char *str);

/* utils_symbol.c */
float		assign_player_angle(char symbol);
bool		is_player_symbol(char c);
bool		is_valid_map_symbol(char c);
bool		check_file_height(t_vector *map);
void		initialize_keys(t_keys *keys);

// raycasting.c
void		initialize_ray(t_game *game, int ray, t_ray_data *ray_data);
void		perform_dda(t_game *game, t_ray_data *ray_data);
void		correct_perp_wall_dist(t_game *game, t_ray_data *ray_data);
void		calculate_line_height(t_ray_data *ray_data);
void		choose_wall_texture(t_game *game, t_ray_data *ray_data);

// ray_utils.c
void		init_ray_angle(t_game *game, int ray, float fov_rad,
				float *ray_angle);
void		init_ray_direction(float ray_angle, float *ray_dir_x,
				float *ray_dir_y);
void		calculate_delta_distances(t_ray *ray);
void		calculate_step_and_side_dist(t_game *game, t_ray *ray);
void		correct_and_compute_wall(float *perp_wall_dist, float ray_angle,
				t_game *game, int *line_height);

// draw.c
void		draw_ceiling_and_floor(mlx_image_t *image, uint32_t ceilingColor,
				uint32_t floorColor);
void		draw_square(t_game *game, int center_x, int center_y, int size);
void		draw_wall_slice(t_game *game, int ray, t_ray_data *ray_data);
void		draw_tile(t_game *game, int tile_x, int tile_y, uint32_t color);
void		draw_map(t_game *game);

// movement.c
void		handle_rotation(t_game *game, float rot_speed);
void		calculate_movement(t_game *game, float move_speed, float *move_x,
				float *move_y);
void		update_position(t_game *game, float move_x, float move_y);
void		handle_movement(t_game *game, float move_speed);
bool		can_move_to(float x, float y, t_game *game);

// minimap.c
void		draw_minimap(t_game *game);
void		draw_minimap_grid(t_game *game, t_minimap *minimap);
void		draw_minimap_player(t_game *game, t_minimap *minimap);
void		draw_minimap_rays(t_game *game, t_minimap *minimap, int ray);
void		process_minimap_ray(t_game *game, t_minimap *minimap,
				float ray_angle);

// dda.c
int			perform_dda_loop(t_game *game, t_ray *ray);
int			check_out_of_bounds(t_game *game, t_ray *ray);
int			check_wall_hit(t_game *game, t_ray *ray);
void		update_ray_position(t_ray *ray);

// input.c
void		handle_key_press(mlx_key_data_t keydata, t_game *game);
void		handle_key_release(mlx_key_data_t keydata, t_game *game);
void		key_press(mlx_key_data_t keydata, void *param);

// line.c
void		initialize_line_vars(t_line_vars *vars, t_point p0, t_point p1);
void		update_line_vars(t_line_vars *vars, int *x, int *y);
void		draw_line_loop(mlx_image_t *image, t_line_vars *vars, t_point p0,
				uint32_t color);
void		draw_line(mlx_image_t *image, t_point p0, t_point p1,
				uint32_t color);
void		process_minimap_tile(t_game *game, t_minimap *minimap, int x,
				int y);

// rendering.c
void		render_frame(t_game *game);
void		clear_image(mlx_image_t *image, uint32_t color);
void		cast_rays(t_game *game);
void		draw_minimap_tile(t_game *game, t_tile *tile);

// ray_info.c
void		initialize_ray_info(t_game *game, float ray_angle,
				t_ray_info *ray_info);
void		perform_ray_casting(t_game *game, t_minimap *minimap,
				t_ray_info *ray_info);
void		compute_end_coordinates(t_minimap *minimap, t_ray_info *ray_info,
				t_point *end_point);
void		calculate_ray_end(t_game *game, t_minimap *minimap, float ray_angle,
				t_point *end_point);

// game.c
int			raycast_engine(t_vector *map, t_player player, t_assets *assets);
void		update(void *param);
uint32_t	retrieve_color_at_coordinates(int x, int y,
				t_texture_data *texture);

// aux_funcs.c
int			get_map_width(t_game *game);
bool		is_walkable(char c);
void		calculate_texture_coordinates(t_game *game, t_ray_data *ray_data);
void		calculate_texture_position(t_ray_data *ray_data);

// aux_funcs_2.c
void		calculate_step_x(t_game *game, t_ray *ray);
void		calculate_step_y(t_game *game, t_ray *ray);
uint32_t	rgb_to_uint32(int rgb[3]);
void		process_ray(t_game *game, int ray);
void		draw_minimap_player_square(t_game *game, int player_px,
				int player_py);

// utils_split.c
bool		validate_comma_count(const char *str, size_t expected_count);

#endif
