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
	if (line_len < 2)
	{
		log_error_message("Invalid map format - boundary line too small");
		return (false);
	}
	i = 0;
	// while (line[i] == ' ' || line[i] == '\t')
	// 	i++;
	while (line[i])
	{
		//printf("look___:[%c]\n", line[i]);
		//if (line[i] != '1' && line[i] != ' ')
		if (line[i] != '1' && line[i] != ' ')
		{
			printf("char___:%d at ind:[%d]\n", line[i], i);
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
	size_t first_line_len;
	size_t i;

	first_line_len = ft_strlen(map->symbols[0]);
	i = 1;
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

static bool	validate_side_walls(t_vector *map)
{
	size_t i;
	size_t line_len;

	i = 1;
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
bool	validate_map_presence(t_vector *map)
{
	if (!map || map->capacity == 0)
	{
		log_error_message("Map data is missing or empty.");
		return (false);
	}
	return (true);
}

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

bool	validate_map(t_vector *map, t_assets *assets)
{
	// validate_map_file_structure(map);
	if (!validate_map_form(map))
		return (false);
	// if (!validate_top_line(map))
	// 	return (false);
	// if (!validate_bottom_line(map))
	// 	return (false);
	if (count_players(map) != 1)
		error_exit_cleanup("Invalid player count in the map.", map, assets);
	if (!validate_texture_paths(assets))
		return (false);
	if (!validate_colors(assets))
		return (false);
	if (!validate_map_presence(map))
		return (false);
	validate_path(map);
	// if (!validate_side_walls(map))
	// 	return (false);
	// if (!validate_map_shape(map))
	// 	return (false);
	return (true);
}
       