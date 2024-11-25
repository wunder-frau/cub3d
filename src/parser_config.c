/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_config.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:05:54 by istasheu          #+#    #+#             */
/*   Updated: 2024/11/25 10:26:00 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

/**
 * Extracts the texture path from the map,
 * based on the identifier (e.g., "NO ", "SO ").
 */
static char	*extract_texture_path(const char *id, t_vector *map,
		t_assets *assets)
{
	size_t	i;
	char	*path;
	char	*map_line;

	i = 0;
	while (i < map->length)
	{
		if (ft_strncmp(id, map->symbols[i], 3) == 0)
		{
			map_line = vector_get_at(map, i);;
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

/**
 * Loads texture paths from the map into the assets structure.
 */
static void	load_config_path_and_rgb(t_vector *map, t_assets *assets)
{
	assets->textures.path_no = extract_texture_path("NO ", map, assets);
	assets->textures.path_so = extract_texture_path("SO ", map, assets);
	assets->textures.path_ea = extract_texture_path("EA ", map, assets);
	assets->textures.path_we = extract_texture_path("WE ", map, assets);
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
	assets->textures.path_no = NULL;
	assets->textures.path_so = NULL;
	assets->textures.path_ea = NULL;
	assets->textures.path_we = NULL;
	assets->textures.no = NULL;
	assets->textures.so = NULL;
	assets->textures.ea = NULL;
	assets->textures.we = NULL;
	if (remove_empty_line(map) == -1)
		error_exit_cleanup("Failed to remove empty lines", map, assets);
	load_config_path_and_rgb(map, assets);
	if (!validate_map(map, assets))
		error_exit_cleanup("Map validation failed", map, assets);
	return (assets);
}
