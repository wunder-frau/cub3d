/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_utils.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpilo <nkarpilo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 14:38:14 by nkarpilo          #+#    #+#             */
/*   Updated: 2024/11/10 14:38:15 by nkarpilo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	init_ray_angle(t_game *game, int ray, float fov_rad, float *ray_angle)
{
	float	angle_increment;
	float	start_angle;

	angle_increment = fov_rad / NUM_RAYS;
	start_angle = game->player.angle - (fov_rad / 2.0f);
	if (start_angle < 0)
		start_angle += 2 * M_PI;
	if (start_angle > 2 * M_PI)
		start_angle -= 2 * M_PI;
	*ray_angle = start_angle + ray * angle_increment;
	if (*ray_angle < 0)
		*ray_angle += 2 * M_PI;
	if (*ray_angle > 2 * M_PI)
		*ray_angle -= 2 * M_PI;
}

void	init_ray_direction(float ray_angle, float *ray_dir_x, float *ray_dir_y)
{
	*ray_dir_x = cos(ray_angle);
	*ray_dir_y = sin(ray_angle);
}

void	calculate_delta_distances(t_ray *ray)
{
	if (ray->ray_dir_x == 0)
		ray->delta_dist_x = 2147483647;
	else
		ray->delta_dist_x = fabs(1.0f / ray->ray_dir_x);
	if (ray->ray_dir_y == 0)
		ray->delta_dist_y = 2147483647;
	else
		ray->delta_dist_y = fabs(1.0f / ray->ray_dir_y);
}

void	calculate_step_and_side_dist(t_game *game, t_ray *ray)
{
	calculate_delta_distances(ray);
	calculate_step_x(game, ray);
	calculate_step_y(game, ray);
}

void	correct_and_compute_wall(float *perp_wall_dist, float ray_angle, \
t_game *game, int *line_height)
{
	float	angle_diff;

	angle_diff = ray_angle - game->player.angle;
	if (angle_diff < -M_PI)
		angle_diff += 2 * M_PI;
	if (angle_diff > M_PI)
		angle_diff -= 2 * M_PI;
	*perp_wall_dist *= cos(angle_diff);
	if (*perp_wall_dist < 0.0001f)
		*perp_wall_dist = 0.0001f;
	*line_height = (int)(HEIGHT / (*perp_wall_dist));
	if (*line_height > HEIGHT)
		*line_height = HEIGHT;
	if (*line_height < 1)
		*line_height = 1;
}
