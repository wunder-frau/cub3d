#include "../cub3d.h"

static bool	validate_texture_paths(t_assets *assets)
{
	if (!assets->textures.path_NO)
		log_error_message("Missing texture path for North (NO).");
	if (!assets->textures.path_SO)
		log_error_message("Missing texture path for South (SO).");
	if (!assets->textures.path_EA)
		log_error_message("Missing texture path for East (EA).");
	if (!assets->textures.path_WE)
		log_error_message("Missing texture path for West (WE).");
	if (!assets->textures.path_NO || !assets->textures.path_SO ||
		!assets->textures.path_EA || !assets->textures.path_WE)
		return (false);
	return (true);
}

static bool	validate_colors(t_assets *assets)
{
	int i;

	i = 0;
	while (i < 3)
	{
		if (assets->colors.rgb_C[i] < 0 || assets->colors.rgb_C[i] > 255)
		{
			ft_printf("Invalid ceiling color component at index %d: %d\n", i, assets->colors.rgb_C[i]);
			return (false);
		}
		if (assets->colors.rgb_F[i] < 0 || assets->colors.rgb_F[i] > 255)
		{
			ft_printf("Invalid floor color component at index %d: %d\n", i, assets->colors.rgb_F[i]);
			return (false);
		}
		i++;
	}
	return true;
}

bool	is_cub(const char *str)
{
	size_t	len;

	len = ft_strlen(str);
	if (ft_strncmp(".cub", str + len - 4, 4) == 0)
		return (true);
	return (false);
}

void trim_leading_spaces(t_vector *map)
{
    size_t i = 0;

    // Iterate over each line of the map and remove leading spaces
    while (i < map->capacity)
    {
        char *line = map->symbols[i];
        while (*line == ' ' || *line == '\t') // Skip leading spaces and tabs
            line++;

        // Copy back the trimmed line to map->text[i]
        map->symbols[i] = ft_strdup(line);
        if (map->symbols[i] == NULL)
        {
            log_error_message("Memory allocation failed");
            exit(1);
        }

        i++;
    }
}

void	space_to_wall(t_vector *map)
{
    int	row;
    int	col;

    if (map == NULL || map->symbols == NULL)
    {
        ft_putstr_fd("Error\nMap or map symbols are NULL.\n", 2);
        return;  // Avoid segmentation fault by early exit
    }

    row = 0;
    while (row < map->length)
    {
        if (map->symbols[row] == NULL)
        {
            row++;
            continue;  // Safeguard to skip any NULL rows
        }

        col = 0;
        while (col < ft_strlen(map->symbols[row]))
        {
            // Directly compare to the space character instead of using ft_strchr
            if (map->symbols[row][col] == ' ')
            {
                printf("DEBUG: SPACE found at row %d, col %d\n", row, col);
                map->symbols[row][col] = '1';
            }
            col++;
        }
        row++;
    }
}


bool	validate_map(t_vector *map, t_assets *assets)
{
	size_t	i;

	i = 0;
	if (!is_valid_boundaries(map))
		error_exit_cleanup("Invalid map boundaries.", map, assets);
	if (count_players(map) != 1)
		error_exit_cleanup("Invalid player count in the map.", map, assets);
	validate_map_with_flood_fill(map);
		printf("DEBUG: Map successfully passed flood fill validation.\n");
	if (!validate_texture_paths(assets))
		return (false);
	if (!validate_colors(assets))
		return (false);
	return (true);
}
