/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 14:37:48 by nkarpilo          #+#    #+#             */
/*   Updated: 2024/11/14 17:08:21 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	draw_minimap(t_game *game)
{
	t_minimap	minimap;
	float		scale_x;
	float		scale_y;
	int			ray;

	minimap.map_height = game->map_grid->capacity;
	minimap.map_width = get_map_width(game);
	minimap.offset_x = 10;
	minimap.offset_y = 10;
	scale_x = 200.0f / (float)minimap.map_width;
	scale_y = 200.0f / (float)minimap.map_height;
	if (scale_x < scale_y)
		minimap.scale = scale_x;
	else
		minimap.scale = scale_y;
	draw_minimap_grid(game, &minimap);
	draw_minimap_player(game, &minimap);
	ray = 0;
	draw_minimap_rays(game, &minimap, ray);
}

void	draw_minimap_grid(t_game *game, t_minimap *minimap)
{
	int	y;
	int	x;
	int	line_length;

	y = 0;
	while (y < minimap->map_height)
	{
		line_length = ft_strlen(game->map_grid->symbols[y]);
		if (game->map_grid->symbols[y][line_length - 1] == '\n')
		{
			line_length -= 1;
		}
		x = 0;
		while (x < line_length)
		{
			process_minimap_tile(game, minimap, x, y);
			x++;
		}
		y++;
	}
}

void	draw_minimap_player(t_game *game, t_minimap *minimap)
{
	float	player_x;
	float	player_y;
	int		player_px;
	int		player_py;

	player_x = minimap->offset_x + (game->player.x / TILE_SIZE) * \
	minimap->scale;
	player_y = minimap->offset_y + (game->player.y / TILE_SIZE) * \
	minimap->scale;
	player_px = (int)player_x;
	player_py = (int)player_y;
	draw_minimap_player_square(game, player_px, player_py);
}

void	draw_minimap_rays(t_game *game, t_minimap *minimap, int ray)
{
	float	fov_rad;
	float	angle_increment;
	float	start_angle;
	float	ray_angle;

	fov_rad = FOV * M_PI / 180.0f;
	angle_increment = fov_rad / NUM_RAYS;
	start_angle = game->player.angle - (fov_rad / 2.0f);
	if (start_angle < 0)
		start_angle += 2 * M_PI;
	if (start_angle > 2 * M_PI)
		start_angle -= 2 * M_PI;
	while (ray < NUM_RAYS)
	{
		if (ray % 10 == 0)
		{
			ray_angle = start_angle + ray * angle_increment;
			if (ray_angle < 0)
				ray_angle += 2 * M_PI;
			if (ray_angle > 2 * M_PI)
				ray_angle -= 2 * M_PI;
			process_minimap_ray(game, minimap, ray_angle);
		}
		ray++;
	}
}

void	process_minimap_ray(t_game *game, t_minimap *minimap, \
float ray_angle)
{
	t_point		end_point;
	t_point		p0;
	t_point		p1;

	calculate_ray_end(game, minimap, ray_angle, &end_point);
	p0.x = minimap->offset_x + (game->player.x / TILE_SIZE) * minimap->scale;
	p0.y = minimap->offset_y + (game->player.y / TILE_SIZE) * minimap->scale;
	p1.x = (int)end_point.x;
	p1.y = (int)end_point.y;
	draw_line(game->image, p0, p1, 0x0000FFFF);
}
