/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dda.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpilo <nkarpilo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 14:37:29 by nkarpilo          #+#    #+#             */
/*   Updated: 2024/11/10 14:37:30 by nkarpilo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

int	perform_dda_loop(t_game *game, t_ray *ray)
{
	int	hit;

	hit = 0;
	ray->perp_wall_dist = 0.0f;
	while (hit == 0)
	{
		update_ray_position(ray);
		hit = check_out_of_bounds(game, ray);
		if (hit != 0)
			break ;
		hit = check_wall_hit(game, ray);
		if (ray->perp_wall_dist > MAX_VIEW_DISTANCE)
		{
			ray->perp_wall_dist = MAX_VIEW_DISTANCE;
			hit = 2;
			break ;
		}
	}
	return (hit);
}

int	check_out_of_bounds(t_game *game, t_ray *ray)
{
	int	map_width;

	map_width = ft_strlen(game->map_grid->symbols[0]);
	if (ray->map_x < 0 || ray->map_x >= map_width || \
	ray->map_y < 0 || ray->map_y >= game->map_grid->capacity)
	{
		ray->perp_wall_dist = MAX_VIEW_DISTANCE;
		return (2);
	}
	return (0);
}

int	check_wall_hit(t_game *game, t_ray *ray)
{
	char	symbol;

	symbol = game->map_grid->symbols[ray->map_y][ray->map_x];
	if (symbol == '1')
	{
		if (ray->side == 0)
			ray->perp_wall_dist = ray->side_dist_x - ray->delta_dist_x;
		else
			ray->perp_wall_dist = ray->side_dist_y - ray->delta_dist_y;
		return (1);
	}
	return (0);
}

void	update_ray_position(t_ray *ray)
{
	if (ray->side_dist_x < ray->side_dist_y)
	{
		ray->side_dist_x += ray->delta_dist_x;
		ray->map_x += ray->step_x;
		ray->side = 0;
	}
	else
	{
		ray->side_dist_y += ray->delta_dist_y;
		ray->map_y += ray->step_y;
		ray->side = 1;
	}
}
