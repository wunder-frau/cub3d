/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_position.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:06:31 by istasheu          #+#    #+#             */
/*   Updated: 2024/11/25 10:25:26 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static int	find_player(t_vector *map)
{
	size_t	row;
	size_t	col;
	int		player_count;
	char	current_symbol;

	row = 0;
	player_count = 0;
	while (row < map->capacity)
	{
		if (map->symbols[row] == NULL)
			error_exit_cleanup("Row in the map is NULL.", map, NULL);
		col = 0;
		while (col < ft_strlen(map->symbols[row]))
		{
			current_symbol = map->symbols[row][col];
			if (is_player_symbol(current_symbol))
				player_count++;
			else
				if (!is_valid_map_symbol(current_symbol))
					error_exit_cleanup("Invalid symbol.", map, NULL);
			col++;
		}
		row++;
	}
	return (player_count);
}

static bool	assign_player_coordinates(size_t row,
			size_t col, t_player *player, t_vector *map)
{
	char	current_symbol;

	current_symbol = map->symbols[row][col];
	if (is_player_symbol(current_symbol))
	{
		player->x = (float)col * TILE_SIZE + (TILE_SIZE / 2.0f);
		player->y = (float)row * TILE_SIZE + (TILE_SIZE / 2.0f);
		player->angle = assign_player_angle(current_symbol);
		return (true);
	}
	else if (!is_valid_map_symbol(current_symbol))
	{
		error_exit_cleanup("Invalid symbol", map, NULL);
	}
	return (false);
}

int	count_players(t_vector *map)
{
	int	player_count;

	if (map == NULL || map->symbols == NULL)
	{
		error_exit_cleanup("Map is not initialized.", map, NULL);
	}
	player_count = find_player(map);
	if (player_count == 0)
	{
		error_exit_cleanup("No player found in the map.", map, NULL);
	}
	else if (player_count > 1)
	{
		error_exit_cleanup("Multiple players are found.", map, NULL);
	}
	return (player_count);
}

/**
 * Function to find the player position in the map
 */
t_player	find_player_pos(t_vector *map)
{
	t_player	player;
	size_t		row;
	size_t		col;

	player = (t_player){0};
	if (map == NULL || map->symbols == NULL)
	{
		return (set_player_not_found("Error: map or symbols not initialized."));
	}
	row = 0;
	while (row < map->length)
	{
		col = 0;
		while (col < ft_strlen(map->symbols[row]))
		{
			if (assign_player_coordinates(row, col, &player, map))
				return (player);
			col++;
		}
		row++;
	}
	return (set_player_not_found("Error: Player not found in the map."));
}
