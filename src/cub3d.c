#include "../cub3d.h"

typedef struct s_keys
{
	bool w;
	bool a;
	bool s;
	bool d;
	bool left;
	bool right;
} t_keys;

t_keys keys = {false, false, false, false};

int ft_char_to_int(char c)
{
	return(c - '0');
}

uint32_t	rgb_to_uint32(int rgb[3])
{
	//printf("Clamped RGB values: R=%d, G=%d, B=%d\n", r, g, b);
	//printf("Combined uint32_t color: 0x%08X\n", color);
	return (rgb[0] << 24 | rgb[1] << 16 | rgb[2] << 8 | 255);
}
uint32_t	applyFogEffect(uint32_t color, float perpWallDist, float maxViewDistance)
{
	float fogFactor = perpWallDist / maxViewDistance;
	if (fogFactor > 1.0f)
		fogFactor = 1.0f;
	uint8_t r = ((color >> 24) & 0xFF) * (1.0f - fogFactor);
	uint8_t g = ((color >> 16) & 0xFF) * (1.0f - fogFactor);
	uint8_t b = ((color >> 8) & 0xFF) * (1.0f - fogFactor);
	uint8_t a = color & 0xFF;
	return (r << 24) | (g << 16) | (b << 8) | a;
}

void	initRay(t_game *game, int ray, float fovRad, float *rayAngle, float *rayDirX, float *rayDirY)
{
	float angleIncrement = fovRad / NUM_RAYS;
	float startAngle = game->player.angle - (fovRad / 2.0f);

	// Normalize start angle within the range of 0 to 2 * PI
	if (startAngle < 0)
		startAngle += 2 * M_PI;
	if (startAngle > 2 * M_PI)
		startAngle -= 2 * M_PI;

	*rayAngle = startAngle + ray * angleIncrement;

	// Normalize the ray angle between 0 and 2 * PI
	if (*rayAngle < 0)
		*rayAngle += 2 * M_PI;
	if (*rayAngle > 2 * M_PI)
		*rayAngle -= 2 * M_PI;

	*rayDirX = cos(*rayAngle);
	*rayDirY = sin(*rayAngle);
}

int	performDDA(t_game *game, float rayDirX, float rayDirY, int *mapX, int *mapY, int *side, float *perpWallDist)
{
	float deltaDistX = (rayDirX == 0) ? 1e30f : fabs(TILE_SIZE / rayDirX);
	float deltaDistY = (rayDirY == 0) ? 1e30f : fabs(TILE_SIZE / rayDirY);

	float sideDistX, sideDistY;
	int stepX, stepY;

	// Determine the step direction and initial side distances
	if (rayDirX < 0)
	{
		stepX = -1;
		sideDistX = (game->player.x - (*mapX) * TILE_SIZE) / fabs(rayDirX);
	}
	else
	{
		stepX = 1;
		sideDistX = (((*mapX) + 1) * TILE_SIZE - game->player.x) / fabs(rayDirX);
	}

	if (rayDirY < 0)
	{
		stepY = -1;
		sideDistY = (game->player.y - (*mapY) * TILE_SIZE) / fabs(rayDirY);
	}
	else
	{
		stepY = 1;
		sideDistY = (((*mapY) + 1) * TILE_SIZE - game->player.y) / fabs(rayDirY);
	}

	int hit = 0;
	*perpWallDist = 0.0f; // Initialize before the loop
	while (hit == 0)
	{
		// Jump to next map square
		if (sideDistX < sideDistY)
		{
			sideDistX += deltaDistX;
			*mapX += stepX;
			*side = 0;
		}
		else
		{
			sideDistY += deltaDistY;
			*mapY += stepY;
			*side = 1;
		}

		// Calculate map_width
		int map_width = ft_strlen(game->mapGrid->symbols[0]);

		// Check if *mapX and *mapY are within map bounds
		if (*mapX < 0 || *mapX >= map_width || *mapY < 0 || *mapY >= game->mapGrid->capacity)
		{
			*perpWallDist = MAX_VIEW_DISTANCE;
			hit = 2;
			break;
		}

		// Check if ray hits a wall
		char symbol = game->mapGrid->symbols[*mapY][*mapX];
		if (ft_char_to_int(symbol) > 0)
		{
			hit = 1;
			if (*side == 0)
					*perpWallDist = (sideDistX - deltaDistX);
			else
					*perpWallDist = (sideDistY - deltaDistY);
		}

		// Check if out of bounds
		if (*perpWallDist > MAX_VIEW_DISTANCE)
		{
			*perpWallDist = MAX_VIEW_DISTANCE;
			hit = 2;
			break;
		}
	}
	return hit;
}


void correctAndComputeWall(float *perpWallDist, float rayAngle, t_game *game, int *lineHeight)
{
	// Correct for the fish-eye effect
	float angleDiff = rayAngle - game->player.angle;
	if (angleDiff < -M_PI)
		angleDiff += 2 * M_PI;
	if (angleDiff > M_PI)
		angleDiff -= 2 * M_PI;
	*perpWallDist *= cos(angleDiff);

	// Calculate height of the line to draw
	//*lineHeight = (int)(HEIGHT * TILE_SIZE / (*perpWallDist));
	*lineHeight = (int)(HEIGHT  * TILE_SIZE/ (*perpWallDist));
	if (*lineHeight > HEIGHT)
		*lineHeight = HEIGHT;
	if (*lineHeight < 1)
		*lineHeight = 1; // Prevent lineHeight from being zero or negative
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
	float fovRad = FOV * M_PI / 180.0f;

	for (int ray = 0; ray < NUM_RAYS; ray++)
	{
		// Ray Initialization
		float rayAngle, rayDirX, rayDirY;
		initRay(game, ray, fovRad, &rayAngle, &rayDirX, &rayDirY);

		// Player position in grid coordinates
		int mapX = (int)(game->player.x / TILE_SIZE);
		int mapY = (int)(game->player.y / TILE_SIZE);

		// Perform DDA algorithm to detect walls
		int side;
		float perpWallDist;
		int hit = performDDA(game, rayDirX, rayDirY, &mapX, &mapY, &side, &perpWallDist);

		// Correct for fish-eye effect and calculate line height
		int lineHeight;
		correctAndComputeWall(&perpWallDist, rayAngle, game, &lineHeight);

		// Calculate draw positions
		int drawStart = -lineHeight / 2 + HEIGHT / 2;
		if (drawStart < 0) drawStart = 0;
		int drawEnd = lineHeight / 2 + HEIGHT / 2;
		if (drawEnd >= HEIGHT) drawEnd = HEIGHT - 1;

		// Choose wall texture based on the side of the hit
		mlx_texture_t *texture;
		if (side == 0) // North/South wall
		{
			texture = (rayDirX < 0) ? game->assets.textures.WE : game->assets.textures.EA;
		}
		else // East/West wall
		{
			texture = (rayDirY < 0) ? game->assets.textures.SO : game->assets.textures.NO;
		}

		// Calculate the texture coordinate based on where the ray hit the wall
		float wallX; // where the wall was hit
		if (side == 0) // North/South wall
			wallX = game->player.y + perpWallDist * rayDirY;
		else // East/West wall
			wallX = game->player.x + perpWallDist * rayDirX;

		wallX -= floor(wallX); // get the fractional part (texture coord)

		// Calculate texture X coordinate
		int texX = (int)(wallX * (float)texture->width);
		if (side == 0 && rayDirX > 0) // correct for direction of the ray
			texX = texture->width - texX - 1;
		if (side == 1 && rayDirY < 0) // correct for direction of the ray
			texX = texture->width - texX - 1;

		// Calculate scaling factor once for this column
		float scaling_factor = (float)texture->height / lineHeight;

		// Calculate initial texture position
		float texture_position = (drawStart - HEIGHT / 2 + lineHeight \
						/ 2) * scaling_factor; // adjust this to your needs

		// Draw the texture on the wall slice
		for (int y = drawStart; y < drawEnd; y++)
		{
			// Calculate the texture Y-coordinate, wrapping correctly
			int texY = (int)texture_position % (texture->height - 1);
			// Increment texture position
			texture_position += scaling_factor;
			// Fetch the pixel color from the texture using texX and texY
			uint32_t color = retrieve_color_at_coordinates(texX, texY, (uint32_t *)texture->pixels, texture->width, texture->height);
			// Optional color manipulation (fog, shading, etc.)
			color = (color << 24) | (((color >> 16) << 24) >> 16) | \
							(((color << 16) >> 24) << 16) | (color >> 24);
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
			keys.w = true;
		if (keydata.key == MLX_KEY_A)
			keys.a = true;
		if (keydata.key == MLX_KEY_S)
			keys.s = true;
		if (keydata.key == MLX_KEY_D)
			keys.d = true;
		if (keydata.key == MLX_KEY_LEFT)
			keys.left = true;
		if (keydata.key == MLX_KEY_RIGHT)
			keys.right = true;
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
			keys.w = false;
		if (keydata.key == MLX_KEY_A)
			keys.a = false;
		if (keydata.key == MLX_KEY_S)
			keys.s = false;
		if (keydata.key == MLX_KEY_D)
			keys.d = false;
		if (keydata.key == MLX_KEY_LEFT)
			keys.left = false;
		if (keydata.key == MLX_KEY_RIGHT)
			keys.right = false;
	}
	printf("W: %d, A: %d, S: %d, D: %d, Left: %d, Right: %d\n", keys.w, keys.a, keys.s, keys.d, keys.left, keys.right);
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

bool can_move_to(float x, float y, t_game *game)
{
	int map_height = game->mapGrid->capacity;
	int map_width = ft_strlen(game->mapGrid->symbols[0]);
	float radius = 2.0f; // Adjust the radius as needed
	int mapX1 = (int)((x - radius) / TILE_SIZE);
	int mapY1 = (int)((y - radius) / TILE_SIZE);
	int mapX2 = (int)((x + radius) / TILE_SIZE);
	int mapY2 = (int)((y + radius) / TILE_SIZE);

	// Check for out-of-bounds
	if (mapX1 < 0 || mapX2 >= map_width || mapY1 < 0 || mapY2 >= map_height)
	{
		return false;
	}
	// Check all corners of the player's bounding box
	if (ft_char_to_int(game->mapGrid->symbols[mapY1][mapX1]) > 0 ||
		ft_char_to_int(game->mapGrid->symbols[mapY1][mapX2]) > 0 ||
		ft_char_to_int(game->mapGrid->symbols[mapY2][mapX1]) > 0 ||
		ft_char_to_int(game->mapGrid->symbols[mapY2][mapX2]) > 0)
	{
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
    int map_width = ft_strlen(game->mapGrid->symbols[0]) - 1;

    // Calculate scale factors
    float scaleX = (float)minimap_width / (float)map_width;
    float scaleY = (float)minimap_height / (float)map_height;
    float scale = (scaleX < scaleY) ? scaleX : scaleY;

    // Draw the map grid
    for (int y = 0; y < map_height; y++)
    {
        for (int x = 0; x < map_width; x++)
        {
            uint32_t color = (ft_char_to_int(game->mapGrid->symbols[y][x]) == 1) ? 0x888888FF : 0x222222FF;

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
    float fovRad = FOV * M_PI / 180.0f;
    float angleIncrement = fovRad / NUM_RAYS;
    float startAngle = game->player.angle - (fovRad / 2.0f);

    for (int ray = 0; ray < NUM_RAYS; ray += 10) // Skip some rays for clarity
    {
        float rayAngle = startAngle + ray * angleIncrement;

        // Normalize angle
        if (rayAngle < 0)
            rayAngle += 2 * M_PI;
        if (rayAngle > 2 * M_PI)
            rayAngle -= 2 * M_PI;

        // Raycasting variables for minimap
        float rayX = game->player.x;
        float rayY = game->player.y;

        float rayDirX = cos(rayAngle);
        float rayDirY = sin(rayAngle);

        // Perform simplified DDA
        int hit = 0;
        float totalDistance = 0.0f;
        float stepSize = 1.0f; // Adjust the increment as needed

        while (!hit)
        {
            // Increment ray position
            rayX += rayDirX * stepSize;
            rayY += rayDirY * stepSize;
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
            if ((game->mapGrid->symbols[mapY][mapX]) - '0' > 0)
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
	if (keys.left)
	{
		game->player.angle -= rotSpeed;
		if (game->player.angle < 0)
			game->player.angle += 2 * M_PI;
	}
	if (keys.right)
	{
		game->player.angle += rotSpeed;
		if (game->player.angle >= 2 * M_PI)
			game->player.angle -= 2 * M_PI;
	}

	// Calculate movement direction
	float moveX = 0.0f;
	float moveY = 0.0f;

	if (keys.w)
	{
		moveX += cos(game->player.angle) * moveSpeed;
		moveY += sin(game->player.angle) * moveSpeed;
	}
	if (keys.s)
	{
		moveX -= cos(game->player.angle) * moveSpeed;
		moveY -= sin(game->player.angle) * moveSpeed;
	}

	if (keys.a)
	{
		moveX += cos(game->player.angle - M_PI / 2) * moveSpeed;
		moveY += sin(game->player.angle - M_PI / 2) * moveSpeed;
	}
	if (keys.d)
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
