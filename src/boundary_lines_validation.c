#include "../cub3d.h"

bool is_boundary(const t_vector *map, const size_t row, const size_t col, const size_t len, const size_t row_count) {
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
	const size_t row_count = map->length;
	flood_fill_vector(map, row, col);

	for (size_t row_idx = 0; row_idx < row_count; ++row_idx)
	{
		size_t col_count = ft_strlen(map->symbols[row_idx]);
		if (col_count > 0 && map->symbols[row_idx][col_count - 1] == '\n')
		{
			col_count -= 1;
		}
	 	printf("DEBUG: Row %zu content: [%s], length (excluding newline): %zu\n", row_idx, map->symbols[row_idx], col_count);
		for (size_t col_idx = 0; col_idx < col_count; ++col_idx)
		{
			char current = map->symbols[row_idx][col_idx];
			if (current != '*')
				continue;

			const bool is_on_boundary = is_boundary(map, row_idx, col_idx, col_count, row_count);
			if (is_on_boundary)
			{
				log_error_message("Error: Flood fill reached a boundary. Map is not enclosed properly.");
				return (false);
			}
		}
	}
	return (true);
}

// bool is_valid(t_vector *map, size_t row, size_t col)
// {
//     const size_t row_count = map->length;
//     flood_fill_vector(map, row, col);

//     for (size_t row_idx = 0; row_idx < row_count; ++row_idx)
//     {
//         // Calculate column count and trim newline if present
//         size_t col_count = ft_strlen(map->symbols[row_idx]);
//         if (col_count > 0 && map->symbols[row_idx][col_count - 1] == '\n')
//         {
//             col_count -= 1; // Exclude the newline character
//         }

//         // Debug information
//         printf("DEBUG: Row %zu content: [%s], length (excluding newline): %zu\n", row_idx, map->symbols[row_idx], col_count);

//         for (size_t col_idx = 0; col_idx < col_count; ++col_idx)
//         {
//             char current = map->symbols[row_idx][col_idx];
//             printf("DEBUG: Checking cell at row %zu, row_count %zu, col %zu, length of row [%zu]\n", row_idx, row_count, col_idx, col_count);

//             if (current != '*')
//                 continue;

//             const bool is_on_boundary = is_boundary(map, row_idx, col_idx, col_count, row_count);
//             if (is_on_boundary)
//             {
//                 printf("DEBUG: Flood fill reached the boundary at row %zu, col %zu. Map is not enclosed properly.\n", row_idx, col_idx);
//                 log_error_message("Error: Flood fill reached a boundary. Map is not enclosed properly.");
//                 return (false);
//             }
//         }
//     }

//     // Debug: Successfully validated the map
//     printf("DEBUG: Map successfully passed flood fill validation.\n");
//     return (true);
// }

static bool	validate_map_presence(t_vector *map)
{
	if (!map || map->capacity == 0)
	{
		log_error_message("Map data is missing or empty.");
		return (false);
	}
	return (true);
}

bool	is_valid_line_len_and_nl(const char *line, t_line_type line_type)
{
	size_t len = ft_strlen(line);

	// Check if line length is sufficient and ends with a newline character
	if (len < 2 || line[len - 1] != '\n')
	{
		if (line_type == LINE_TOP)
			log_error_message("Invalid top line: line too small or missing '\\n'");
		else if (line_type == LINE_BOTTOM)
			log_error_message("Invalid bottom line: line too small or missing '\\n'");
		else if (line_type == LINE_MIDDLE)
			log_error_message("Invalid middle line: line too small or missing '\\n'");
		return false;
	}

	return true;
}

bool is_valid_line_characters(const char *line, const char *valid_chars, t_line_type line_type)
{
	size_t len = ft_strlen(line);
	size_t i = 0;

	while (i < len - 1)
	{
		if (ft_strchr(valid_chars, line[i]) == NULL)
		{
			if (line_type == LINE_TOP)
				log_error_message("Invalid character found in the top line of the map");
			else if (line_type == LINE_BOTTOM)
				log_error_message("Invalid character found in the bottom line of the map");
			else if (line_type == LINE_MIDDLE)
				log_error_message("Invalid character found in the middle line of the map");
			return false;
		}
		i++;
	}
	return true;
}


static bool is_valid_boundary_line(const char *line)
{
	size_t len = ft_strlen(line);

	if (len < 2 || line[len - 1] != '\n')
	{
		log_error_message("Invalid line format: line too small or missing '\\n'");
		return false;
	}
	size_t i = 0;
	while (i < len - 1)
	{
		char c = line[i];
		if (c != '1' && c != ' ')
		{
			log_error_message("Invalid character found in the line of the map");
			return false;
		}
		i++;
	}
	printf("Debug: line validation successful.\n");
	return true;
}

bool	validate_line(const char *line, const char *valid_chars, t_line_type line_type)
{
	size_t	i;

	if (!is_valid_line_len_and_nl(line, line_type))
		return (false);
	if (!is_valid_line_characters(line, valid_chars, line_type))
		return (false);
	if (line_type == LINE_MIDDLE)
	{
		i = 0;
		while (line[i] == ' ')
			i++;
		if (line[i] != '1')
		{
			log_error_message("Map middle line must start with wall '1'");
			return (false);
		}
		while (line[i] != '\n')
			i++;
		if (i > 0 && line[i - 1] != '1')
		{
			log_error_message("Map middle line must end with wall '1'");
			return (false);
		}
	}
	return (true);
}

bool	is_valid_boundaries(t_vector *map)
{
	size_t	i;
	if (!validate_map_presence(map) || map->capacity < 3)
	{
		log_error_message("Map too small or not initialized properly");
		return (false);
	}
	if (!validate_line(map->symbols[0], "1 ", LINE_TOP))
		return (false);
	i = 1;
	while (i < (map->capacity - 1))
	{
		if (!validate_line(map->symbols[i], "NESW10 ", LINE_MIDDLE))
			return (false);
		i++;
	}
	if (!validate_line(map->symbols[map->capacity - 1], "1 ", LINE_BOTTOM))
		return (false);
	return (true);
}

// static bool validate_middle_line(char *line)
// {
//     size_t i = 0;
//     size_t line_len;

//     // Get the length of the line
//     line_len = ft_strlen(line);
//     printf("Debug: Middle line length is %zu\n", line_len);

//     // Check if the middle line is too small
//     if (line_len < 2)
//     {
//         printf("Debug: Middle line is too small with length %zu (must be >= 2)\n", line_len);
//         log_error_message("Invalid map format - middle line too small");
//         return (false);
//     }

//     // Check if the middle line ends with a newline character
//     if (line[line_len - 1] != '\n')
//     {
//         printf("Debug: Middle line does not end with a newline character.\n");
//         log_error_message("Invalid map format - unexpected newline");
//         return (false);
//     }

//     // Iterate through the characters of the middle line to validate
//     while (line[i] == ' ')
//     {
//         printf("Debug: Skipping space at index %zu\n", i);
//         i++;
//     }

//     // After skipping spaces, the first non-space character must be '1'
//     if (line[i] != '1')
//     {
//         printf("Debug: Invalid start character at index %zu: '%c'\n", i, line[i]);
//         log_error_message("Map must be surrounded by walls (1)");
//         return (false);
//     }

//     // Iterate through the line checking valid map symbols
//     while (line[i] != ' ' && line[i] != '\n')
//     {
//         printf("Debug: Checking character at index %zu: '%c'\n", i, line[i]);
//         if (ft_strchr("NESW10 ", line[i]) == NULL)
//         {
//             printf("Debug: Invalid character at index %zu: '%c'\n", i, line[i]);
//             log_error_message("Invalid character in map");
//             return (false);
//         }
//         i++;
//     }

//     if (i != 0 && line[i - 1] != '1')
//     {
//         printf("Debug: Middle line not properly enclosed at index %zu: '%c'\n", i, line[i]);
//         log_error_message("Map must be surrounded by walls (1)");
//         return (false);
//     }

//     // Debug: Middle line validation succeeded
//     printf("Debug: Middle line validation successful.\n");

//     return (true);
// }

// bool	is_valid_boundaries(t_vector *map)
// {
// 	size_t	i;

// 	if (!validate_map_presence(map))
// 		return (false);
// 	if (map->capacity < 3)
// 	{
// 		printf("Debug: Map capacity is %zu, which is too small (must be >= 3)\n", map->capacity);
// 		log_error_message("Map too small");
// 		return (false);
// 	}
// 	if (!is_valid_boundary_line(map->symbols[0]))
// 		return (false);
// 	if (!is_valid_boundary_line(map->symbols[map->capacity - 1]))
// 		return (false);
// 	i = 0;
// 	while (i < (map->capacity - 1))
// 	{
// 		if (!validate_middle_line(map->symbols[i]))
// 			return (false);
// 		i++;
// 	}
// 	return (true);
// }
