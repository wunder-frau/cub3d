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

static bool	copy_rgb_values(char *rgb_trimmed, t_assets *assets, const char *id)
{
	char **rgb_arr;
	int r, g, b;

	rgb_arr = ft_split(rgb_trimmed, ',');
	free(rgb_trimmed);
	if (!rgb_arr || !rgb_arr[0] || !rgb_arr[1] || !rgb_arr[2])
			return false;
	r = ft_atoi(rgb_arr[0]);
	g = ft_atoi(rgb_arr[1]);
	b = ft_atoi(rgb_arr[2]);
	if (id[0] == 'F')
	{
		assets->colors.rgb_F[0] = r;
		assets->colors.rgb_F[1] = g;
		assets->colors.rgb_F[2] = b;
	}
	else
	{
		assets->colors.rgb_C[0] = r;
		assets->colors.rgb_C[1] = g;
		assets->colors.rgb_C[2] = b;
	}
	free(rgb_arr);
	return true;
}

static char *get_rgb(const char *id, t_vector *map, t_assets *assets)
{
	size_t i;
	char *rgb;
	char *rgb_trimmed;

	i = 0;
	while (i < map->length)
	{
		if (ft_strncmp(id, map->symbols[i], 2) == 0)
		{
			rgb = get_element_from_vector(map, i);
			if (!rgb)
				print_error_map("Failed to retrieve RGB line", map, assets);
			
			rgb_trimmed = trim_and_extract(rgb, 2);
			if (!rgb_trimmed)
				print_error_map("Failed to extract RGB line", map, assets);

			if (!copy_rgb_values(rgb_trimmed, assets, id))
				print_error_map("Failed to copy RGB values", map, assets);
			return rgb_trimmed;
		}
		i++;
	}
	print_error_map("Missing RGB information", map, assets);
	return NULL;
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
			print_error_map("Failed to allocate assets", map, NULL);
	assets->textures.path_NO = NULL;
	assets->textures.path_SO = NULL;
	assets->textures.path_EA = NULL;
	assets->textures.path_WE = NULL;
	load_asset_config(map, assets);
	load_asset_rgb(map, assets);
	return (assets);
}