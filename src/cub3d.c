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
}t_ray_data;

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

void	draw_line(mlx_image_t *image, int x0, int y0, int x1, int y1, uint32_t color)
{
	int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
	int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
	int err = dx + dy, e2;

	while (1)
	{
			if (x0 >= 0 && x0 < image->width && y0 >= 0 && y0 < image->height)
					mlx_put_pixel(image, x0, y0, color);

			if (x0 == x1 && y0 == y1)
					break;

			e2 = 2 * err;
			if (e2 >= dy)
			{
					err += dy;
					x0 += sx;
			}
			if (e2 <= dx)
			{
					err += dx;
					y0 += sy;
			}
	}
}

void clear_image(mlx_image_t *image, uint32_t color)
{
	if (!image || !image->pixels)
		return;

	uint32_t *pixels = (uint32_t *)image->pixels;
	size_t total_pixels = WIDTH * HEIGHT;

	for (size_t i = 0; i < total_pixels; i++)
	{
		pixels[i] = color;
	}
}

static bool is_walkable(char c)
{
	return (c == '0' || is_player_symbol(c));
}


bool can_move_to(float x, float y, t_game *game)
{
		// int map_height = game->map_grid->capacity; // Ensure this reflects the actual number of rows
		// int map_width = ft_strlen(game->map_grid->symbols[0]);
		int map_height = game->map_grid->capacity;
		int map_width = 0;

		int i = 0;
		while (i < map_height)
		{
			int line_length = ft_strlen(game->map_grid->symbols[i]) - 1;
			if (line_length > map_width)
					map_width = line_length;
			i++;
		}
		float radius = 10.0f; // Adjust the radius based on TILE_SIZE

		int mapX1 = (int)((x - radius) / TILE_SIZE);
		int mapY1 = (int)((y - radius) / TILE_SIZE);
		int mapX2 = (int)((x + radius) / TILE_SIZE);
		int mapY2 = (int)((y + radius) / TILE_SIZE);

		// Debugging: Print the map coordinates being checked
		//printf("Checking movement to (%.2f, %.2f): mapX1=%d, mapY1=%d, mapX2=%d, mapY2=%d\n",
					//	x, y, mapX1, mapY1, mapX2, mapY2);

		// Check for out-of-bounds
		if (mapX1 < 0 || mapX2 >= map_width || mapY1 < 0 || mapY2 >= map_height)
		{
				printf("Movement blocked: Out of bounds\n");
				return false;
		}

		// Check all corners of the player's bounding box
	if (!is_walkable(game->map_grid->symbols[mapY1][mapX1]) ||
		!is_walkable(game->map_grid->symbols[mapY1][mapX2]) ||
		!is_walkable(game->map_grid->symbols[mapY2][mapX1]) ||
		!is_walkable(game->map_grid->symbols[mapY2][mapX2]))
	{
		printf("Movement blocked: Collision detected\n");
		return false;
	}

		return true;
}

void drawMinimap(t_game *game)
{
	// Desired minimap dimensions (in pixels)
	int minimap_width = 200; // Adjust as needed
	int minimap_height = 200; // Adjust as needed

	// Minimap position on the screen
	int minimap_offset_x = 10; // Position from the left edge
	int minimap_offset_y = 10; // Position from the top edge

 		int map_height = game->map_grid->capacity;
		int map_width = 0;

		int i = 0;
		while (i < map_height)
		{
			int line_length = ft_strlen(game->map_grid->symbols[i]) - 1;
			if (line_length > map_width)
					map_width = line_length;
			i++;
		}
	// Calculate scale factors
	float scaleX = (float)minimap_width / (float)map_width;
	float scaleY = (float)minimap_height / (float)map_height;
	float scale = (scaleX < scaleY) ? scaleX : scaleY;

	// Draw the map grid
	for (int y = 0; y < map_height; y++)
	{ 
			int line_length = ft_strlen(game->map_grid->symbols[y]) - 1;
		for (int x = 0; x < line_length; x++)
		{
			uint32_t color = (game->map_grid->symbols[y][x] == '1') ? 0x888888FF : 0x222222FF;

			// Calculate scaled positions
			int tileX0 = minimap_offset_x + (int)(x * scale);
			int tileY0 = minimap_offset_y + (int)(y * scale);
			int tileX1 = minimap_offset_x + (int)((x + 1) * scale);
			int tileY1 = minimap_offset_y + (int)((y + 1) * scale);

			// Draw the tile
			for (int py = tileY0; py < tileY1; py++)
			{
				for (int px = tileX0; px < tileX1; px++)
				{
					if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT)
					{
						mlx_put_pixel(game->image, px, py, color);
					}
				}
			}
		}
	}

	// Draw the player on the minimap
	float playerX = minimap_offset_x + (game->player.x / TILE_SIZE) * scale;
	float playerY = minimap_offset_y + (game->player.y / TILE_SIZE) * scale;

	// Draw the player as a small rectangle or point
	for (int i = -2; i <= 2; i++)
	{
		for (int j = -2; j <= 2; j++)
		{
			int px = (int)(playerX + i);
			int py = (int)(playerY + j);

			if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT)
			{
				mlx_put_pixel(game->image, px, py, 0xFF0000FF); // Red color for the player
			}
		}
	}

	// Draw the player's viewing direction
	float dirX = cos(game->player.angle) * 5 * scale;
	float dirY = sin(game->player.angle) * 5 * scale;

	int lineEndX = (int)(playerX + dirX);
	int lineEndY = (int)(playerY + dirY);

	// Draw the line representing the player's viewing direction
	draw_line(game->image, (int)playerX, (int)playerY, lineEndX, lineEndY, 0xFFFF00FF); // Yellow color

	// Visualize rays on the minimap with collision detection
	float fov_rad = FOV * M_PI / 180.0f;
	float angleIncrement = fov_rad / NUM_RAYS;
	float startAngle = game->player.angle - (fov_rad / 2.0f);

	for (int ray = 0; ray < NUM_RAYS; ray += 10) // Skip some rays for clarity
	{
		float ray_angle = startAngle + ray * angleIncrement;

		// Normalize angle
		if (ray_angle < 0)
			ray_angle += 2 * M_PI;
		if (ray_angle > 2 * M_PI)
			ray_angle -= 2 * M_PI;

		// Raycasting variables for minimap
		float rayX = game->player.x;
		float rayY = game->player.y;

		float ray_dir_x = cos(ray_angle);
		float ray_dir_y = sin(ray_angle);

		// Perform simplified DDA
		int hit = 0;
		float totalDistance = 0.0f;
		float stepSize = 1.0f; // Adjust the increment as needed

		while (!hit)
		{
			// Increment ray position
			rayX += ray_dir_x * stepSize;
			rayY += ray_dir_y * stepSize;
			totalDistance += stepSize;

			// Calculate map coordinates
			int map_x = (int)(rayX / TILE_SIZE);
			int map_y = (int)(rayY / TILE_SIZE);

			// Check if ray is out of bounds
			if (map_x < 0 || map_x >= map_width || map_y < 0 || map_y >= map_height)
			{
				hit = 1;
				break;
			}

			// Check if the ray has hit a wall
			if ((game->map_grid->symbols[map_y][map_x]) == '1')
			{
				hit = 1;
				break;
			}

			// Check if the ray has reached maximum view distance
			if (totalDistance >= MAX_VIEW_DISTANCE)
			{
				hit = 1;
				break;
			}
		}

		// Scale to minimap
		float endMinimapX = minimap_offset_x + (rayX / TILE_SIZE) * scale;
		float endMinimapY = minimap_offset_y + (rayY / TILE_SIZE) * scale;

		// Draw the ray on the minimap
		draw_line(game->image, (int)playerX, (int)playerY, (int)endMinimapX, (int)endMinimapY, 0x0000FFFF); // Blue color
	}
}

void	update(void *param)
{
	t_game *game = (t_game *)param;

	// Clear the image
	clear_image(game->image, 0x000000FF); // Clear to black

	// Movement variables
	float moveSpeed = 1.0f; // Movement speed
	float rotSpeed = 2.0f * M_PI / 180.0f; // Rotation speed

	// Rotate the player
	if (g_keys.left)
	{
		game->player.angle -= rotSpeed;
		if (game->player.angle < 0)
			game->player.angle += 2 * M_PI;
	}
	if (g_keys.right)
	{
		game->player.angle += rotSpeed;
		if (game->player.angle >= 2 * M_PI)
			game->player.angle -= 2 * M_PI;
	}

	// Calculate movement direction
	float moveX = 0.0f;
	float moveY = 0.0f;

	if (g_keys.w)
	{
		moveX += cos(game->player.angle) * moveSpeed;
		moveY += sin(game->player.angle) * moveSpeed;
	}
	if (g_keys.s)
	{
		moveX -= cos(game->player.angle) * moveSpeed;
		moveY -= sin(game->player.angle) * moveSpeed;
	}

	if (g_keys.a)
	{
		moveX += cos(game->player.angle - M_PI / 2) * moveSpeed;
		moveY += sin(game->player.angle - M_PI / 2) * moveSpeed;
	}
	if (g_keys.d)
	{
		moveX += cos(game->player.angle + M_PI / 2) * moveSpeed;
		moveY += sin(game->player.angle + M_PI / 2) * moveSpeed;
	}
	// Normalize movement vector
	float magnitude = sqrt(moveX * moveX + moveY * moveY);
	if (magnitude > 0)
	{
		moveX = (moveX / magnitude) * moveSpeed;
		moveY = (moveY / magnitude) * moveSpeed;
	}

	// New positions
	float newX = game->player.x + moveX;
	float newY = game->player.y + moveY;

	// Wall sliding collision detection with collision buffer
	bool canMoveX = can_move_to(newX, game->player.y, game);
	bool canMoveY = can_move_to(game->player.x, newY, game);

	// Update positions based on collision detection
	if (canMoveX)
	{
		game->player.x = newX;
	}

	if (canMoveY)
	{
		game->player.y = newY;
	}

	// Draw ceiling and floor once per frame
	uint32_t ceilingColor = rgb_to_uint32(game->assets.colors.rgb_C); // Ceiling color
	uint32_t floorColor = rgb_to_uint32(game->assets.colors.rgb_F); // Floor color

	draw_ceiling_and_floor(game->image, ceilingColor, floorColor);
	// Cast rays and render the 3D view
	cast_rays(game);

	// Draw the minimap
	drawMinimap(game);
}

int	raycast_engine(t_vector *map, t_player player, t_assets *assets)
{
	t_game *game;

	game = initialize_game(map, player, assets);
	if (!game)
	{
		return (EXIT_FAILURE);
	}
	mlx_loop_hook(game->mlx, &update, game);
	mlx_key_hook(game->mlx, &key_press, game);
	mlx_loop(game->mlx);
	cleanup(game);
	return (EXIT_SUCCESS);
}
