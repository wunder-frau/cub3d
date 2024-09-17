#include "../cub3d.h"

void	free_assets_struct(t_assets *assets)
{
	if (assets->textures.path_EA != NULL)
		free(assets->textures.path_EA);
	if (assets->textures.path_NO != NULL)
		free(assets->textures.path_NO);
	if (assets->textures.path_SO != NULL)
		free(assets->textures.path_SO);
	if (assets->textures.path_WE != NULL)
		free(assets->textures.path_WE);
}

void	map_validation_error(char *msg, t_vector *map, t_assets *assets)
{
	if (assets != NULL)
		free_assets_struct(assets);
	if (map != NULL)
		free_vector(map);
	if (msg != NULL)
	{
		ft_putendl_fd("Error", 2);
		ft_putendl_fd(msg, 2);
	}
	exit(1);
}

void	msg_and_exit(char *msg)
{
	ft_putendl_fd("Error", 2);
	ft_putendl_fd(msg, 2);
	exit(1);
}

bool	error_msg(char *msg)
{
	ft_putendl_fd("Error", 2);
	ft_putendl_fd(msg, 2);
	return (false);
}

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
static char	*extract_texture_path(const char *id, t_vector *map, t_assets *assets)
{
	size_t	i;
	char		*texture_path;

	i = 0;
	while (i < map->length)
	{
		if (ft_strncmp(id, map->symbols[i], 3) == 0)
		{
			char *texture_line = extract_array(map, i);
			if (!texture_line)
				map_validation_error("Failed to allocate texture line", map, assets);
			texture_path = trim_and_extract(texture_line, 3);
			if (!texture_path)
				map_validation_error("Failed to allocate texture path", map, assets);
			return (texture_path);
		}
		i++;
	}
	map_validation_error("Missing texture information", map, assets);
	return (NULL);
}

/**
 * Loads texture paths and RGB color values (floor and ceiling) from the map into the assets structure.
 */
void	load_asset_config(t_vector *map, t_assets *assets)
{
	assets->textures.path_NO = extract_texture_path("NO ", map, assets);
	assets->textures.path_SO = extract_texture_path("SO ", map, assets);
	assets->textures.path_EA = extract_texture_path("EA ", map, assets);
	assets->textures.path_WE = extract_texture_path("WE ", map, assets);
}

/**
 * Allocates and initializes the assets structure.
 */
t_assets	*initialize_assets(t_vector *map)
{
	t_assets	*assets;

	assets = (t_assets *)malloc(sizeof(t_assets));
	if (!assets)
			map_validation_error("Failed to allocate assets", map, NULL);
	assets->textures.path_NO = NULL;
	assets->textures.path_SO = NULL;
	assets->textures.path_EA = NULL;
	assets->textures.path_WE = NULL;

	load_asset_config(map, assets);
	return (assets);
}