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
	draw_line(img, 100, 100, 500, 300, 0x00FF00FF);
	mlx_loop(mlx);
	mlx_terminate(mlx);
	return (EXIT_SUCCESS);
}
