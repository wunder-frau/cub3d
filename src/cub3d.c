#include "MLX42/MLX42.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h> // For trigonometric functions
#include "vector/vector.h"
#include "../cub3d.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define WIDTH 1200
#define HEIGHT 800
#define MAX_DEPTH 16 // Increase from 16 to 64
#define MAX_RENDER_DISTANCE 1000.0f // Adjust as needed
#define MAX_VIEW_DISTANCE 600.0f 

#define FOV 75 // Field of View in degrees
#define NUM_RAYS WIDTH // One ray per screen column

#define TILE_SIZE 32 // Size of each tile in pixels

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

// Function to convert RGB to uint32_t
uint32_t	rgb_to_uint32(int rgb[3])
{
	//printf("Clamped RGB values: R=%d, G=%d, B=%d\n", r, g, b);
	//printf("Combined uint32_t color: 0x%08X\n", color);
	return (rgb[0] << 24 | rgb[1] << 16 | rgb[2] << 8 | 255);
}
uint32_t applyFogEffect(uint32_t color, float perpWallDist, float maxViewDistance)
{
    float fogFactor = perpWallDist / maxViewDistance;
    if (fogFactor > 1.0f)
        fogFactor = 1.0f;

    // Extract RGB components from color and apply fog
    uint8_t r = ((color >> 24) & 0xFF) * (1.0f - fogFactor);
    uint8_t g = ((color >> 16) & 0xFF) * (1.0f - fogFactor);
    uint8_t b = ((color >> 8) & 0xFF) * (1.0f - fogFactor);
    uint8_t a = color & 0xFF; // Alpha stays unchanged

    // Return the color with fog applied
    return (r << 24) | (g << 16) | (b << 8) | a;
}

void initRay(t_game *game, int ray, float fovRad, float *rayAngle, float *rayDirX, float *rayDirY)
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

int performDDA(t_game *game, float rayDirX, float rayDirY, int *mapX, int *mapY, int *side, float *perpWallDist)
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
            hit = 2;
            *perpWallDist = MAX_VIEW_DISTANCE;
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

void drawCeilingAndFloor(t_game *game, int ray, int drawStart, int drawEnd)
{
    // Calculate ceiling and floor colors
    uint32_t ceilingColor = rgb_to_uint32(game->assets.colors.rgb_C); // Ceiling color
    uint32_t floorColor = rgb_to_uint32(game->assets.colors.rgb_F); // Floor color

    // Draw ceiling and floor
    for (int y = 0; y < HEIGHT; y++)
    {
        if (y < drawStart)
        {
            // Ceiling rendering
            mlx_put_pixel(game->image, ray, y, ceilingColor);
        }
        else if (y >= drawStart && y < drawEnd)
        {
            // Wall rendering is handled in the main casting function
        }
        else
        {
            // Floor rendering
            mlx_put_pixel(game->image, ray, y, floorColor);
        }
    }
}

// Function to retrieve a color from a 2D array using x and y coordinates
uint32_t retrieve_color_at_coordinates(int x, int y, uint32_t *arr, int tex_width)
{
    // Calculate the index based on the y coordinate
    int index = y * tex_width + x;

    // Check if the calculated index is within the bounds of the array
    if (x < 0 || x >= tex_width || y < 0) {
        fprintf(stderr, "Error: Coordinates (%d, %d) are out of bounds.\n", x, y);
        return 0; // Return a default color (e.g., transparent or black)
    }
    // Retrieve the color from the array
    return arr[index];
}

void castRays(t_game *game)
{
    int map_height = game->mapGrid->capacity;
    int map_width = ft_strlen(game->mapGrid->symbols[0]);
    float fovRad = FOV * M_PI / 180.0f;

    // Clear the image first (if necessary)
    // mlx_clear_image(game->image);

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
        // Calculate the scaling factor for texture mapping
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
            uint32_t color = retrieve_color_at_coordinates(texX, texY, (uint32_t *)texture->pixels, texture->width);

            // Optional color manipulation (fog, shading, etc.)
            color = (color << 24) | (((color >> 16) << 24) >> 16) | \
                    (((color << 16) >> 24) << 16) | (color >> 24);

            // Draw the pixel on the screen
            mlx_put_pixel(game->image, ray, y, color);
        }
        // Call the separate function to draw ceiling and floor
        drawCeilingAndFloor(game, ray, drawStart, drawEnd);
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
            // Exit the game
            mlx_close_window(game->mlx);
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
    int scale = 8; // Adjust the scale to make the minimap 2x bigger (from 4 to 8)
    int map_height = game->mapGrid->capacity;
    int map_width = ft_strlen(game->mapGrid->symbols[0]) - 1;
    		//printf("strlen: %d\n", map_width);
    // Draw the map grid
    // for (int y = 0; y < MAP_HEIGHT; y++)
    for (int y = 0; y < map_height; y++)
    {
        for (int x = 0; x < map_width; x++)
        {
            uint32_t color = (ft_char_to_int(game->mapGrid->symbols[y][x]) == 1) ? 0x888888FF : 0x222222FF;
           //uint32_t color = (ft_char_to_int(game->mapGrid->symbols[y][x]) == 1) ? 0xFF007FFF :  0xC8A2C8FF;

            int tileX = x * scale;
            int tileY = y * scale;

            for (int i = 0; i < scale; i++)
            {
                for (int j = 0; j < scale; j++)
                {
                    int px = tileX + i;
                    int py = tileY + j;

                    if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT)
                    {
                        mlx_put_pixel(game->image, px, py, color);
                    }
                }
            }
        }
    }

    // Draw the player on the minimap
    int playerX = (int)(game->player.x / TILE_SIZE * scale);
    int playerY = (int)(game->player.y / TILE_SIZE * scale);

    // Draw the player as a small rectangle or point
    for (int i = -2; i <= 2; i++)
    {
        for (int j = -2; j <= 2; j++)
        {
            int px = playerX + i;
            int py = playerY + j;

            if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT)
            {
                mlx_put_pixel(game->image, px, py, 0xFF0000FF); // Red color for the player
            }
        }
    }

    // Draw the player's viewing direction
    float dirX = cos(game->player.angle) * 5 * scale;
    float dirY = sin(game->player.angle) * 5 * scale;

    int lineEndX = playerX + (int)dirX;
    int lineEndY = playerY + (int)dirY;

    // Draw the line representing the player's viewing direction
    draw_line(game->image, playerX, playerY, lineEndX, lineEndY, 0xFFFF00FF); // Yellow color

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
            if (mapX < 0 || mapX >= map_height || mapY < 0 || mapY >= map_width)
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
        int endMinimapX = (int)(rayX / TILE_SIZE * scale);
        int endMinimapY = (int)(rayY / TILE_SIZE * scale);

        // Draw the ray on the minimap
        draw_line(game->image, playerX, playerY, endMinimapX, endMinimapY, 0x0000FFFF); // Blue color
    }
}



void update(void *param)
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

    // Cast rays and render the 3D view
    castRays(game);

    // Draw the minimap
    drawMinimap(game);
}

mlx_texture_t *load_texture(const char *path)
{
    mlx_texture_t *texture = mlx_load_png(path);
    printf("path_____%s\n", path);
    if (!texture)
    {
        ft_putstr_fd("Failed to load texture: ", 2);
        ft_putstr_fd(path, 2);
        ft_putstr_fd("\n", 2);
    }
    return texture;
}

t_assets *initialize_ass(t_vector *map, t_assets *assets)
{
    // t_assets *assets = malloc(sizeof(t_assets));
    // if (!assets)
    //     return NULL;

    // Initialize paths (assuming paths are already assigned to the assets->textures)
    assets->textures.NO = load_texture(assets->textures.path_NO);
    assets->textures.SO = load_texture(assets->textures.path_SO);
    assets->textures.EA = load_texture(assets->textures.path_EA);
    assets->textures.WE = load_texture(assets->textures.path_WE);

    // Check if any texture failed to load
    if (!assets->textures.NO || !assets->textures.SO || !assets->textures.EA || !assets->textures.WE)
    {
        ft_putstr_fd("Failed to load all required textures\n", 2);
        //free_assets_struct(assets);  // Function to free loaded assets
        error_exit_cleanup("Map validation failed", map, assets);
        return NULL;
    }

    // Continue initializing other assets (colors, etc.)
    // ...

    return assets;
}

// Function to initialize the game structure
t_game *initialize_game(t_vector *map, t_player player, t_assets *assets) {
    t_game *game = malloc(sizeof(t_game));
    if (!game)
        return NULL;

    // Initialize MLX42
    game->mlx = mlx_init(WIDTH, HEIGHT, "CUB 3D", true);
    if (!game->mlx)
    {
        ft_putstr_fd("Failed to initialize MLX42\n", 2);
        free(game);
        return NULL;
    }

    // Create an image
    game->image = mlx_new_image(game->mlx, WIDTH, HEIGHT);
    if (!game->image)
    {
        ft_putstr_fd("Failed to create image\n", 2);
        mlx_terminate(game->mlx);
        free(game);
        return NULL;
    }

    // Attach the image to the window
    if (mlx_image_to_window(game->mlx, game->image, 0, 0) < 0)
    {
        ft_putstr_fd("Failed to attach image to window\n", 2);
        mlx_delete_image(game->mlx, game->image);
        mlx_terminate(game->mlx);
        free(game);
        return NULL;
    }

    // Initialize player position and angle
    game->player.x = player.x;
    game->player.y = player.y;
    game->player.angle = player.angle;
    ft_memcpy(game->assets.colors.rgb_C, assets->colors.rgb_C, sizeof(assets->colors.rgb_C));
    ft_memcpy(game->assets.colors.rgb_F, assets->colors.rgb_F, sizeof(assets->colors.rgb_F));
    // Initialize assets (textures, colors, etc.)
    game->assets = *initialize_ass(map, assets);
    if (!game->assets.textures.NO || !game->assets.textures.SO || !game->assets.textures.EA || !game->assets.textures.WE)
    {
        fprintf(stderr, "Failed to initialize assets or load textures.\n");
        mlx_delete_image(game->mlx, game->image);
        mlx_terminate(game->mlx);
        free(game);
        return NULL;
    }

    // Initialize the map
    game->mapGrid = map;

    return game;
}

int raycast_engine(t_vector *map, t_player player, t_assets *assets)
{
    t_game *game = initialize_game(map, player, assets);
    if (!game) {
        return EXIT_FAILURE; // Initialization failed
    }

    // Register the update function and key press hook
    mlx_loop_hook(game->mlx, &update, game);
    mlx_key_hook(game->mlx, &key_press, game);

    // Start the MLX42 loop
    mlx_loop(game->mlx);

    // Clean up resources
    mlx_delete_image(game->mlx, game->image);
    mlx_terminate(game->mlx);
    free(game); // Don't forget to free the game struct

    return EXIT_SUCCESS;
}
