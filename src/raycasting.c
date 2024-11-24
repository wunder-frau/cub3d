/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycasting.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 14:38:17 by nkarpilo          #+#    #+#             */
/*   Updated: 2024/11/24 19:47:39 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	initialize_ray(t_game *game, int ray, t_ray_data *ray_data)
{
	float	fov_rad;
	float	player_x_in_tiles;
	float	player_y_in_tiles;

	fov_rad = FOV * M_PI / 180.0f;
	init_ray_angle(game, ray, fov_rad, &(ray_data->ray_angle));
	init_ray_direction(ray_data->ray_angle, &(ray_data->ray_dir_x), \
	&(ray_data->ray_dir_y));
	player_x_in_tiles = game->player.x / TILE_SIZE;
	player_y_in_tiles = game->player.y / TILE_SIZE;
	ray_data->map_x = (int)(player_x_in_tiles);
	ray_data->map_y = (int)(player_y_in_tiles);
}

void	perform_dda(t_game *game, t_ray_data *ray_data)
{
	t_ray	ray;

	ray.ray_dir_x = ray_data->ray_dir_x;
	ray.ray_dir_y = ray_data->ray_dir_y;
	ray.map_x = ray_data->map_x;
	ray.map_y = ray_data->map_y;
	calculate_step_and_side_dist(game, &ray);
	perform_dda_loop(game, &ray);
	ray_data->map_x = ray.map_x;
	ray_data->map_y = ray.map_y;
	ray_data->side = ray.side;
	ray_data->perp_wall_dist = ray.perp_wall_dist;
}

void	correct_perp_wall_dist(t_game *game, t_ray_data *ray_data)
{
	float	angle_diff;

	angle_diff = ray_data->ray_angle - game->player.angle;
	if (angle_diff < -M_PI)
		angle_diff += 2 * M_PI;
	if (angle_diff > M_PI)
		angle_diff -= 2 * M_PI;
	ray_data->corrected_perp_wall_dist = ray_data->perp_wall_dist * \
	cos(angle_diff);
}

void	calculate_line_height(t_ray_data *ray_data)
{
	ray_data->line_height = (int)(HEIGHT / ray_data->corrected_perp_wall_dist);
	ray_data->draw_start = -ray_data->line_height / 2 + HEIGHT / 2;
	if (ray_data->draw_start < 0)
		ray_data->draw_start = 0;
	ray_data->draw_end = ray_data->line_height / 2 + HEIGHT / 2;
	if (ray_data->draw_end >= HEIGHT)
		ray_data->draw_end = HEIGHT - 1;
}

void	choose_wall_texture(t_game *game, t_ray_data *ray_data)
{
	if (ray_data->side == 0)
	{
		if (ray_data->ray_dir_x < 0)
			ray_data->texture = game->assets.textures.WE;
		else
			ray_data->texture = game->assets.textures.EA;
	}
	else
	{
		if (ray_data->ray_dir_y < 0)
			ray_data->texture = game->assets.textures.SO;
		else
			ray_data->texture = game->assets.textures.no;
	}
}
