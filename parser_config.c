#include "../cub3d.h"

/**
 * Trims the identifier (NO, SO, EA, WE, F, C) from the config line and removes trailing spaces and newlines.
 */
static char	*trim_and_extract(const char *str, unsigned int prefix_len)
{
	unsigned int	end;

	while (str[prefix_len] == ' ' || str[prefix_len] == '\n')
		prefix_len++;
	end = ft_strlen(str);
	while (end > prefix_len && (str[end - 1] == ' ' || str[end - 1] == '\n'))
		end--;
	return (ft_substr(str, prefix_len, end - prefix_len));
}

/**
 * Extracts the texture path from the map, based on the identifier (e.g., "NO ", "SO ").
 */
static char	*extract_path(const char *id, t_vector *map, t_assets *assets)
{
	size_t	i;
	char		*path;
	char		*map_line;

	i = 0;
	while (i < map->length)
	{
		if (ft_strncmp(id, map->symbols[i], 3) == 0)
		{
			map_line = get_element_from_vector(map, i);
			printf("map_line:%s", map_line);
			if (!map_line)
				print_error_map("Failed to allocate map line", map, assets);
			path = trim_and_extract(map_line, 3);
			if (!path)
				print_error_map("Failed to allocate texture path", map, assets);
			return (path);
		}
		i++;
	}
	print_error_map("Missing texture information", map, assets);
	return (NULL);
}

/**
 * Loads texture paths from the map into the assets structure.
 */
void	load_asset_config(t_vector *map, t_assets *assets)
{
	assets->textures.path_NO = extract_path("NO ", map, assets);
	assets->textures.path_SO = extract_path("SO ", map, assets);
	assets->textures.path_EA = extract_path("EA ", map, assets);
	assets->textures.path_WE = extract_path("WE ", map, assets);
}

/**
 * Allocates and initializes the assets structure.
 */
t_assets	*initialize_assets(t_vector *map)
{
	t_assets	*assets;

	assets = (t_assets *)malloc(sizeof(t_assets));
	if (!assets)
			print_error_map("Failed to allocate assets", map, NULL);
	assets->textures.path_NO = NULL;
	assets->textures.path_SO = NULL;
	assets->textures.path_EA = NULL;
	assets->textures.path_WE = NULL;
	load_asset_config(map, assets);
	return (assets);
}