#include "../cub3d.h"

bool	find_symbol_position(t_vector *map, const char *symbols,
	int *row_out, int *col_out)
{
	int	row;
	int	col;

	row = 0;
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

void	flood_fill(t_vector *map, int row, int col)
{
	if (row < 0 || row >= map->length || col < 0
		|| col >= ft_strlen(map->symbols[row]))
		return ;
	if (is_player_symbol(map->symbols[row][col]) ||
			map->symbols[row][col] == ' ' || map->symbols[row][col] == '0')
	{
		map->symbols[row][col] = '*';
		flood_fill(map, row, col - 1);
		flood_fill(map, row, col + 1);
		flood_fill(map, row - 1, col);
		flood_fill(map, row + 1, col);
	}
}

bool	is_boundary(const t_vector *map, const size_t row, const size_t col)
{
	size_t	row_count;
  size_t	len;

	row_count = map->capacity;
	len = ft_strlen(map->symbols[row]);
	if (row == 0 || row == row_count - 1 || col == 0 || col == len - 1)
		return (true);
	if (row == 0 || row >= row_count - 1)
	{
			printf("DEBUG: Row %zu is at the boundary, cannot access neighboring rows safely.\n", row);
			return (true);
	}
	const size_t prev_len = ft_strlen(map->symbols[row - 1]);
	const size_t next_len = ft_strlen(map->symbols[row + 1]);
		printf("DEBUG: Previous row length (trimmed): %zu, Next row length (trimmed): %zu, Current column: %zu\n", prev_len, next_len, col);
	if (col < prev_len - 1 && col < next_len - 1)
	{
		return (false);
	}
	return (true);
}

bool is_valid(t_vector *map, size_t row, size_t col)
{
	size_t row_count;
	size_t col_count;
	size_t row_idx;
	size_t col_idx;

	row_idx = 0;
	row_count = map->length;
	flood_fill(map, row, col);
	while (row_idx < row_count)
	{
		col_count = ft_strlen(map->symbols[row_idx]);
		if (col_count > 0 && map->symbols[row_idx][col_count - 1] == '\n')
		{
			col_count -= 1;
		}
		col_idx = 0;
		while (col_idx < col_count)
		{
			if (map->symbols[row_idx][col_idx] == '*')
			{
				const bool is_on_boundary = is_boundary(map, row_idx, col_idx);
				if (is_on_boundary)
				{
					log_error_message("Error: Flood fill reached a boundary. Map is not enclosed properly.");
					return (false);
				}
			}
			col_idx++;
		}
		row_idx++;
	}
	return (true);
}

void validate_map_with_flood_fill(t_vector *map)
{
	t_vector	*dup;
	int				row;
	int				col;

	dup = vector_copy(map);
	if (!dup->symbols)
	{
			ft_putstr_fd("Error\nMemory allocation failed for map duplication\n", 2);
			exit(1);
	}
	if (!find_symbol_position(dup, "NWSE", &row, &col))
	{
		ft_putstr_fd("Error\nInitial position not found in the map\n", 2);
		vector_free(dup);
		exit(1);
	}
	if (!is_valid(dup, row, col))
	{
		vector_free(dup);
		ft_putstr_fd("Error\nMap is not enclosed properly.\n", 2);
		exit(1);
	}
	printf("HERE IS DUP____\n");
	vector_print(dup);
	vector_free(dup);
}
