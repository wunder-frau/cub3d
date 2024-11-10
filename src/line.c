/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   line.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpilo <nkarpilo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 14:37:42 by nkarpilo          #+#    #+#             */
/*   Updated: 2024/11/10 14:37:46 by nkarpilo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

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
