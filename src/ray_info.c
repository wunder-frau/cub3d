/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_info.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpilo <nkarpilo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 14:38:11 by nkarpilo          #+#    #+#             */
/*   Updated: 2024/11/10 14:38:12 by nkarpilo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	initialize_ray_info(t_game *game, float ray_angle, t_ray_info *ray_info)
{
	ray_info->ray_x = game->player.x;
	ray_info->ray_y = game->player.y;
	ray_info->ray_dir_x = cos(ray_angle);
	ray_info->ray_dir_y = sin(ray_angle);
	ray_info->hit = 0;
	ray_info->total_distance = 0.0f;
	ray_info->step_size = 1.0f;
}

void	perform_ray_casting(t_game *game, t_minimap *minimap, \
t_ray_info *ray_info)
{
	while (!ray_info->hit)
	{
		ray_info->ray_x += ray_info->ray_dir_x * ray_info->step_size;
		ray_info->ray_y += ray_info->ray_dir_y * ray_info->step_size;
		ray_info->total_distance += ray_info->step_size;
		ray_info->map_x = (int)(ray_info->ray_x / TILE_SIZE);
		ray_info->map_y = (int)(ray_info->ray_y / TILE_SIZE);
		if (ray_info->map_x < 0 || ray_info->map_x >= minimap->map_width || \
			ray_info->map_y < 0 || ray_info->map_y >= minimap->map_height)
			ray_info->hit = 1;
		else if (game->map_grid->symbols[ray_info->map_y][ray_info->map_x] \
		== '1')
			ray_info->hit = 1;
		else if (ray_info->total_distance >= MAX_VIEW_DISTANCE)
			ray_info->hit = 1;
	}
}

void	compute_end_coordinates(t_minimap *minimap, t_ray_info *ray_info, \
t_point *end_point)
{
	end_point->x = minimap->offset_x + (ray_info->ray_x / \
	TILE_SIZE) * minimap->scale;
	end_point->y = minimap->offset_y + (ray_info->ray_y / \
	TILE_SIZE) * minimap->scale;
}

void	calculate_ray_end(t_game *game, t_minimap *minimap, \
float ray_angle, t_point *end_point)
{
	t_ray_info	ray_info;

	initialize_ray_info(game, ray_angle, &ray_info);
	perform_ray_casting(game, minimap, &ray_info);
	compute_end_coordinates(minimap, &ray_info, end_point);
}
