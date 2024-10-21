#include "../cub3d.h"

static float	assign_player_angle(char symbol)
{
	if (symbol == 'N' || symbol == 'n')
		return (0.0f);
	else if (symbol == 'E' || symbol == 'e')
		return (90.0f);
	else if (symbol == 'S' || symbol == 's')
		return (180.0f);
	else if (symbol == 'W' || symbol == 'w')
		return (270.0f);
	return (0.0f);
}

bool	is_player_symbol(char c)
{
	return (c == 'N' || c == 'n'
		|| c == 'S' || c == 's'
		|| c == 'E' || c == 'e'
		|| c == 'W' || c == 'w');
}

bool	is_valid_map_symbol(char c, t_vector *map)
{
	char	*valid_symbols;

	valid_symbols = "01NSEW";
	return (ft_strchr(valid_symbols, c) != NULL
		|| c == '\n' || c == '\0' || c == ' ');
}

int	find_player(t_vector *map)
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
				if (!is_valid_map_symbol(current_symbol, map))
					error_exit_cleanup("Invalid symbol.", map, NULL);
			col++;
		}
		row++;
	}
	return (player_count);
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

bool	assign_player_coordinates(size_t row,
			size_t col, t_player *player, t_vector *map)
{
	char	current_symbol;

	current_symbol = map->symbols[row][col];
	if (is_player_symbol(current_symbol))
	{
		player->x = (float)col * TILE_SIZE + (TILE_SIZE / 2.0f);
		player->y = (float)row * TILE_SIZE + (TILE_SIZE / 2.0f);
		player->angle = assign_player_angle(current_symbol);
		printf("player_pos:__%.2f, %.2f\n", player->x, player->y);
		return (true);
	}
	else if (!is_valid_map_symbol(current_symbol, map))
	{
		error_exit_cleanup("Invalid symbol", map, NULL);
	}
	return (false);
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
