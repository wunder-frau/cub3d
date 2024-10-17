#include "../cub3d.h"

static void	flood_fill_vector(t_vector *map, int row, int col)
{
	if (row < 0 || row >= map->length || col < 0 || col >= ft_strlen(map->symbols[row]))
		return ;
	if (is_player_symbol(map->symbols[row][col]) ||
								map->symbols[row][col] == ' ' ||
								map->symbols[row][col] == '0')
	{
		map->symbols[row][col] = '*';
		flood_fill_vector(map, row, col - 1);
		flood_fill_vector(map, row, col + 1);
		flood_fill_vector(map, row - 1, col);
		flood_fill_vector(map, row + 1, col);
	}
}

bool	is_symbol(t_vector *map, int *row_out, int *col_out)
{
	int	row;
	int	col;

	row = 0;
	while(row < map->length)
	{
		col = 0;
		while(col < ft_strlen(map->symbols[row]))
		{
			if (ft_strchr("NWSE", map->symbols[row][col]))
			{
				*row_out = row;
				*col_out = col;
				return (true);
			}
			col++;
		}
		row++;
	}
	return	(false);
}

void	validate_path(t_vector *map_ptr)
{
	t_vector	*dup;
	int	row, col, i, j;

	dup = vector_copy(map_ptr);
	if (!dup->symbols)
	{
		ft_putstr_fd("Error\nMemory allocation failed for map duplication\n", 2);
		exit(1);
	}
	if (!is_symbol(dup, &row, &col))
	{
		ft_putstr_fd("Error\nInitial position not found in the map\n", 2);
		vector_free(dup);
		exit(1);
	}
	flood_fill_vector(dup, row - 1, col); // `row` was incremented during search
	printf("HERE IS DUP____\n");
	vector_print(dup);
	i = 0;
	while( i < dup->length)
	{
		j = 0;
		while(j < ft_strlen(dup->symbols[i]))
		{
			printf("lololo_:%c\n",dup->symbols[i][j]);
			if (dup->symbols[i][j] != '1' && dup->symbols[i][j] != '*' && dup->symbols[i][j] != ' ' && dup->symbols[i][j] != '\n')
			{
				ft_putstr_fd("Error\nShould be only obstacles and spaces.\n", 2);
				vector_free(dup);
				exit(1);
			}
			j++;
		}
		i++;
	}
	vector_free(dup);
}

