#include "MLX42/include/MLX42/MLX42.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

void	draw_line(mlx_image_t* img, int x0, int y0, int x1, int y1, uint32_t color)
{
	int dx = abs(x1 - x0);
	int dy = -abs(y1 - y0);
	int sx = x0 < x1 ? 1 : -1;
	int sy = y0 < y1 ? 1 : -1;
	int err = dx + dy;
	while (1)
	{
		mlx_put_pixel(img, x0, y0, color);
		if (x0 == x1 && y0 == y1)
			break;
		int e2 = 2 * err;
		if (e2 >= dy) { err += dy; x0 += sx; }
		if (e2 <= dx) { err += dx; y0 += sy; }
	}
}

void draw_rectangle(mlx_image_t* img, int x, int y, int width, int height, uint32_t color)
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			mlx_put_pixel(img, x + i, y + j, color);
		}
	}
}

void draw_triangle(mlx_image_t* img, int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color)
{
	draw_line(img, x1, y1, x2, y2, color);
	draw_line(img, x2, y2, x3, y3, color);
	draw_line(img, x3, y3, x1, y1, color);
}

void draw_circle(mlx_image_t* img, int xc, int yc, int radius, uint32_t color)
{
	int	x = radius;
	int	y = 0;
	int	err = 0;
	while (x >= y)
	{
		mlx_put_pixel(img, xc + x, yc + y, color);
		mlx_put_pixel(img, xc + y, yc + x, color);
		mlx_put_pixel(img, xc - y, yc + x, color);
		mlx_put_pixel(img, xc - x, yc + y, color);
		mlx_put_pixel(img, xc - x, yc - y, color);
		mlx_put_pixel(img, xc - y, yc - x, color);
		mlx_put_pixel(img, xc + y, yc - x, color);
		mlx_put_pixel(img, xc + x, yc - y, color);

		if (err <= 0)
		{
			y += 1;
			err += 2*y + 1;
		}
		if (err > 0)
		{
			x -= 1;
			err -= 2*x + 1;
		}
	}
}

void draw_gradient(mlx_image_t* img)
{
	for (uint32_t y = 0; y < img->height; y++)
	{
		for (uint32_t x = 0; x < img->width; x++)
		{
			// Calculate the red and green color values based on position
			uint8_t red = (uint8_t)(x * 255 / img->width);    // Gradient in the red channel
			uint8_t green = (uint8_t)(y * 255 / img->height); // Gradient in the green channel
			uint8_t blue = 128;                              // Constant blue value for contrast

			// Combine the RGB values into a single 32-bit color value (ARGB format)
			uint32_t color = (255 << 24) | (red << 16) | (green << 8) | blue; // 255 for full alpha

			// Draw the pixel with the calculated color
			mlx_put_pixel(img, x, y, color);
		}
	}
}

void	close_window(void* param)
{
	mlx_t* mlx = param;
	mlx_close_window(mlx);
}

void	handle_key(mlx_key_data_t keydata, void* param)
{
	mlx_t* mlx = param;
	if (keydata.key == MLX_KEY_ESCAPE && keydata.action == MLX_PRESS)
		mlx_close_window(mlx);
}

int32_t	main(void)
{
	mlx_t*	mlx = mlx_init(800, 600, "Cub3D", true);
	if (!mlx)
		return (EXIT_FAILURE);
	// Create an image buffer to draw on
	mlx_image_t*	img = mlx_new_image(mlx, 800, 600);
	if (!img)
		return (EXIT_FAILURE);
	mlx_image_to_window(mlx, img, 0, 0);
	mlx_close_hook(mlx, &close_window, mlx);
	mlx_key_hook(mlx, &handle_key, mlx);
	draw_gradient(img);
	draw_rectangle(img, 50, 50, 200, 100, 0xA0A0A0FF);
	draw_line(img, 100, 100, 500, 300, 0x00FF00FF);
	draw_triangle(img, 600, 100, 700, 200, 500, 200, 0xFFFF00FF);
	draw_circle(img, 400, 300, 50, 0x0000FFFF);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (EXIT_SUCCESS);
}
