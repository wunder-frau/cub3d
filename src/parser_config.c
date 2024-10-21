#include "../cub3d.h"

/**
 * Trims the identifier (NO, SO, EA, WE, F, C)
 * from the config line and removes trailing spaces and newlines.
 */
static char	*trim_and_extract(char *str, unsigned int prefix_len)
{
	unsigned int	end;
	char			*trimmed;

	while (str[prefix_len] == ' ' || str[prefix_len] == '\n')
		prefix_len++;
	end = ft_strlen(str);
	while (end > prefix_len && (str[end - 1] == ' ' || str[end - 1] == '\n'))
		end--;
	trimmed = ft_substr(str, prefix_len, end - prefix_len);
	free(str);
	str = NULL;
	return (trimmed);
}

/**
 * Extracts the texture path from the map,
 * based on the identifier (e.g., "NO ", "SO ").
 */
static char	*extract_path(const char *id, t_vector *map, t_assets *assets)
{
	size_t	i;
	char	*path;
	char	*map_line;

	i = 0;
	while (i < map->length)
	{
		if (ft_strncmp(id, map->symbols[i], 3) == 0)
		{
			map_line = vector_get_at(map, i);
			printf("map_line:%s", map_line);
			if (!map_line)
				error_exit_cleanup("Failed to allocate map line", map, assets);
			path = trim_and_extract(map_line, 3);
			if (!path)
				error_exit_cleanup("Failed to allocate "
					"texture path", map, assets);
			return (path);
		}
		i++;
	}
	error_exit_cleanup("Missing texture information", map, assets);
	return (NULL);
}

static void	free_rgb_array(char **rgb_arr)
{
	int	i;

	i = 0;
	while (rgb_arr[i] != NULL)
	{
		free(rgb_arr[i]);
		i++;
	}
	free(rgb_arr);
}

static bool	store_rgb(char *rgb_trimmed, t_assets *assets, const char *id)
{
	char	**rgb_arr;

	rgb_arr = ft_split(rgb_trimmed, ',');
	free(rgb_trimmed);
	if (!rgb_arr || !rgb_arr[0] || !rgb_arr[1] || !rgb_arr[2])
		return (false);
	if (id[0] == 'F')
	{
		assets->colors.rgb_F[0] = ft_atoi(rgb_arr[0]);
		assets->colors.rgb_F[1] = ft_atoi(rgb_arr[1]);
		assets->colors.rgb_F[2] = ft_atoi(rgb_arr[2]);
	}
	else
	{
		assets->colors.rgb_C[0] = ft_atoi(rgb_arr[0]);
		assets->colors.rgb_C[1] = ft_atoi(rgb_arr[1]);
		assets->colors.rgb_C[2] = ft_atoi(rgb_arr[2]);
	}
	free_rgb_array(rgb_arr);
	return (true);
}

static char	*get_rgb(const char *id, t_vector *map, t_assets *assets)
{
	size_t	i;
	char	*rgb;
	char	*rgb_trimmed;

	i = 0;
	while (i < map->length)
	{
		if (ft_strncmp(id, map->symbols[i], 2) == 0)
		{
			rgb = vector_get_at(map, i);
			if (!rgb)
				error_exit_cleanup("Failed to retrieve RGB line", map, assets);
			rgb_trimmed = trim_and_extract(rgb, 2);
			if (!rgb_trimmed)
				error_exit_cleanup("Failed to extract RGB line", map, assets);
			if (!store_rgb(rgb_trimmed, assets, id))
				error_exit_cleanup("Failed to copy RGB values", map, assets);
			return (rgb_trimmed);
		}
		i++;
	}
	error_exit_cleanup("Missing RGB information", map, assets);
	return (NULL);
}

/**
 * Loads texture paths from the map into the assets structure.
 */
static void	load_asset_config(t_vector *map, t_assets *assets)
{
	assets->textures.path_NO = extract_path("NO ", map, assets);
	assets->textures.path_SO = extract_path("SO ", map, assets);
	assets->textures.path_EA = extract_path("EA ", map, assets);
	assets->textures.path_WE = extract_path("WE ", map, assets);
}

static void	load_asset_rgb(t_vector *map, t_assets *assets)
{
	get_rgb("C ", map, assets);
	get_rgb("F ", map, assets);
}

/**
 * Allocates and initializes the assets structure.
 */
t_assets	*initialize_assets(t_vector *map)
{
	t_assets	*assets;

	assets = (t_assets *)malloc(sizeof(t_assets));
	if (!assets)
		error_exit_cleanup("Failed to allocate assets", map, NULL);
	assets->textures.path_NO = NULL;
	assets->textures.path_SO = NULL;
	assets->textures.path_EA = NULL;
	assets->textures.path_WE = NULL;
	assets->textures.NO = NULL;
	assets->textures.SO = NULL;
	assets->textures.EA = NULL;
	assets->textures.WE = NULL;
	if (remove_empty_line(map) == -1)
		error_exit_cleanup("Failed to remove empty lines", map, assets);
	load_asset_config(map, assets);
	load_asset_rgb(map, assets);
	if (!validate_map(map, assets))
		error_exit_cleanup("Map validation failed", map, assets);
	return (assets);
}
