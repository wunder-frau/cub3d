
#include "../cub3d.h"

int find_player_pos(t_vector *map)
{
	t_player *player;
	if (map == NULL || map->symbols == NULL)
	{
		fprintf(stderr, "Error: map or symbols not initialized.\n");
		return 0;
	}
	printf("hell");
	printf("helppppppp: %s\n", map->symbols[1]);
	printf("len______jjjjjjjjjj %zu\n", map->length);

	size_t row = 0;
	size_t col = 0;

	// Iterate through the map
	while (row < map->length)
	{
		// Ensure we stay within the bounds of the current row
		if (col < ft_strlen(map->symbols[row]))
		{
			// Check for player position
			if (ft_strchr("NSEWnsew", map->symbols[row][col]))
			{
				player->x = (float)col;
				player->y = (float)row;
				player->angle = 0.0f;
				printf("Player found at: x = %.2f, y = %.2f\n", player->x, player->y);
				return 1;
			}
			 error_exit_cleanup("Player not found.\n", map, NULL);
		}
		else
		{
			row++;
			col = 0;
			continue;
		}
		col++;
	}
	return 0;
}
