/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flood_fill.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:04:49 by istasheu          #+#    #+#             */
/*   Updated: 2024/10/22 10:42:44 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static bool	find_symbol_position(t_vector *map, const char *symbols,
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

static void	flood_fill(t_vector *map, int row, int col)
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

static bool	is_boundary(const t_vector *map, const size_t row, const size_t col)
{
	size_t	row_count;
	size_t	len;
	size_t	prev_len;
	size_t	next_len;

	row_count = map->capacity;
	len = ft_strlen(map->symbols[row]);
	if (row == 0 || row == row_count - 1 || col == 0 || col == len - 1)
		return (true);
	if (row == 0 || row >= row_count - 1)
		return (true);
	prev_len = ft_strlen(map->symbols[row - 1]);
	next_len = ft_strlen(map->symbols[row + 1]);
	if (col < prev_len - 1 && col < next_len - 1)
	{
		return (false);
	}
	return (true);
}

static bool	is_valid(t_vector *map, size_t row, size_t col)
{
	size_t	row_count;
	size_t	col_count;
	size_t	row_idx;
	size_t	col_idx;

	row_idx = 0;
	row_count = map->length;
	flood_fill(map, row, col);
	while (row_idx < row_count)
	{
		col_count = ft_strlen(map->symbols[row_idx]);
		if (col_count > 0 && map->symbols[row_idx][col_count - 1] == '\n')
			col_count -= 1;
		col_idx = 0;
		while (col_idx < col_count)
		{
			if (map->symbols[row_idx][col_idx] == '*'
				&& is_boundary(map, row_idx, col_idx))
				return (false);
			col_idx++;
		}
		row_idx++;
	}
	return (true);
}

void	validate_map_with_flood_fill(t_vector *map)
{
	t_vector	*dup;
	int			row;
	int			col;

	dup = vector_copy(map);
	if (!dup->symbols)
		error_exit_cleanup("Memory allocation failed for map dupl", dup, NULL);
	if (!find_symbol_position(dup, "NWSE", &row, &col))
		error_exit_cleanup("Initial position not found in the map", dup, NULL);
	if (!is_valid(dup, row, col))
		error_exit_cleanup("Flood fill reached a boundary."
			" Map is not enclosed properly.", dup, NULL);
	printf("HERE IS DUP____\n");
	vector_print(dup);
	vector_free(dup);
}
