#include "../cub3d.h"

static bool	validate_boundary_line(const char *line)
{
	size_t i;
	size_t line_len;

	if (line == NULL)
	{
		log_error_message("Invalid map format - boundary line is NULL");
		return (false);
	}
	line_len = ft_strlen(line);
	// if (line_len < 2 || line[line_len - 1] != '\n')
	if (line_len < 2)
	{
		log_error_message("Invalid map format - boundary line too small");
		return (false);
	}
	if (line[0] != '1' || line[line_len - 2] != '1')
	{
		log_error_message("Invalid map format - boundary line not enclosed by walls");
		return (false);
	}
	i = 0;
	while (line[i] != '\n')
	{
		if (line[i] != '1' && line[i] != ' ')
		{
			log_error_message("Invalid map format - boundary line must only contain '1' and spaces");
			return (false);
		}
		i++;
	}
	return (true);
}

static bool	validate_top_line(t_vector *map)
{
	if (map->capacity < 3)
	{
		log_error_message("Map too small");
		return (false);
	}
	return (validate_boundary_line(map->symbols[0]));
}

static bool	validate_bottom_line(t_vector *map)
{
	return (validate_boundary_line(map->symbols[map->capacity - 1]));
}

static bool	validate_map_shape(t_vector *map)
{
	size_t first_line_len = ft_strlen(map->symbols[0]);
	size_t i = 1;

	while (i < map->capacity)
	{
		if (ft_strlen(map->symbols[i]) != first_line_len)
		{
			log_error_message("Invalid map format - map lines are not of equal length");
			return (false);
		}
		i++;
	}
	return (true);
}

static bool validate_side_walls(t_vector *map)
{
	size_t i = 1;
	size_t line_len;

	while (i < map->capacity - 1)
	{
		line_len = ft_strlen(map->symbols[i]);
		if (map->symbols[i][0] != '1' || map->symbols[i][line_len - 2] != '1')
		{
			ft_putstr_fd("Invalid map format - map not enclosed by walls on line", 2);
			ft_printf(" %d\n", i + 1);
			return (false);
		}
		i++;
	}
	return (true);
}
bool validate_map_presence(t_vector *map)
{
	if (!map || map->capacity == 0)
	{
		log_error_message("Map data is missing or empty.");
		return false;
	}
	return (true);
}

static bool validate_texture_paths(t_assets *assets)
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
        return false;

    return true;
}

static bool validate_colors(t_assets *assets)
{
    for (int i = 0; i < 3; i++)
    {
        if (assets->colors.rgb_C[i] < 0 || assets->colors.rgb_C[i] > 255)
        {
            ft_printf("Invalid ceiling color component at index %d: %d\n", i, assets->colors.rgb_C[i]);
            return false;
        }
        if (assets->colors.rgb_F[i] < 0 || assets->colors.rgb_F[i] > 255)
        {
            ft_printf("Invalid floor color component at index %d: %d\n", i, assets->colors.rgb_F[i]);
            return false;
        }
    }
    return true;
}

// bool	validate_map(t_vector *map)
// {
// 	if (!validate_top_line(map))
// 		return (false);
// 	if (!validate_bottom_line(map))
// 		return (false);
// 	if (!validate_side_walls(map))
// 		return (false);
// 	if (!validate_map_shape(map))
// 		return (false);
// 	if (!validate_map_presence(map))
// 		return false;
// 	return (true);
// }

bool validate_map(t_vector *map, t_assets *assets)
{
    // // First, validate textures and colors
    if (!validate_texture_paths(assets))
        return false;

    if (!validate_colors(assets))
        return false;

    // Check for map presence
    if (!validate_map_presence(map))
        return false;

    // Proceed with existing map validations
    if (!validate_top_line(map))
        return false;
    if (!validate_bottom_line(map))
        return false;
    if (!validate_side_walls(map))
        return false;
    if (!validate_map_shape(map))
        return false;

    return true;
}
