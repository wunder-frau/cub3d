/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpilo <nkarpilo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 14:38:08 by nkarpilo          #+#    #+#             */
/*   Updated: 2024/11/25 14:57:14 by nkarpilo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	handle_rotation(t_game *game, float rot_speed)
{
	if (game->keys.left)
	{
		game->player.angle -= rot_speed;
		if (game->player.angle < 0)
			game->player.angle += 2 * M_PI;
	}
	if (game->keys.right)
	{
		game->player.angle += rot_speed;
		if (game->player.angle >= 2 * M_PI)
			game->player.angle -= 2 * M_PI;
	}
}

void	handle_movement(t_game *game, float move_speed)
{
	float	move_x;
	float	move_y;

	calculate_movement(game, move_speed, &move_x, &move_y);
	update_position(game, move_x, move_y);
}

void	calculate_movement(t_game *game, float move_speed, \
float *move_x, float *move_y)
{
	*move_x = 0.0f;
	*move_y = 0.0f;
	if (game->keys.w)
	{
		*move_x += cos(game->player.angle) * move_speed;
		*move_y += sin(game->player.angle) * move_speed;
	}
	if (game->keys.s)
	{
		*move_x -= cos(game->player.angle) * move_speed;
		*move_y -= sin(game->player.angle) * move_speed;
	}
	if (game->keys.a)
	{
		*move_x += cos(game->player.angle - M_PI / 2) * move_speed;
		*move_y += sin(game->player.angle - M_PI / 2) * move_speed;
	}
	if (game->keys.d)
	{
		*move_x += cos(game->player.angle + M_PI / 2) * move_speed;
		*move_y += sin(game->player.angle + M_PI / 2) * move_speed;
	}
}

void	update_position(t_game *game, float move_x, float move_y)
{
	float	magnitude;
	float	new_x;
	float	new_y;
	bool	can_move_x;
	bool	can_move_y;

	magnitude = sqrt(move_x * move_x + move_y * move_y);
	if (magnitude > 0)
	{
		move_x = (move_x / magnitude) * magnitude;
		move_y = (move_y / magnitude) * magnitude;
	}
	new_x = game->player.x + move_x;
	new_y = game->player.y + move_y;
	can_move_x = can_move_to(new_x, game->player.y, game);
	can_move_y = can_move_to(game->player.x, new_y, game);
	if (can_move_x)
		game->player.x = new_x;
	if (can_move_y)
		game->player.y = new_y;
}

bool	can_move_to(float x, float y, t_game *game)
{
	int				map_height;
	int				map_width;
	float			radius;
	t_map_coords	coords;

	map_height = game->map_grid->capacity;
	map_width = get_map_width(game);
	radius = 10.0f;
	coords.map_x1 = (int)((x - radius) / TILE_SIZE);
	coords.map_y1 = (int)((y - radius) / TILE_SIZE);
	coords.map_x2 = (int)((x + radius) / TILE_SIZE);
	coords.map_y2 = (int)((y + radius) / TILE_SIZE);
	if (coords.map_x1 < 0 || coords.map_x2 >= map_width || \
		coords.map_y1 < 0 || coords.map_y2 >= map_height)
		return (false);
	if (!is_walkable(game->map_grid->symbols[coords.map_y1][coords.map_x1]) ||
		!is_walkable(game->map_grid->symbols[coords.map_y1][coords.map_x2]) ||
		!is_walkable(game->map_grid->symbols[coords.map_y2][coords.map_x1]) ||
		!is_walkable(game->map_grid->symbols[coords.map_y2][coords.map_x2]))
		return (false);
	return (true);
}
