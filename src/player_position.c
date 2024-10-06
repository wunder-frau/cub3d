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

static bool is_player_symbol(char c)
{
	return (c == 'N' || c == 'n' ||
					c == 'S' || c == 's' ||
					c == 'E' || c == 'e' ||
					c == 'W' || c == 'w');
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
