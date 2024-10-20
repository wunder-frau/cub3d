#include "../cub3d.h"

static float assign_player_angle(char symbol)
{
	if (symbol == 'N' || symbol == 'n')
		return 0.0f;
	else if (symbol == 'E' || symbol == 'e')
		return 90.0f;
	else if (symbol == 'S' || symbol == 's')
		return 180.0f;
	else if (symbol == 'W' || symbol == 'w')
		return 270.0f; 
	return 0.0f;
}

bool is_player_symbol(char c)
{
	return (c == 'N' || c == 'n' ||
					c == 'S' || c == 's' ||
					c == 'E' || c == 'e' ||
					c == 'W' || c == 'w');
}

bool	is_valid_map_symbol(char c, t_vector *map)
{
	const char	valid_symbols[] = "01NSEW";
	return (ft_strchr(valid_symbols, c) != NULL || c == '\n' || c == '\0' || c == ' ');
}

int	count_players(t_vector *map)
{
	size_t row = 0;
	size_t col = 0;
	int player_count = 0;

	if (map == NULL || map->symbols == NULL)
	{
		log_error_message("Error: Map or symbols array is not initialized.");
		return (-1);
	}
	while (row < map->capacity)
	{
		if (map->symbols[row] == NULL)
		{
			fprintf(stderr, "Error: Row %zu in the map is NULL.\n", row);
			error_exit_cleanup("Row in the map is NULL.", map, NULL);
		}
		col = 0;
		while (col < ft_strlen(map->symbols[row]))
		{
			char current_symbol = map->symbols[row][col];
			//validate_map_symbol(current_symbol, row, col, map);
			if (is_player_symbol(current_symbol))
			{
				player_count++;
				if (player_count > 1)
				{
					log_error_message("Error: Multiple player symbols found in the map.");
					error_exit_cleanup("Multiple player symbols found in the map.", map, NULL);
				}
			}
			else if (!is_valid_map_symbol(current_symbol, map))
			{
				fprintf(stderr, "Error: Invalid symbol '%c' found at row %zu, col %zu\n", current_symbol, row, col);
				error_exit_cleanup("Invalid symbol found in the map.", map, NULL);
			}
			col++;
		}
		row++;
	}
	printf("player_count:[%d]\n", player_count);
	if (player_count == 0)
	{
		log_error_message("Error: No player symbol found in the map.");
		error_exit_cleanup("No player found in the map.", map, NULL);
	}
	return (player_count);
}

/**
 * Function to find the player position in the map
 */

t_player	find_player_pos(t_vector *map)
{
	t_player player = {0};
	size_t row = 0;
	size_t col = 0;

	if (map == NULL || map->symbols == NULL)
	{
			fprintf(stderr, "Error: map or symbols not initialized.\n");
			player.x = -1;
			player.y = -1;
			player.angle = -1.0f;
			return player;
	}
	while (row < map->length)
	{
		col = 0;
		while (col < ft_strlen(map->symbols[row]))
		{
			char current_symbol = map->symbols[row][col];
			if (is_player_symbol(current_symbol))
			{
				player.x = (float)col * TILE_SIZE + (TILE_SIZE / 2.0f);
				player.y = (float)row * TILE_SIZE + (TILE_SIZE / 2.0f);
				player.angle = assign_player_angle(current_symbol);
				printf("player_pos:__%.2f, %.2f\n", player.x, player.y);
				return (player);
			}
			else if (!is_valid_map_symbol(current_symbol, map))
			{
				error_exit_cleanup("Invalid symbol", map, NULL);
			}
			col++;
		}
		row++;
	}
	player.x = -1;
	player.y = -1;
	player.angle = -1.0f;
	fprintf(stderr, "Error: Player not found in the map.\n");
	return (player);
}
