#include "../cub3d.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef struct s_keys
{
	bool	w;
	bool	a;
	bool	s;
	bool	d;
	bool	left;
	bool	right;
}	t_keys;

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

// Function prototypes
void	process_ray(t_game *game, int ray);
void	initialize_ray(t_game *game, int ray, t_ray_data *ray_data);
void	perform_dda(t_game *game, t_ray_data *ray_data);
void	correct_perp_wall_dist(t_game *game, t_ray_data *ray_data);
void	calculate_line_height(t_ray_data *ray_data);
void	choose_wall_texture(t_game *game, t_ray_data *ray_data);
void	calculate_texture_coordinates(t_game *game, t_ray_data *ray_data);
void	draw_wall_slice(t_game *game, int ray, t_ray_data *ray_data);

// Global variables
static t_keys	g_keys = {false, false, false, false, false, false};

// Convert character to integer
int	ft_char_to_int(char c)
{
	return (c - '0');
}

// Convert RGB array to uint32_t color value
uint32_t	rgb_to_uint32(int rgb[3])
{
	return ((rgb[0] << 24) | (rgb[1] << 16) | (rgb[2] << 8) | 255);
}

// Apply fog effect to color based on wall distance
uint32_t	apply_fog_effect(uint32_t color, float perp_wall_dist, \
float max_view_distance)
{
	float	fog_factor;
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
	uint8_t	a;

	fog_factor = perp_wall_dist / max_view_distance;
	if (fog_factor > 1.0f)
		fog_factor = 1.0f;
	r = ((color >> 24) & 0xFF) * (1.0f - fog_factor);
	g = ((color >> 16) & 0xFF) * (1.0f - fog_factor);
	b = ((color >> 8) & 0xFF) * (1.0f - fog_factor);
	a = color & 0xFF;
	return ((r << 24) | (g << 16) | (b << 8) | a);
}

// Initialize a ray for casting
void	init_ray_angle(t_game *game, int ray, float fov_rad, float *ray_angle)
{
	float	angle_increment;
	float	start_angle;

	angle_increment = fov_rad / NUM_RAYS;
	start_angle = game->player.angle - (fov_rad / 2.0f);
	if (start_angle < 0)
		start_angle += 2 * M_PI;
	if (start_angle > 2 * M_PI)
		start_angle -= 2 * M_PI;
	*ray_angle = start_angle + ray * angle_increment;
	if (*ray_angle < 0)
		*ray_angle += 2 * M_PI;
	if (*ray_angle > 2 * M_PI)
		*ray_angle -= 2 * M_PI;
}

void	init_ray_direction(float ray_angle, float *ray_dir_x, float *ray_dir_y)
{
	*ray_dir_x = cos(ray_angle);
	*ray_dir_y = sin(ray_angle);
}

void	calculate_delta_distances(t_ray *ray)
{
	if (ray->ray_dir_x == 0)
		ray->delta_dist_x = 2147483647;
	else
		ray->delta_dist_x = fabs(1.0f / ray->ray_dir_x);
	if (ray->ray_dir_y == 0)
		ray->delta_dist_y = 2147483647;
	else
		ray->delta_dist_y = fabs(1.0f / ray->ray_dir_y);
}

void	calculate_step_x(t_game *game, t_ray *ray)
{
	float	player_x_in_tiles;

	player_x_in_tiles = game->player.x / TILE_SIZE;
	if (ray->ray_dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (player_x_in_tiles - ray->map_x) * ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1.0f - player_x_in_tiles) * \
		ray->delta_dist_x;
	}
}

void	calculate_step_y(t_game *game, t_ray *ray)
{
	float	player_y_in_tiles;

	player_y_in_tiles = game->player.y / TILE_SIZE;
	if (ray->ray_dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (player_y_in_tiles - ray->map_y) * ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0f - player_y_in_tiles) * \
		ray->delta_dist_y;
	}
}

void	calculate_step_and_side_dist(t_game *game, t_ray *ray)
{
	calculate_delta_distances(ray);
	calculate_step_x(game, ray);
	calculate_step_y(game, ray);
}

void	update_ray_position(t_ray *ray)
{
	if (ray->side_dist_x < ray->side_dist_y)
	{
		ray->side_dist_x += ray->delta_dist_x;
		ray->map_x += ray->step_x;
		ray->side = 0;
	}
	else
	{
		ray->side_dist_y += ray->delta_dist_y;
		ray->map_y += ray->step_y;
		ray->side = 1;
	}
}

int	check_out_of_bounds(t_game *game, t_ray *ray)
{
	int	map_width;

	map_width = ft_strlen(game->map_grid->symbols[0]);
	if (ray->map_x < 0 || ray->map_x >= map_width || \
	ray->map_y < 0 || ray->map_y >= game->map_grid->capacity)
	{
		ray->perp_wall_dist = MAX_VIEW_DISTANCE;
		return (2);
	}
	return (0);
}

int	check_wall_hit(t_game *game, t_ray *ray)
{
	char	symbol;

	symbol = game->map_grid->symbols[ray->map_y][ray->map_x];
	if (symbol == '1')
	{
		if (ray->side == 0)
			ray->perp_wall_dist = ray->side_dist_x - ray->delta_dist_x;
		else
			ray->perp_wall_dist = ray->side_dist_y - ray->delta_dist_y;
		return (1);
	}
	return (0);
}

int	perform_dda_loop(t_game *game, t_ray *ray)
{
	int	hit;

	hit = 0;
	ray->perp_wall_dist = 0.0f;
	while (hit == 0)
	{
		update_ray_position(ray);
		hit = check_out_of_bounds(game, ray);
		if (hit != 0)
			break ;
		hit = check_wall_hit(game, ray);
		if (ray->perp_wall_dist > MAX_VIEW_DISTANCE)
		{
			ray->perp_wall_dist = MAX_VIEW_DISTANCE;
			hit = 2;
			break ;
		}
	}
	return (hit);
}

void	perform_dda(t_game *game, t_ray_data *ray_data)
{
	t_ray	ray;
	int		hit;

	ray.ray_dir_x = ray_data->ray_dir_x;
	ray.ray_dir_y = ray_data->ray_dir_y;
	ray.map_x = ray_data->map_x;
	ray.map_y = ray_data->map_y;
	calculate_step_and_side_dist(game, &ray);
	hit = perform_dda_loop(game, &ray);
	ray_data->map_x = ray.map_x;
	ray_data->map_y = ray.map_y;
	ray_data->side = ray.side;
	ray_data->perp_wall_dist = ray.perp_wall_dist;
}

void	correct_and_compute_wall(float *perp_wall_dist, float ray_angle, \
t_game *game, int *line_height)
{
	float	angle_diff;

	angle_diff = ray_angle - game->player.angle;
	if (angle_diff < -M_PI)
		angle_diff += 2 * M_PI;
	if (angle_diff > M_PI)
		angle_diff -= 2 * M_PI;
	*perp_wall_dist *= cos(angle_diff);
	if (*perp_wall_dist < 0.0001f)
		*perp_wall_dist = 0.0001f;
	*line_height = (int)(HEIGHT / (*perp_wall_dist));
	if (*line_height > HEIGHT)
		*line_height = HEIGHT;
	if (*line_height < 1)
		*line_height = 1;
}

void	draw_ceiling_and_floor(mlx_image_t *image, uint32_t ceilingColor, \
uint32_t floorColor)
{
	int	x;
	int	y;

	y = 0;
	while (y < HEIGHT / 2)
	{
		x = 0;
		while (x < WIDTH)
		{
			mlx_put_pixel(image, x, y, ceilingColor);
			x++;
		}
		y++;
	}
	y = HEIGHT / 2;
	while (y < HEIGHT)
	{
		x = 0;
		while (x < WIDTH)
		{
			mlx_put_pixel(image, x, y, floorColor);
			x++;
		}
		y++;
	}
}

uint32_t	retrieve_color_at_coordinates(int x, int y, t_texture_data *texture)
{
	int	index;

	if (x < 0 || x >= texture->width || y < 0 || y >= texture->height)
		return (0x000000FF);
	index = y * texture->width + x;
	return (texture->pixels[index]);
}

void	process_ray(t_game *game, int ray)
{
	t_ray_data	ray_data;

	initialize_ray(game, ray, &ray_data);
	perform_dda(game, &ray_data);
	correct_perp_wall_dist(game, &ray_data);
	calculate_line_height(&ray_data);
	choose_wall_texture(game, &ray_data);
	calculate_texture_coordinates(game, &ray_data);
	draw_wall_slice(game, ray, &ray_data);
}

void	correct_perp_wall_dist(t_game *game, t_ray_data *ray_data)
{
	float	angle_diff;

	angle_diff = ray_data->ray_angle - game->player.angle;
	if (angle_diff < -M_PI)
		angle_diff += 2 * M_PI;
	if (angle_diff > M_PI)
		angle_diff -= 2 * M_PI;
	ray_data->corrected_perp_wall_dist = ray_data->perp_wall_dist * \
	cos(angle_diff);
}

void	calculate_line_height(t_ray_data *ray_data)
{
	ray_data->line_height = (int)(HEIGHT / ray_data->corrected_perp_wall_dist);
	ray_data->draw_start = -ray_data->line_height / 2 + HEIGHT / 2;
	if (ray_data->draw_start < 0)
		ray_data->draw_start = 0;
	ray_data->draw_end = ray_data->line_height / 2 + HEIGHT / 2;
	if (ray_data->draw_end >= HEIGHT)
		ray_data->draw_end = HEIGHT - 1;
}

void	choose_wall_texture(t_game *game, t_ray_data *ray_data)
{
	if (ray_data->side == 0)
	{
		if (ray_data->ray_dir_x < 0)
			ray_data->texture = game->assets.textures.WE;
		else
			ray_data->texture = game->assets.textures.EA;
	}
	else
	{
		if (ray_data->ray_dir_y < 0)
			ray_data->texture = game->assets.textures.SO;
		else
			ray_data->texture = game->assets.textures.NO;
	}
}

void	calculate_texture_position(t_ray_data *ray_data)
{
	ray_data->step = 1.0f * ray_data->texture->height / ray_data->line_height;
	if (ray_data->draw_start > 0)
		ray_data->tex_pos = (ray_data->draw_start - HEIGHT / 2 + \
	ray_data->line_height / 2) * ray_data->step;
	else
		ray_data->tex_pos = (0 - HEIGHT / 2 + ray_data->line_height / 2) * \
	ray_data->step;
}

void	calculate_texture_coordinates(t_game *game, t_ray_data *ray_data)
{
	float	player_x_in_tiles;
	float	player_y_in_tiles;

	player_x_in_tiles = game->player.x / TILE_SIZE;
	player_y_in_tiles = game->player.y / TILE_SIZE;
	if (ray_data->side == 0)
		ray_data->wall_x = player_y_in_tiles + ray_data->perp_wall_dist * \
	ray_data->ray_dir_y;
	else
		ray_data->wall_x = player_x_in_tiles + ray_data->perp_wall_dist * \
	ray_data->ray_dir_x;
	ray_data->wall_x -= floor(ray_data->wall_x);
	ray_data->tex_x = (int)(ray_data->wall_x * ray_data->texture->width);
	if (ray_data->tex_x < 0)
		ray_data->tex_x = 0;
	if (ray_data->tex_x >= ray_data->texture->width)
		ray_data->tex_x = ray_data->texture->width - 1;
	if (ray_data->side == 0 && ray_data->ray_dir_x > 0)
		ray_data->tex_x = ray_data->texture->width - ray_data->tex_x - 1;
	if (ray_data->side == 1 && ray_data->ray_dir_y < 0)
		ray_data->tex_x = ray_data->texture->width - ray_data->tex_x - 1;
	calculate_texture_position(ray_data);
}

void	draw_wall_slice(t_game *game, int ray, t_ray_data *ray_data)
{
	int				y;
	int				tex_y;
	uint32_t		color;
	t_texture_data	texture_data;

	texture_data.pixels = (uint32_t *)ray_data->texture->pixels;
	texture_data.width = ray_data->texture->width;
	texture_data.height = ray_data->texture->height;
	y = ray_data->draw_start;
	while (y < ray_data->draw_end)
	{
		tex_y = (int)ray_data->tex_pos;
		if (tex_y < 0)
			tex_y = 0;
		if (tex_y >= texture_data.height)
			tex_y = texture_data.height - 1;
		ray_data->tex_pos += ray_data->step;
		color = retrieve_color_at_coordinates(ray_data->tex_x, tex_y, \
		&texture_data);
		color = (color << 24) | (((color >> 16) << 24) >> 16) | \
				(((color << 16) >> 24) << 16) | (color >> 24);
		mlx_put_pixel(game->image, ray, y, color);
		y++;
	}
}

void	initialize_ray(t_game *game, int ray, t_ray_data *ray_data)
{
	float	fov_rad;
	float	player_x_in_tiles;
	float	player_y_in_tiles;

	fov_rad = FOV * M_PI / 180.0f;
	init_ray_angle(game, ray, fov_rad, &(ray_data->ray_angle));
	init_ray_direction(ray_data->ray_angle, &(ray_data->ray_dir_x), \
	&(ray_data->ray_dir_y));
	player_x_in_tiles = game->player.x / TILE_SIZE;
	player_y_in_tiles = game->player.y / TILE_SIZE;
	ray_data->map_x = (int)(player_x_in_tiles);
	ray_data->map_y = (int)(player_y_in_tiles);
}

void	cast_rays(t_game *game)
{
	int	ray;

	ray = 0;
	while (ray < NUM_RAYS)
	{
		process_ray(game, ray);
		ray++;
	}
}

int	get_map_width(t_game *game)
{
	int	i;
	int	map_width;
	int	line_length;
	int	map_height;

	i = 0;
	map_width = 0;
	map_height = game->map_grid->capacity;
	while (i < map_height)
	{
		line_length = ft_strlen(game->map_grid->symbols[i]);
		if (line_length > map_width)
			map_width = line_length;
		i++;
	}
	return (map_width);
}

bool	is_walkable(char c)
{
	return (c == '0' || is_player_symbol(c));
}

void	draw_tile(t_game *game, int tile_x, int tile_y, uint32_t color)
{
	int	i;
	int	j;
	int	px;
	int	py;

	i = 0;
	while (i < TILE_SIZE)
	{
		j = 0;
		while (j < TILE_SIZE)
		{
			px = tile_x + i;
			py = tile_y + j;
			if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT)
				mlx_put_pixel(game->image, px, py, color);
			j++;
		}
		i++;
	}
}

void	draw_map(t_game *game)
{
	int			y;
	int			x;
	uint32_t	color;
	int			tile_x;
	int			tile_y;

	y = 0;
	while (y < game->map_grid->capacity)
	{
		x = 0;
		while (x < ft_strlen(game->map_grid->symbols[0]))
		{
			if (game->map_grid->symbols[y][x] == '1')
				color = 0xFFFFFFFF;
			else
				color = 0x000000FF;
			tile_x = x * TILE_SIZE;
			tile_y = y * TILE_SIZE;
			draw_tile(game, tile_x, tile_y, color);
			x++;
		}
		y++;
	}
}

void	draw_square(t_game *game, int center_x, int center_y, int size)
{
	int			x;
	int			y;
	int			px;
	int			py;
	uint32_t	color;

	y = -size;
	while (y <= size)
	{
		x = -size;
		while (x <= size)
		{
			px = center_x + x;
			py = center_y + y;
			if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT)
			{
				mlx_put_pixel(game->image, px, py, color);
			}
			x++;
		}
		y++;
	}
}

void	draw_player(t_game *game)
{
	int	player_x;
	int	player_y;
	int	size;

	player_x = (int)game->player.x;
	player_y = (int)game->player.y;
	size = 5;
	draw_square(game, player_x, player_y, size);
}

void	handle_key_press(mlx_key_data_t keydata, t_game *game)
{
	if (keydata.key == MLX_KEY_W)
		g_keys.w = true;
	else if (keydata.key == MLX_KEY_A)
		g_keys.a = true;
	else if (keydata.key == MLX_KEY_S)
		g_keys.s = true;
	else if (keydata.key == MLX_KEY_D)
		g_keys.d = true;
	else if (keydata.key == MLX_KEY_LEFT)
		g_keys.left = true;
	else if (keydata.key == MLX_KEY_RIGHT)
		g_keys.right = true;
	else if (keydata.key == MLX_KEY_ESCAPE)
	{
		cleanup(game);
		exit(EXIT_SUCCESS);
	}
}

void	handle_key_release(mlx_key_data_t keydata)
{
	if (keydata.key == MLX_KEY_W)
		g_keys.w = false;
	else if (keydata.key == MLX_KEY_A)
		g_keys.a = false;
	else if (keydata.key == MLX_KEY_S)
		g_keys.s = false;
	else if (keydata.key == MLX_KEY_D)
		g_keys.d = false;
	else if (keydata.key == MLX_KEY_LEFT)
		g_keys.left = false;
	else if (keydata.key == MLX_KEY_RIGHT)
		g_keys.right = false;
}

void	key_press(mlx_key_data_t keydata, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (keydata.action == MLX_PRESS)
		handle_key_press(keydata, game);
	else if (keydata.action == MLX_RELEASE)
		handle_key_release(keydata);
}

void	initialize_line_vars(t_line_vars *vars, t_point p0, t_point p1)
{
	vars->dx = abs(p1.x - p0.x);
	vars->dy = -abs(p1.y - p0.y);
	if (p0.x < p1.x)
		vars->sx = 1;
	else
		vars->sx = -1;
	if (p0.y < p1.y)
		vars->sy = 1;
	else
		vars->sy = -1;
	vars->err = vars->dx + vars->dy;
	vars->x1 = p1.x;
	vars->y1 = p1.y;
}

void	update_line_vars(t_line_vars *vars, int *x, int *y)
{
	vars->e2 = 2 * vars->err;
	if (vars->e2 >= vars->dy)
	{
		vars->err += vars->dy;
		*x += vars->sx;
	}
	if (vars->e2 <= vars->dx)
	{
		vars->err += vars->dx;
		*y += vars->sy;
	}
}

void	draw_line_loop(mlx_image_t *image, t_line_vars *vars, t_point p0, \
uint32_t color)
{
	int	x;
	int	y;
	int	width;
	int	height;
	int	done;

	x = p0.x;
	y = p0.y;
	width = image->width;
	height = image->height;
	done = 0;
	while (!done)
	{
		if (x >= 0 && x < width && y >= 0 && y < height)
			mlx_put_pixel(image, x, y, color);
		if (x == vars->x1 && y == vars->y1)
			done = 1;
		else
			update_line_vars(vars, &x, &y);
	}
}

void	draw_line(mlx_image_t *image, t_point p0, t_point p1, uint32_t color)
{
	t_line_vars	vars;

	initialize_line_vars(&vars, p0, p1);
	draw_line_loop(image, &vars, p0, color);
}

bool	can_move_to(float x, float y, t_game *game)
{
	int				map_height;
	int				map_width;
	float			radius;
	t_map_coords	coords;

	map_height = game->map_grid->capacity;
	map_width = get_map_width(game);
	radius = 10.0f;
	coords.map_x1 = (int)((x - radius) / TILE_SIZE);
	coords.map_y1 = (int)((y - radius) / TILE_SIZE);
	coords.map_x2 = (int)((x + radius) / TILE_SIZE);
	coords.map_y2 = (int)((y + radius) / TILE_SIZE);
	if (coords.map_x1 < 0 || coords.map_x2 >= map_width || \
		coords.map_y1 < 0 || coords.map_y2 >= map_height)
		return (false);
	if (!is_walkable(game->map_grid->symbols[coords.map_y1][coords.map_x1]) ||
		!is_walkable(game->map_grid->symbols[coords.map_y1][coords.map_x2]) ||
		!is_walkable(game->map_grid->symbols[coords.map_y2][coords.map_x1]) ||
		!is_walkable(game->map_grid->symbols[coords.map_y2][coords.map_x2]))
		return (false);
	return (true);
}

void	draw_minimap_player_direction(t_game *game, int player_px, \
int player_py, t_minimap *minimap)
{
	float		dir_x;
	float		dir_y;
	t_point		p0;
	t_point		p1;

	dir_x = cos(game->player.angle) * 5 * minimap->scale;
	dir_y = sin(game->player.angle) * 5 * minimap->scale;
	p0.x = player_px;
	p0.y = player_py;
	p1.x = (int)(player_px + dir_x);
	p1.y = (int)(player_py + dir_y);
}

void	draw_minimap_player_square(t_game *game, int player_px, int player_py)
{
	int	i;
	int	j;
	int	px;
	int	py;

	i = -2;
	while (i <= 2)
	{
		j = -2;
		while (j <= 2)
		{
			px = player_px + i;
			py = player_py + j;
			if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT)
				mlx_put_pixel(game->image, px, py, 0xFF0000FF);
			j++;
		}
		i++;
	}
}

void	draw_minimap_player(t_game *game, t_minimap *minimap)
{
	float	player_x;
	float	player_y;
	int		player_px;
	int		player_py;

	player_x = minimap->offset_x + (game->player.x / TILE_SIZE) * \
	minimap->scale;
	player_y = minimap->offset_y + (game->player.y / TILE_SIZE) * \
	minimap->scale;
	player_px = (int)player_x;
	player_py = (int)player_y;
	draw_minimap_player_square(game, player_px, player_py);
	//draw_minimap_player_direction(game, player_px, player_py, minimap);
}

void	initialize_ray_info(t_game *game, float ray_angle, t_ray_info *ray_info)
{
	ray_info->ray_x = game->player.x;
	ray_info->ray_y = game->player.y;
	ray_info->ray_dir_x = cos(ray_angle);
	ray_info->ray_dir_y = sin(ray_angle);
	ray_info->hit = 0;
	ray_info->total_distance = 0.0f;
	ray_info->step_size = 1.0f;
}

void	perform_ray_casting(t_game *game, t_minimap *minimap, \
t_ray_info *ray_info)
{
	while (!ray_info->hit)
	{
		ray_info->ray_x += ray_info->ray_dir_x * ray_info->step_size;
		ray_info->ray_y += ray_info->ray_dir_y * ray_info->step_size;
		ray_info->total_distance += ray_info->step_size;
		ray_info->map_x = (int)(ray_info->ray_x / TILE_SIZE);
		ray_info->map_y = (int)(ray_info->ray_y / TILE_SIZE);
		if (ray_info->map_x < 0 || ray_info->map_x >= minimap->map_width || \
			ray_info->map_y < 0 || ray_info->map_y >= minimap->map_height)
			ray_info->hit = 1;
		else if (game->map_grid->symbols[ray_info->map_y][ray_info->map_x] \
		== '1')
			ray_info->hit = 1;
		else if (ray_info->total_distance >= MAX_VIEW_DISTANCE)
			ray_info->hit = 1;
	}
}

void	compute_end_coordinates(t_minimap *minimap, t_ray_info *ray_info, \
t_point *end_point)
{
	end_point->x = minimap->offset_x + (ray_info->ray_x / \
	TILE_SIZE) * minimap->scale;
	end_point->y = minimap->offset_y + (ray_info->ray_y / \
	TILE_SIZE) * minimap->scale;
}

void	calculate_ray_end(t_game *game, t_minimap *minimap, \
float ray_angle, t_point *end_point)
{
	t_ray_info	ray_info;

	initialize_ray_info(game, ray_angle, &ray_info);
	perform_ray_casting(game, minimap, &ray_info);
	compute_end_coordinates(minimap, &ray_info, end_point);
}

void	process_minimap_ray(t_game *game, t_minimap *minimap, \
float ray_angle)
{
	t_point		end_point;
	t_point		p0;
	t_point		p1;

	calculate_ray_end(game, minimap, ray_angle, &end_point);
	p0.x = minimap->offset_x + (game->player.x / TILE_SIZE) * minimap->scale;
	p0.y = minimap->offset_y + (game->player.y / TILE_SIZE) * minimap->scale;
	p1.x = (int)end_point.x;
	p1.y = (int)end_point.y;
	draw_line(game->image, p0, p1, 0x0000FFFF);
}

void	draw_minimap_rays(t_game *game, t_minimap *minimap)
{
	float	fov_rad;
	float	angle_increment;
	float	start_angle;
	int		ray;
	float	ray_angle;

	fov_rad = FOV * M_PI / 180.0f;
	angle_increment = fov_rad / NUM_RAYS;
	start_angle = game->player.angle - (fov_rad / 2.0f);
	if (start_angle < 0)
		start_angle += 2 * M_PI;
	if (start_angle > 2 * M_PI)
		start_angle -= 2 * M_PI;
	ray = 0;
	while (ray < NUM_RAYS)
	{
		if (ray % 10 == 0)
		{
			ray_angle = start_angle + ray * angle_increment;
			if (ray_angle < 0)
				ray_angle += 2 * M_PI;
			if (ray_angle > 2 * M_PI)
				ray_angle -= 2 * M_PI;
			process_minimap_ray(game, minimap, ray_angle);
		}
		ray++;
	}
}

void	draw_minimap_tile(t_game *game, t_tile *tile)
{
	int	px;
	int	py;

	py = tile->y0;
	while (py < tile->y1)
	{
		px = tile->x0;
		while (px < tile->x1)
		{
			if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT)
			{
				mlx_put_pixel(game->image, px, py, tile->color);
			}
			px++;
		}
		py++;
	}
}

void	process_minimap_tile(t_game *game, t_minimap *minimap, int x, int y)
{
	uint32_t	color;
	t_tile		tile;

	if (game->map_grid->symbols[y][x] == '1')
		color = 0x888888FF;
	else
		color = 0x222222FF;
	tile.x0 = minimap->offset_x + (int)(x * minimap->scale);
	tile.y0 = minimap->offset_y + (int)(y * minimap->scale);
	tile.x1 = minimap->offset_x + (int)((x + 1) * minimap->scale);
	tile.y1 = minimap->offset_y + (int)((y + 1) * minimap->scale);
	tile.color = color;
	draw_minimap_tile(game, &tile);
}

void	draw_minimap_grid(t_game *game, t_minimap *minimap)
{
	int	y;
	int	x;
	int	line_length;

	y = 0;
	while (y < minimap->map_height)
	{
		line_length = ft_strlen(game->map_grid->symbols[y]);
		x = 0;
		while (x < line_length)
		{
			process_minimap_tile(game, minimap, x, y);
			x++;
		}
		y++;
	}
}

void	draw_minimap(t_game *game)
{
	t_minimap	minimap;
	float		scale_x;
	float		scale_y;

	minimap.map_height = game->map_grid->capacity;
	minimap.map_width = get_map_width(game);
	minimap.offset_x = 10;
	minimap.offset_y = 10;
	scale_x = 200.0f / (float)minimap.map_width;
	scale_y = 200.0f / (float)minimap.map_height;
	if (scale_x < scale_y)
		minimap.scale = scale_x;
	else
		minimap.scale = scale_y;
	draw_minimap_grid(game, &minimap);
	draw_minimap_player(game, &minimap);
	draw_minimap_rays(game, &minimap);
}

void	handle_rotation(t_game *game, float rot_speed)
{
	if (g_keys.left)
	{
		game->player.angle -= rot_speed;
		if (game->player.angle < 0)
			game->player.angle += 2 * M_PI;
	}
	if (g_keys.right)
	{
		game->player.angle += rot_speed;
		if (game->player.angle >= 2 * M_PI)
			game->player.angle -= 2 * M_PI;
	}
}

void	calculate_movement(t_game *game, float move_speed, \
float *move_x, float *move_y)
{
	*move_x = 0.0f;
	*move_y = 0.0f;
	if (g_keys.w)
	{
		*move_x += cos(game->player.angle) * move_speed;
		*move_y += sin(game->player.angle) * move_speed;
	}
	if (g_keys.s)
	{
		*move_x -= cos(game->player.angle) * move_speed;
		*move_y -= sin(game->player.angle) * move_speed;
	}
	if (g_keys.a)
	{
		*move_x += cos(game->player.angle - M_PI / 2) * move_speed;
		*move_y += sin(game->player.angle - M_PI / 2) * move_speed;
	}
	if (g_keys.d)
	{
		*move_x += cos(game->player.angle + M_PI / 2) * move_speed;
		*move_y += sin(game->player.angle + M_PI / 2) * move_speed;
	}
}

void	update_position(t_game *game, float move_x, float move_y)
{
	float	magnitude;
	float	new_x;
	float	new_y;
	bool	can_move_x;
	bool	can_move_y;

	magnitude = sqrt(move_x * move_x + move_y * move_y);
	if (magnitude > 0)
	{
		move_x = (move_x / magnitude) * magnitude;
		move_y = (move_y / magnitude) * magnitude;
	}
	new_x = game->player.x + move_x;
	new_y = game->player.y + move_y;
	can_move_x = can_move_to(new_x, game->player.y, game);
	can_move_y = can_move_to(game->player.x, new_y, game);
	if (can_move_x)
		game->player.x = new_x;
	if (can_move_y)
		game->player.y = new_y;
}

void	handle_movement(t_game *game, float move_speed)
{
	float	move_x;
	float	move_y;

	calculate_movement(game, move_speed, &move_x, &move_y);
	update_position(game, move_x, move_y);
}

void	render_frame(t_game *game)
{
	uint32_t	ceiling_color;
	uint32_t	floor_color;

	ceiling_color = rgb_to_uint32(game->assets.colors.rgb_c);
	floor_color = rgb_to_uint32(game->assets.colors.rgb_f);
	draw_ceiling_and_floor(game->image, ceiling_color, floor_color);
	cast_rays(game);
	draw_minimap(game);
}

void	clear_image(mlx_image_t *image, uint32_t color)
{
	uint32_t	*pixels;
	size_t		total_pixels;
	size_t		i;
	size_t		width;
	size_t		height;

	if (!image || !image->pixels)
		return ;
	width = image->width;
	height = image->height;
	pixels = (uint32_t *)image->pixels;
	total_pixels = width * height;
	i = 0;
	while (i < total_pixels)
	{
		pixels[i] = color;
		i++;
	}
}

void	update(void *param)
{
	t_game	*game;
	float	move_speed;
	float	rot_speed;

	game = (t_game *)param;
	clear_image(game->image, 0x000000FF);
	move_speed = 1.0f;
	rot_speed = 2.0f * M_PI / 180.0f;
	handle_rotation(game, rot_speed);
	handle_movement(game, move_speed);
	render_frame(game);
}

int	raycast_engine(t_vector *map, t_player player, t_assets *assets)
{
	t_game	*game;

	game = initialize_game(map, player, assets);
	if (!game)
		return (EXIT_FAILURE);
	mlx_loop_hook(game->mlx, &update, game);
	mlx_key_hook(game->mlx, &key_press, game);
	mlx_loop(game->mlx);
	cleanup(game);
	return (EXIT_SUCCESS);
}
