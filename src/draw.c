/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   draw.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpilo <nkarpilo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 14:37:32 by nkarpilo          #+#    #+#             */
/*   Updated: 2024/11/10 15:11:25 by nkarpilo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

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
				if (!color)
					color = 0;
				mlx_put_pixel(game->image, px, py, color);
			}
			x++;
		}
		y++;
	}
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
	size_t		y;
	size_t		x;
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
