/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpilo <nkarpilo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 14:38:21 by nkarpilo          #+#    #+#             */
/*   Updated: 2024/11/10 14:38:22 by nkarpilo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

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
