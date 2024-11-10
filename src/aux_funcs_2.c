/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux_funcs_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpilo <nkarpilo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 14:37:25 by nkarpilo          #+#    #+#             */
/*   Updated: 2024/11/10 14:39:57 by nkarpilo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	calculate_step_x(t_game *game, t_ray *ray)
{
	float	player_x_in_tiles;

	player_x_in_tiles = game->player.x / TILE_SIZE;
	if (ray->ray_dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (player_x_in_tiles - ray->map_x) * ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1.0f - player_x_in_tiles) * \
		ray->delta_dist_x;
	}
}

void	calculate_step_y(t_game *game, t_ray *ray)
{
	float	player_y_in_tiles;

	player_y_in_tiles = game->player.y / TILE_SIZE;
	if (ray->ray_dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (player_y_in_tiles - ray->map_y) * ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0f - player_y_in_tiles) * \
		ray->delta_dist_y;
	}
}

uint32_t	rgb_to_uint32(int rgb[3])
{
	return ((rgb[0] << 24) | (rgb[1] << 16) | (rgb[2] << 8) | 255);
}

void	process_ray(t_game *game, int ray)
{
	t_ray_data	ray_data;

	initialize_ray(game, ray, &ray_data);
	perform_dda(game, &ray_data);
	correct_perp_wall_dist(game, &ray_data);
	calculate_line_height(&ray_data);
	choose_wall_texture(game, &ray_data);
	calculate_texture_coordinates(game, &ray_data);
	draw_wall_slice(game, ray, &ray_data);
}

void	draw_minimap_player_square(t_game *game, int player_px, int player_py)
{
	int	i;
	int	j;
	int	px;
	int	py;

	i = -2;
	while (i <= 2)
	{
		j = -2;
		while (j <= 2)
		{
			px = player_px + i;
			py = player_py + j;
			if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT)
				mlx_put_pixel(game->image, px, py, 0xFF0000FF);
			j++;
		}
		i++;
	}
}
