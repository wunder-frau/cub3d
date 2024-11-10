/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   aux_funcs.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpilo <nkarpilo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 14:37:19 by nkarpilo          #+#    #+#             */
/*   Updated: 2024/11/10 15:15:41 by nkarpilo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

int	get_map_width(t_game *game)
{
	int	i;
	int	map_width;
	int	line_length;
	int	map_height;

	i = 0;
	map_width = 0;
	map_height = game->map_grid->capacity;
	while (i < map_height)
	{
		line_length = ft_strlen(game->map_grid->symbols[i]);
		if (line_length > map_width)
			map_width = line_length;
		i++;
	}
	return (map_width);
}

bool	is_walkable(char c)
{
	return (c == '0' || is_player_symbol(c));
}

void	calculate_texture_coordinates(t_game *game, t_ray_data *ray_data)
{
	float	player_x_in_tiles;
	float	player_y_in_tiles;

	player_x_in_tiles = game->player.x / TILE_SIZE;
	player_y_in_tiles = game->player.y / TILE_SIZE;
	if (ray_data->side == 0)
		ray_data->wall_x = player_y_in_tiles + ray_data->perp_wall_dist * \
	ray_data->ray_dir_y;
	else
		ray_data->wall_x = player_x_in_tiles + ray_data->perp_wall_dist * \
	ray_data->ray_dir_x;
	ray_data->wall_x -= floor(ray_data->wall_x);
	ray_data->tex_x = (int)(ray_data->wall_x * ray_data->texture->width);
	if (ray_data->tex_x < 0)
		ray_data->tex_x = 0;
	if ((uint32_t)ray_data->tex_x >= ray_data->texture->width)
		ray_data->tex_x = ray_data->texture->width - 1;
	if (ray_data->side == 0 && ray_data->ray_dir_x > 0)
		ray_data->tex_x = ray_data->texture->width - ray_data->tex_x - 1;
	if (ray_data->side == 1 && ray_data->ray_dir_y < 0)
		ray_data->tex_x = ray_data->texture->width - ray_data->tex_x - 1;
	calculate_texture_position(ray_data);
}

void	calculate_texture_position(t_ray_data *ray_data)
{
	ray_data->step = 1.0f * ray_data->texture->height / ray_data->line_height;
	if (ray_data->draw_start > 0)
		ray_data->tex_pos = (ray_data->draw_start - HEIGHT / 2 + \
	ray_data->line_height / 2) * ray_data->step;
	else
		ray_data->tex_pos = (0 - HEIGHT / 2 + ray_data->line_height / 2) * \
	ray_data->step;
}
