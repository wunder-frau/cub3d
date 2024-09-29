
#include "../cub3d.h"

float assign_player_angle(char symbol) {
    if (symbol == 'N' || symbol == 'n') {
        return 0.0f;    // North
    } else if (symbol == 'E' || symbol == 'e') {
        return 90.0f;   // East
    } else if (symbol == 'S' || symbol == 's') {
        return 180.0f;  // South
    } else if (symbol == 'W' || symbol == 'w') {
        return 270.0f;  // West
    }
    return 0.0f; // Default case, should not occur
}

// int find_player_pos(t_vector *map, t_player *player)
// {
// 	if (map == NULL || map->symbols == NULL)
// 	{
// 		fprintf(stderr, "Error: map or symbols not initialized.\n");
// 		return 0;
// 	}
// 	printf("hell");
// 	printf("helppppppp: %s\n", map->symbols[1]);
// 	printf("len______jjjjjjjjjj %zu\n", map->length);

// 	size_t row = 0;
// 	size_t col = 0;

// 	// Iterate through the map
// 	while (row < map->length)
// 	{
// 		// Ensure we stay within the bounds of the current row
// 		if (col < ft_strlen(map->symbols[row]))
// 		{
// 			// Check for player position
// 			char current_symbol = map->symbols[row][col];
// 			if (ft_strchr("NSEWnsew", current_symbol))
// 			{
// 				player->x = (float)col;
// 				player->y = (float)row;
// 				player->angle = assign_player_angle(current_symbol);
// 						 	printf("Player found at: x = %.2f, y = %.2f, angle = %.2f degrees\n", player->x, player->y, player->angle);
// 				return 1;
// 			}
// 		}
// 		else
// 		{
// 			row++;
// 			col = 0;
// 			continue;
// 			error_exit_cleanup("Player not found.\n", map, NULL);
// 		}
// 		col++;
// 	}
// 	return 0;
// }


//#include "../cub3d.h"

// Function to find the player position in the map
t_player find_player_pos(t_vector *map) {
    t_player player = {0}; // Initialize player with default values
    if (map == NULL || map->symbols == NULL) {
        fprintf(stderr, "Error: map or symbols not initialized.\n");
        player.x = -1; // Indicate an error
        player.y = -1; // Indicate an error
        player.angle = -1.0f; // Indicate an error
        return player; // Return player with error values
    }

    size_t row = 0;
    size_t col = 0;

    // Iterate through the map
    while (row < map->length) {
        // Ensure we stay within the bounds of the current row
        if (col < ft_strlen(map->symbols[row])) {
            // Check for player position
            char current_symbol = map->symbols[row][col];
            if (ft_strchr("NSEWnsew", current_symbol)) {
                // player.x = (float)col;  // Set player's x position
                // player.y = (float)row;  // Set player's y position
								// player.x = TILE_SIZE + (TILE_SIZE / 1.5f);  // Center X
                // player.y = TILE_SIZE + (TILE_SIZE / 1.5f); 
								player.x = (float)col * (TILE_SIZE / 2.0f);  // Center X
								player.y = (float)row * (TILE_SIZE / 2.0f);
                player.angle = assign_player_angle(current_symbol); // Assign angle

                // printf("Player found at: x = %.2f, y = %.2f, angle = %.2f degrees\n", player.x, player.y, player.angle);
                return player; // Return the player struct
            }
        } else {
            row++;
            col = 0;
            continue; // Continue to the next row if out of bounds
        }
        col++;
    }

    // If player not found, return a player with error values
    player.x = -1; // Indicate player not found
    player.y = -1; // Indicate player not found
    player.angle = -1.0f; // Indicate player not found
    fprintf(stderr, "Error: Player not found in the map.\n");
    return player; // Return player with error values
}
