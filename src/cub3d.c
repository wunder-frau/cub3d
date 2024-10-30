#include "../cub3d.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef struct s_keys
{
	bool w;
	bool a;
	bool s;
	bool d;
	bool left;
	bool right;
} t_keys;

typedef struct s_ray
{
	float ray_dir_x;
	float ray_dir_y;
	int map_x;
	int map_y;
	float delta_dist_x;
	float delta_dist_y;
	float side_dist_x;
	float side_dist_y;
	int step_x;
	int step_y;
	int side;
	float perp_wall_dist;
} t_ray;

// Global variables
static t_keys g_keys = {false, false, false, false, false, false};

// Convert character to integer
int ft_char_to_int(char c)
{
	return (c - '0');
}

// Convert RGB array to uint32_t color value
uint32_t rgb_to_uint32(int rgb[3])
{
	return ((rgb[0] << 24) | (rgb[1] << 16) | (rgb[2] << 8) | 255);
}

// Apply fog effect to color based on wall distance
uint32_t apply_fog_effect(uint32_t color, float perp_wall_dist, float max_view_distance)
{
	float fog_factor;
	uint8_t r, g, b, a;

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
void init_ray_angle(t_game *game, int ray, float fov_rad, float *ray_angle)
{
	float angle_increment;
	float start_angle;

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

void init_ray_direction(float ray_angle, float *ray_dir_x, float *ray_dir_y)
{
	*ray_dir_x = cos(ray_angle);
	*ray_dir_y = sin(ray_angle);
}

// Calculate delta distances for DDA
void calculate_delta_distances(t_ray *ray)
{
	ray->delta_dist_x = (ray->ray_dir_x == 0) ? 1e30f : fabs(1.0f / ray->ray_dir_x);
	ray->delta_dist_y = (ray->ray_dir_y == 0) ? 1e30f : fabs(1.0f / ray->ray_dir_y);
}

// Calculate step values for X direction
void calculate_step_x(t_game *game, t_ray *ray)
{
	float player_x_in_tiles;

	player_x_in_tiles = game->player.x / TILE_SIZE;

	if (ray->ray_dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (player_x_in_tiles - ray->map_x) * ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1.0f - player_x_in_tiles) * ray->delta_dist_x;
	}
}

// Calculate step values for Y direction
void calculate_step_y(t_game *game, t_ray *ray)
{
	float player_y_in_tiles;

	player_y_in_tiles = game->player.y / TILE_SIZE;

	if (ray->ray_dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (player_y_in_tiles - ray->map_y) * ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0f - player_y_in_tiles) * ray->delta_dist_y;
	}
}

// Initialize step and side distances for DDA
void calculate_step_and_side_dist(t_game *game, t_ray *ray)
{
	calculate_delta_distances(ray);
	calculate_step_x(game, ray);
	calculate_step_y(game, ray);
}

// Update ray position and side distance
void update_ray_position(t_ray *ray)
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

// Check if ray is out of bounds
int check_out_of_bounds(t_game *game, t_ray *ray)
{
	int map_width;

	map_width = ft_strlen(game->mapGrid->symbols[0]);
	if (ray->map_x < 0 || ray->map_x >= map_width || ray->map_y < 0 || ray->map_y >= game->mapGrid->capacity)
	{
		ray->perp_wall_dist = MAX_VIEW_DISTANCE;
		return (2);
	}
	return (0);
}

// Check if ray hit a wall
int check_wall_hit(t_game *game, t_ray *ray)
{
	char symbol;

	symbol = game->mapGrid->symbols[ray->map_y][ray->map_x];
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

// Perform DDA loop to find wall hit
int perform_dda_loop(t_game *game, t_ray *ray)
{
	int hit = 0;

	ray->perp_wall_dist = 0.0f;
	while (hit == 0)
	{
		update_ray_position(ray);
		if ((hit = check_out_of_bounds(game, ray)) != 0)
			break;
		hit = check_wall_hit(game, ray);
		if (ray->perp_wall_dist > MAX_VIEW_DISTANCE)
		{
			ray->perp_wall_dist = MAX_VIEW_DISTANCE;
			hit = 2;
			break;
		}
	}
	return (hit);
}

int perform_dda(t_game *game, float ray_dir_x, float ray_dir_y, int *map_x, int *map_y, int *side, float *perp_wall_dist)
{
	t_ray ray;

	ray.ray_dir_x = ray_dir_x;
	ray.ray_dir_y = ray_dir_y;
	ray.map_x = *map_x;
	ray.map_y = *map_y;

	calculate_step_and_side_dist(game, &ray);
	int hit = perform_dda_loop(game, &ray);

	*map_x = ray.map_x;
	*map_y = ray.map_y;
	*side = ray.side;
	*perp_wall_dist = ray.perp_wall_dist;

	return (hit);
}




void correctAndComputeWall(float *perpWallDist, float ray_angle, t_game *game, int *lineHeight)
{
	float angleDiff;

	// Correct for the fish-eye effect
	angleDiff = ray_angle - game->player.angle;
	if (angleDiff < -M_PI)
		angleDiff += 2 * M_PI;
	if (angleDiff > M_PI)
		angleDiff -= 2 * M_PI;
	*perpWallDist *= cos(angleDiff);

	// Prevent division by zero
	if (*perpWallDist < 0.0001f)
		*perpWallDist = 0.0001f;

	// Calculate height of the line to draw (proportional to the inverse of distance)
	*lineHeight = (int)(HEIGHT / (*perpWallDist));
	if (*lineHeight > HEIGHT)
		*lineHeight = HEIGHT;
	if (*lineHeight < 1)
		*lineHeight = 1; // Ensure lineHeight is at least 1
}


void drawCeilingAndFloor(mlx_image_t *image, uint32_t ceilingColor, uint32_t floorColor)
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

uint32_t	retrieve_color_at_coordinates(int x, int y, uint32_t *arr, int tex_width, int tex_height)
{
	int	index;
	// Check if the calculated index is within the bounds of the array
	if (x < 0 || x >= tex_width || y < 0 || y >= tex_height)
	// Optionally, return a default color or handle the error as needed
		return (0x000000FF); // Black color with full alpha
	// Calculate the index based on the y coordinate
	index = y * tex_width + x;
	// Retrieve the color from the array
	return (arr[index]);
}

void castRays(t_game *game)
{
    int map_height = game->mapGrid->capacity;
    int map_width = ft_strlen(game->mapGrid->symbols[0]);
    float fov_rad = FOV * M_PI / 180.0f;

    // Convert player position to tile units
    float playerXInTiles = game->player.x / TILE_SIZE;
    float playerYInTiles = game->player.y / TILE_SIZE;

    for (int ray = 0; ray < NUM_RAYS; ray++)
    {
        // Ray Initialization
        float ray_angle, ray_dir_x, ray_dir_y;
        init_ray_angle(game, ray, fov_rad, &ray_angle);
        init_ray_direction(ray_angle, &ray_dir_x, &ray_dir_y);

        // Player position in grid coordinates
        int mapX = (int)(playerXInTiles);
        int mapY = (int)(playerYInTiles);

        // Perform DDA algorithm to detect walls
        int side;
        float perpWallDist;
        int hit = perform_dda(game, ray_dir_x, ray_dir_y, &mapX, &mapY, &side, &perpWallDist);

        // Correct for the fish-eye effect and calculate line height
        float angleDiff = ray_angle - game->player.angle;
        if (angleDiff < -M_PI)
            angleDiff += 2 * M_PI;
        if (angleDiff > M_PI)
            angleDiff -= 2 * M_PI;

        // Correct the perpendicular wall distance
        float correctedPerpWallDist = perpWallDist * cos(angleDiff);

        // Calculate line height for rendering
        int lineHeight = (int)(HEIGHT / correctedPerpWallDist);
        if (lineHeight > HEIGHT)
            lineHeight = HEIGHT;

        // Calculate draw positions
        int drawStart = -lineHeight / 2 + HEIGHT / 2;
        if (drawStart < 0) drawStart = 0;
        int drawEnd = lineHeight / 2 + HEIGHT / 2;
        if (drawEnd >= HEIGHT) drawEnd = HEIGHT - 1;

        // Choose wall texture based on the side of the hit
        mlx_texture_t *texture;
        if (side == 0) // North/South wall
        {
            texture = (ray_dir_x < 0) ? game->assets.textures.WE : game->assets.textures.EA;
        }
        else // East/West wall
        {
            texture = (ray_dir_y < 0) ? game->assets.textures.SO : game->assets.textures.NO;
        }

        // Calculate the texture coordinate based on where the ray hit the wall
        float wallX; // where the wall was hit
        if (side == 0) // North/South wall
            wallX = playerYInTiles + perpWallDist * ray_dir_y;
        else // East/West wall
            wallX = playerXInTiles + perpWallDist * ray_dir_x;

        wallX -= floor(wallX); // get the fractional part (texture coord)

        // Calculate texture X coordinate
        int texX = (int)(wallX * (float)texture->width);
        if (side == 0 && ray_dir_x > 0) // correct for direction of the ray
            texX = texture->width - texX - 1;
        if (side == 1 && ray_dir_y < 0) // correct for direction of the ray
            texX = texture->width - texX - 1;

        // Calculate step and initial texture position
        float step = 1.0f * texture->height / lineHeight;
        float texPos = (drawStart - HEIGHT / 2 + lineHeight / 2) * step;

        // Draw the texture on the wall slice
        for (int y = drawStart; y < drawEnd; y++)
        {
            int texY = (int)texPos;
            if (texY < 0) texY = 0;
            if (texY >= texture->height) texY = texture->height - 1;
            texPos += step;

            // Fetch the pixel color from the texture using texX and texY
            uint32_t color = retrieve_color_at_coordinates(texX, texY, (uint32_t *)texture->pixels, texture->width, texture->height);

            // Handle endianness if necessary
            color = (color << 24) | (((color >> 16) << 24) >> 16) | (((color << 16) >> 24) << 16) | (color >> 24);

            // Draw the pixel on the screen
            mlx_put_pixel(game->image, ray, y, color);
        }
    }
}






void drawMap(t_game *game)
{
	// for (int y = 0; y < MAP_HEIGHT; y++)
	for (int y = 0; y < game->mapGrid->capacity; y++)
	{
		// for (int x = 0; x < MAP_WIDTH; x++)
		for (int x = 0; x < ft_strlen(game->mapGrid->symbols[0]); x++)
		{
			uint32_t color = (game->mapGrid->symbols[y][x] - '0' == 1) ? 0xFFFFFFFF : 0x000000FF; // White for walls, black for empty space
			int tileX = x * TILE_SIZE;
			int tileY = y * TILE_SIZE;

			// Draw the tile
			for (int i = 0; i < TILE_SIZE; i++)
			{
				for (int j = 0; j < TILE_SIZE; j++)
				{
					int px = tileX + i;
					int py = tileY + j;

					// Ensure we don't draw outside the image boundaries
					if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT)
					{
						mlx_put_pixel(game->image, px, py, color);
					}
				}
			}
		}
	}
}

void drawPlayer(t_game *game)
{
	int playerX = (int)game->player.x;
	int playerY = (int)game->player.y;
	uint32_t color = 0xFF0000FF; // Red color

	// Draw a small square to represent the player
	int size = 5; // Size of the player square
	for (int y = -size; y <= size; y++)
	{
		for (int x = -size; x <= size; x++)
		{
			int px = playerX + x;
			int py = playerY + y;

			if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT)
			{
				mlx_put_pixel(game->image, px, py, color);
			}
		}
	}
}

void key_press(mlx_key_data_t keydata, void *param)
{
	t_game *game = (t_game *)param;

	if (keydata.action == MLX_PRESS)
	{
		if (keydata.key == MLX_KEY_W)
			g_keys.w = true;
		if (keydata.key == MLX_KEY_A)
			g_keys.a = true;
		if (keydata.key == MLX_KEY_S)
			g_keys.s = true;
		if (keydata.key == MLX_KEY_D)
			g_keys.d = true;
		if (keydata.key == MLX_KEY_LEFT)
			g_keys.left = true;
		if (keydata.key == MLX_KEY_RIGHT)
			g_keys.right = true;
		if (keydata.key == MLX_KEY_ESCAPE)
		{
			// Exit the game gracefully
			cleanup(game);
			exit(EXIT_SUCCESS);
			// mlx_close_window(game->mlx);
		}
	}
	else if (keydata.action == MLX_RELEASE)
	{
		if (keydata.key == MLX_KEY_W)
			g_keys.w = false;
		if (keydata.key == MLX_KEY_A)
			g_keys.a = false;
		if (keydata.key == MLX_KEY_S)
			g_keys.s = false;
		if (keydata.key == MLX_KEY_D)
			g_keys.d = false;
		if (keydata.key == MLX_KEY_LEFT)
			g_keys.left = false;
		if (keydata.key == MLX_KEY_RIGHT)
			g_keys.right = false;
	}
	printf("W: %d, A: %d, S: %d, D: %d, Left: %d, Right: %d\n", g_keys.w, g_keys.a, g_keys.s, g_keys.d, g_keys.left, g_keys.right);
}

void draw_line(mlx_image_t *image, int x0, int y0, int x1, int y1, uint32_t color)
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

// bool can_move_to(float x, float y, t_game *game)
// {
// 	int map_height = game->mapGrid->capacity;
// 	int map_width = ft_strlen(game->mapGrid->symbols[0]);
// 	float radius = 2.0f; // Adjust the radius as needed
// 	int mapX1 = (int)((x - radius) / TILE_SIZE);
// 	int mapY1 = (int)((y - radius) / TILE_SIZE);
// 	int mapX2 = (int)((x + radius) / TILE_SIZE);
// 	int mapY2 = (int)((y + radius) / TILE_SIZE);

// 	// Check for out-of-bounds
// 	if (mapX1 < 0 || mapX2 >= map_width || mapY1 < 0 || mapY2 >= map_height)
// 	{
// 		return false;
// 	}
// 	// Check all corners of the player's bounding box
// 	if (ft_char_to_int(game->mapGrid->symbols[mapY1][mapX1]) > 0 ||
// 		ft_char_to_int(game->mapGrid->symbols[mapY1][mapX2]) > 0 ||
// 		ft_char_to_int(game->mapGrid->symbols[mapY2][mapX1]) > 0 ||
// 		ft_char_to_int(game->mapGrid->symbols[mapY2][mapX2]) > 0)
// 	{
// 		return false;
// 	}
// 	return true;
// }

static bool is_walkable(char c)
{
	return (c == '0' || is_player_symbol(c));
}


bool can_move_to(float x, float y, t_game *game)
{
		// int map_height = game->mapGrid->capacity; // Ensure this reflects the actual number of rows
		// int map_width = ft_strlen(game->mapGrid->symbols[0]);
		int map_height = game->mapGrid->capacity;
		int map_width = 0;

		int i = 0;
		while (i < map_height)
		{
			int line_length = ft_strlen(game->mapGrid->symbols[i]) - 1;
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
	if (!is_walkable(game->mapGrid->symbols[mapY1][mapX1]) ||
		!is_walkable(game->mapGrid->symbols[mapY1][mapX2]) ||
		!is_walkable(game->mapGrid->symbols[mapY2][mapX1]) ||
		!is_walkable(game->mapGrid->symbols[mapY2][mapX2]))
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

 		int map_height = game->mapGrid->capacity;
		int map_width = 0;

		int i = 0;
		while (i < map_height)
		{
			int line_length = ft_strlen(game->mapGrid->symbols[i]) - 1;
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
			int line_length = ft_strlen(game->mapGrid->symbols[y]) - 1;
		for (int x = 0; x < line_length; x++)
		{
			uint32_t color = (game->mapGrid->symbols[y][x] == '1') ? 0x888888FF : 0x222222FF;

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
			int mapX = (int)(rayX / TILE_SIZE);
			int mapY = (int)(rayY / TILE_SIZE);

			// Check if ray is out of bounds
			if (mapX < 0 || mapX >= map_width || mapY < 0 || mapY >= map_height)
			{
				hit = 1;
				break;
			}

			// Check if the ray has hit a wall
			if ((game->mapGrid->symbols[mapY][mapX]) == '1')
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

	drawCeilingAndFloor(game->image, ceilingColor, floorColor);
	// Cast rays and render the 3D view
	castRays(game);

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
