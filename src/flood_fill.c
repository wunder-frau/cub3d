#include "../cub3d.h"

// Use this function to find the starting position of the player
bool find_symbol_position(t_vector *map, const char *symbols, int *row_out, int *col_out)
{
    int row = 0;
    int col = 0;

    while (row < map->length)
    {
        col = 0;
        while (col < ft_strlen(map->symbols[row]))
        {
            if (ft_strchr(symbols, map->symbols[row][col]))
            {
                if (row_out != NULL && col_out != NULL)
                {
                    *row_out = row;
                    *col_out = col;
                }
                return (true);
            }
            col++;
        }
        row++;
    }
    return (false);
}

// The flood fill function is used to mark all reachable cells starting from a given cell
void flood_fill_vector(t_vector *map, int row, int col)
{
    // Check boundary conditions to prevent out-of-bounds access
    if (row < 0 || row >= map->length || col < 0 || col >= ft_strlen(map->symbols[row]))
        return;

    // Perform flood fill only for player symbols, '0', or empty spaces ' '
    if (is_player_symbol(map->symbols[row][col]) || map->symbols[row][col] == ' ' || map->symbols[row][col] == '0')
    {
        map->symbols[row][col] = '*';  // Mark the current cell as visited

        // Call flood fill recursively in four directions
        flood_fill_vector(map, row, col - 1); // Left
        flood_fill_vector(map, row, col + 1); // Right
        flood_fill_vector(map, row - 1, col); // Up
        flood_fill_vector(map, row + 1, col); // Down
    }
}

// Function to validate the entire map using flood fill from the player's starting position
void validate_map_with_flood_fill(t_vector *map)
{
    t_vector *dup;
    int row, col;

    // Duplicate the map to perform flood fill without modifying the original map
    dup = vector_copy(map);
    if (!dup->symbols)
    {
        ft_putstr_fd("Error\nMemory allocation failed for map duplication\n", 2);
        exit(1);
    }

    // Find the player's starting position ('N', 'W', 'S', 'E')
    if (!find_symbol_position(dup, "NWSE", &row, &col))
    {
        ft_putstr_fd("Error\nInitial position not found in the map\n", 2);
        vector_free(dup);
        exit(1);
    }

    // Validate the map using flood fill
    if (!is_valid(dup, row, col))
    {
        vector_free(dup);
        ft_putstr_fd("Error\nMap is not enclosed properly.\n", 2);
        exit(1);
    }

    // Debug: Print the map after flood fill
    printf("HERE IS DUP____\n");
    vector_print(dup);

    // Debug: Successfully validated map
    printf("DEBUG: Map successfully passed flood fill validation.\n");

    // Free the duplicated map after validation is complete
    vector_free(dup);
}
// // Function to validate the entire map using flood fill from the player's starting position
// void validate_map_with_flood_fill(t_vector *map)
// {
//     t_vector *dup;
//     int row, col, i, j;

//     // Duplicate the map to perform flood fill without modifying the original map
//     dup = vector_copy(map);
//     if (!dup->symbols)
//     {
//         ft_putstr_fd("Error\nMemory allocation failed for map duplication\n", 2);
//         exit(1);
//     }

//     // Find the player's starting position ('N', 'W', 'S', 'E')
//     if (!find_symbol_position(dup, "NWSE", &row, &col))
//     {
//         ft_putstr_fd("Error\nInitial position not found in the map\n", 2);
//         vector_free(dup);
//         exit(1);
//     }

//     // Start flood fill from the player's position
//     //flood_fill_vector(dup, row, col);
//     is_valid(map, row, col);
//     // Validate that all cells are either walls or visited ('1' or '*')
//     // for (i = 0; i < dup->length; i++)
//     // {
//     //     for (j = 0; j < ft_strlen(dup->symbols[i]); j++)
//     //     {
//     //         // Debug information to track the cells being validated
//     //         printf("DEBUG: Checking cell at row %d, col %d: '%c'\n", i, j, dup->symbols[i][j]);

// 	// 					if (dup->symbols[i][j] == '0')
// 	// 					{
// 	// 						find_symbol_position(dup, "0", &i, &j);
// 	// 						flood_fill_vector(dup, i, j);
// 	// 					}
//     //         // If any cell is not a wall ('1'), visited ('*'), or space (' '), return an error
//     //         else if (dup->symbols[i][j] != '1' && dup->symbols[i][j] != '*' && dup->symbols[i][j] != ' ' && dup->symbols[i][j] != '\n')
//     //         {
//     //             ft_putstr_fd("Error\nShould be only obstacles and spaces.\n", 2);
//     //             vector_free(dup);
//     //             exit(1);
//     //         }
//     //     }
//     // }

//     // Debug: Print the map after flood fill
//     printf("HERE IS DUP____\n");
//     vector_print(dup);

//     // Free the duplicated map after validation is complete
//     vector_free(dup);
// }

// #include "../cub3d.h"

// static void	flood_fill_vector(t_vector *map, int row, int col)
// {
// 	if (row < 0 || row >= map->length || col < 0 || col >= ft_strlen(map->symbols[row]))
// 		return ;
// 	if (is_player_symbol(map->symbols[row][col]) ||
// 								map->symbols[row][col] == ' ' ||
// 								map->symbols[row][col] == '0')
// 	{
// 		map->symbols[row][col] = '*';
// 		flood_fill_vector(map, row, col - 1);
// 		flood_fill_vector(map, row, col + 1);
// 		flood_fill_vector(map, row - 1, col);
// 		flood_fill_vector(map, row + 1, col);
// 	}
// }

// bool	find_symbol_position(t_vector *map, const char *symbols, int *row_out, int *col_out)
// {
// 	int row = 0;
// 	int col = 0;

// 	while (row < map->length)
// 	{
// 		col = 0;
// 		while (col < ft_strlen(map->symbols[row]))
// 		{
// 			if (ft_strchr(symbols, map->symbols[row][col]))
// 			{
// 				if (row_out != NULL && col_out != NULL)
// 				{
// 					*row_out = row;
// 					*col_out = col;
// 				}
// 				return (true);
// 			}
// 			col++;
// 		}
// 		row++;
// 	}
// 	return (false);
// }

// void	validate_path(t_vector *map_ptr)
// {
// 	t_vector	*dup;
// 	int	row, col, i, j;

// 	dup = vector_copy(map_ptr);
// 	if (!dup->symbols)
// 	{
// 		ft_putstr_fd("Error\nMemory allocation failed for map duplication\n", 2);
// 		exit(1);
// 	}
// 	if (!find_symbol_position(dup, "NWSE", &row, &col))
// 	{
// 		ft_putstr_fd("Error\nInitial position not found in the map\n", 2);
// 		vector_free(dup);
// 		exit(1);
// 	}
// 	flood_fill_vector(dup, row - 1, col); // `row` was incremented during search
// 	printf("HERE IS DUP____\n");
// 	vector_print(dup);
// 	i = 0;
// 	while( i < dup->length)
// 	{
// 		j = 0;
// 		while(j < ft_strlen(dup->symbols[i]))
// 		{
// 			printf("lololo_:%c\n",dup->symbols[i][j]);
// 			if (dup->symbols[i][j] != '1' && dup->symbols[i][j] != '*' && dup->symbols[i][j] != ' ' && dup->symbols[i][j] != '\n')
// 			{
// 				ft_putstr_fd("Error\nShould be only obstacles and spaces.\n", 2);
// 				vector_free(dup);
// 				exit(1);
// 			}
// 			j++;
// 		}
// 		i++;
// 	}
// 	vector_free(dup);
// }
