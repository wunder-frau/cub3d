#include "../cub3d.h"

bool	is_symbol(t_vector *map, char *symbols)
{
	int	row;
	int	col;

	row = 0;
	while(row < map->length)
	{
		col = 0;
		while(col < ft_strlen(map->symbols[row]))
		{
			if (ft_strchr(symbols, map->symbols[row][col]))
			{
				printf("DEBUG: Player found at row %d, col %d\n", row, col);
				return (true);
			}
			col++;
		}
		row++;
	}
	return	(false);
}

static bool	check_zero_cells_surrounded(t_vector *map)
{
	size_t y = 0;
	size_t x;
	size_t row_len;

	while (y < map->capacity)
	{
		row_len = ft_strlen(map->symbols[y]);
		x = 0;
		while (x < row_len)
		{
			printf("Checking cell at y: %zu, x: %zu, value: %c\n", y, x, map->symbols[y][x]);
			if (map->symbols[y][x] == '0')
			{
				printf("Found '0' at y: %zu, x: %zu\n", y, x);
				if (y == 0 || y == map->capacity - 1 ||
						x == 0 || x == row_len - 1 ||
						map->symbols[y - 1][x] == ' ' ||
						map->symbols[y + 1][x] == ' ' ||
						(x > 0 && map->symbols[y][x - 1] == ' ') ||
						(x < row_len - 1 && map->symbols[y][x + 1] == ' '))
					{
						printf("Error: '0' at y: %zu, x: %zu is not properly enclosed.\n", y, x);
						return false;
					}
					else
					{
						printf("'0' at y: %zu, x: %zu is properly enclosed.\n", y, x);
					}
			}
			x++;
		}
		y++;
	}
	printf("All '0' cells are properly enclosed.\n");
	return true;
}

bool validate_map_form(t_vector *map)
{
	printf("DEBUG: Starting map shape validation...\n");
	if (!is_symbol(map, "NWSE"))
	{
		printf("DEBUG: No player symbol (N, W, S, E) found in the map.\n");
		return (false);
	}
	printf("DEBUG: Player symbol found, proceeding with validation.\n");
	if (check_zero_cells_surrounded(map) == false)
		return (printf("Map must be surrounded by walls"));
	printf("print:");
	vector_print(map);
	return (true);
}