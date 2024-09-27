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
#define MAX_DEPTH 64 // Increase from 16 to 64
#define MAX_RENDER_DISTANCE 1000.0f // Adjust as needed
#define MAX_VIEW_DISTANCE 600.0f 

#define FOV 75 // Field of View in degrees
#define NUM_RAYS WIDTH // One ray per screen column

#define TILE_SIZE 32 // Size of each tile in pixels

#define MAP_WIDTH 24
#define MAP_HEIGHT 24

// int mapGrid[MAP_HEIGHT][MAP_WIDTH] = {
//     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
//     {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
//     {1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1},
//     {1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1},
//     {1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1},
//     {1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1},
//     {1,0,1,0,1,0,1,1,1,1,1,1,1,1,1,1,1,1,0,1,0,1,0,1},
//     {1,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,1,0,1,0,1,0,1},
//     {1,0,1,0,1,0,1,0,1,1,1,1,1,1,1,1,0,1,0,1,0,1,0,1},
//     {1,0,1,0,1,0,1,0,1,0,0,0,0,0,0,1,0,1,0,1,0,1,0,1},
//     {1,0,1,0,1,0,1,0,1,0,1,1,1,1,0,1,0,1,0,1,0,1,0,1},
//     {1,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,0,1,0,1,0,1,0,1},
//     {1,0,1,0,1,0,1,0,1,0,1,0,0,1,0,1,0,1,0,1,0,1,0,1},
//     {1,0,1,0,1,0,1,0,1,0,1,1,0,1,0,1,0,1,0,1,0,1,0,1},
//     {1,0,1,0,1,0,1,0,1,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
//     {1,0,1,0,1,0,1,0,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
//     {1,0,1,0,1,0,1,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
//     {1,0,1,0,1,0,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
//     {1,0,1,0,1,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
//     {1,0,0,0,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
//     {1,0,0,0,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
//     {1,0,1,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1,0,1},
//     {1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1},
//     {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
// };

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

void castRays(t_game *game)
{
    int map_height = game->mapGrid->capacity;
    int map_width = ft_strlen(game->mapGrid->symbols[0]);
    char symbol;
    float fovRad = FOV * M_PI / 180.0f;
    float angleIncrement = fovRad / NUM_RAYS;

    float startAngle = game->player.angle - (fovRad / 2.0f);
    if (startAngle < 0)
        startAngle += 2 * M_PI;
    if (startAngle > 2 * M_PI)
        startAngle -= 2 * M_PI;

    for (int ray = 0; ray < NUM_RAYS; ray++)
    {
        float rayAngle = startAngle + ray * angleIncrement;
        if (rayAngle < 0)
            rayAngle += 2 * M_PI;
        if (rayAngle > 2 * M_PI)
            rayAngle -= 2 * M_PI;

        // DDA algorithm initialization
        float rayDirX = cos(rayAngle);
        float rayDirY = sin(rayAngle);

        int mapX = (int)(game->player.x / TILE_SIZE);
        int mapY = (int)(game->player.y / TILE_SIZE);

        // Calculate deltaDistX and deltaDistY
        float deltaDistX = (rayDirX == 0) ? 1e30f : fabs(TILE_SIZE / rayDirX);
        float deltaDistY = (rayDirY == 0) ? 1e30f : fabs(TILE_SIZE / rayDirY);

        float sideDistX;
        float sideDistY;

        int stepX;
        int stepY;

        // Calculate step and initial sideDist
        if (rayDirX < 0)
        {
            stepX = -1;
            sideDistX = (game->player.x - mapX * TILE_SIZE) / fabs(rayDirX);
        }
        else
        {
            stepX = 1;
            sideDistX = ((mapX + 1) * TILE_SIZE - game->player.x) / fabs(rayDirX);
        }

        if (rayDirY < 0)
        {
            stepY = -1;
            sideDistY = (game->player.y - mapY * TILE_SIZE) / fabs(rayDirY);
        }
        else
        {
            stepY = 1;
            sideDistY = ((mapY + 1) * TILE_SIZE - game->player.y) / fabs(rayDirY);
        }

        // Perform DDA
        int hit = 0; // Was there a wall hit?
        int side = 0; // Was a NS or EW wall hit?
        float perpWallDist = 0.0f;

        while (hit == 0)
        {
            // Jump to next map square, either in x-direction or y-direction
            if (sideDistX < sideDistY)
            {
                sideDistX += deltaDistX;
                mapX += stepX;
                side = 0;
            }
            else
            {
                sideDistY += deltaDistY;
                mapY += stepY;
                side = 1;
            }

            // Check if ray has gone out of bounds
            if (mapX < 0 || mapX >= map_width || mapY < 0 || mapY >= map_height)
            {
                // Ray is out of bounds
                perpWallDist = MAX_VIEW_DISTANCE;
                hit = 2; // No wall hit within view distance
                break;
            }
            // printf("struct___:%d\n", (int)(game->mapGrid->symbols[2][4]));
            // printf("struct___:%d\n", game->mapGrid->symbols[mapY][mapX] - '0');
            // printf("int_array__:%d\n", mapGrid[mapY][mapX]);
            // Check if ray has hit a wall
            symbol = game->mapGrid->symbols[mapY][mapX];
            if (ft_char_to_int(symbol) > 0)
            {
                hit = 1;
                // Calculate distance to the point of impact
                if (side == 0)
                    perpWallDist = (sideDistX - deltaDistX);
                else
                    perpWallDist = (sideDistY - deltaDistY);
            }

            // Check if the ray has exceeded the maximum view distance
            if (perpWallDist > MAX_VIEW_DISTANCE)
            {
                hit = 2; // No wall hit within view distance
                perpWallDist = MAX_VIEW_DISTANCE;
                break;
            }
        }

        // Correct for fish-eye effect
        float angleDiff = rayAngle - game->player.angle;
        if (angleDiff < -M_PI)
            angleDiff += 2 * M_PI;
        if (angleDiff > M_PI)
            angleDiff -= 2 * M_PI;
        perpWallDist *= cos(angleDiff);

        // Calculate height of line to draw
        int lineHeight = (int)(HEIGHT * TILE_SIZE / perpWallDist);
        if (lineHeight > HEIGHT)
            lineHeight = HEIGHT;
        if (lineHeight < 1)
            lineHeight = 1; // Prevent lineHeight from being zero or negative

        // Calculate lowest and highest pixel to fill in current stripe
        int drawStart = -lineHeight / 2 + HEIGHT / 2;
        if (drawStart < 0)
            drawStart = 0;
        int drawEnd = lineHeight / 2 + HEIGHT / 2;
        if (drawEnd >= HEIGHT)
            drawEnd = HEIGHT - 1;

        // Choose wall color
        uint32_t color;
        if (hit == 1)
        {
            if (side == 0)
                color = 0xFF0000FF; // Red for x-side walls
            else
                color = 0x00FF00FF; // Green for y-side walls

            // Apply fog effect
            float fogFactor = perpWallDist / MAX_VIEW_DISTANCE;
            if (fogFactor > 1.0f)
                fogFactor = 1.0f;

            // Apply fog to color
            uint8_t r = ((color >> 24) & 0xFF) * (1.0f - fogFactor);
            uint8_t g = ((color >> 16) & 0xFF) * (1.0f - fogFactor);
            uint8_t b = ((color >> 8) & 0xFF) * (1.0f - fogFactor);
            uint8_t a = color & 0xFF;
            color = (r << 24) | (g << 16) | (b << 8) | a;
        }
        else
        {
            // No wall hit within view distance, use background color
            color = 0x000000FF; // Black
        }

        // Draw the vertical line with floor and ceiling
        for (int y = 0; y < HEIGHT; y++)
        {
            if (y < drawStart)
            {
                // Ceiling
                mlx_put_pixel(game->image, ray, y, 0x333333FF);
            }
            else if (y >= drawStart && y <= drawEnd)
            {
                // Wall or background
                mlx_put_pixel(game->image, ray, y, color);
            }
            else
            {
                // Floor
                mlx_put_pixel(game->image, ray, y, 0x777777FF);
            }
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
    		printf("strlen: %d\n", map_width);
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


int raycast_engine(t_vector *map, t_assets *assets)
//int raycast_engine(void)
{
    t_game game;

    // Initialize MLX42
    game.mlx = mlx_init(WIDTH, HEIGHT, "CUB 3D", true);
    if (!game.mlx)
    {
        fprintf(stderr, "Failed to initialize MLX42.\n");
        return EXIT_FAILURE;
    }

    // Create an image
    game.image = mlx_new_image(game.mlx, WIDTH, HEIGHT);
    if (!game.image)
    {
        fprintf(stderr, "Failed to create image.\n");
        mlx_terminate(game.mlx);
        return EXIT_FAILURE;
    }

    // Attach the image to the window
    if (mlx_image_to_window(game.mlx, game.image, 0, 0) < 0)
    {
        fprintf(stderr, "Failed to attach image to window.\n");
        mlx_delete_image(game.mlx, game.image);
        mlx_terminate(game.mlx);
        return EXIT_FAILURE;
    }

    // Initialize player position and angle
    game.player.x = TILE_SIZE * 1.5f; // Center of the tile
    game.player.y = TILE_SIZE * 1.5f;
    game.player.angle = 0.0f; // Angle in radians
    game.mapGrid = map;
    printf("game__raycastengine%c\n", map->symbols[4][2]); 
    printf("game__raycastengine:%c\n", game.mapGrid->symbols[4][2]);
    // Register the update function
    mlx_loop_hook(game.mlx, &update, &game);

    // Register the key callback
    mlx_key_hook(game.mlx, &key_press, &game);

    // Start the MLX42 loop
    mlx_loop(game.mlx);

    // Clean up resources after the loop ends
    mlx_delete_image(game.mlx, game.image);
    mlx_terminate(game.mlx);

    return EXIT_SUCCESS;
}