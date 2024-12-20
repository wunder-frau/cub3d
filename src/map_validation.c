/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:05:34 by istasheu          #+#    #+#             */
/*   Updated: 2024/11/25 10:25:31 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static bool	validate_texture_paths(t_assets *assets)
{
	if (!assets->textures.path_no)
		log_error_message("Missing texture path for North (NO).");
	if (!assets->textures.path_so)
		log_error_message("Missing texture path for South (SO).");
	if (!assets->textures.path_ea)
		log_error_message("Missing texture path for East (EA).");
	if (!assets->textures.path_we)
		log_error_message("Missing texture path for West (WE).");
	if (!assets->textures.path_no || !assets->textures.path_so
		|| !assets->textures.path_ea || !assets->textures.path_we)
		return (false);
	return (true);
}

static bool	validate_colors(t_assets *assets)
{
	int	i;

	i = 0;
	while (i < 3)
	{
		if (assets->colors.rgb_c[i] < 0 || assets->colors.rgb_c[i] > 255)
		{
			log_error_message("Invalid ceiling color component");
			ft_printf("at index %d: %d\n", i, assets->colors.rgb_c[i]);
			return (false);
		}
		if (assets->colors.rgb_f[i] < 0 || assets->colors.rgb_f[i] > 255)
		{
			log_error_message("Invalid floor color component ");
			ft_printf("at index %d: %d\n", i, assets->colors.rgb_f[i]);
			return (false);
		}
		i++;
	}
	return (true);
}

bool	is_cub(const char *str)
{
	size_t	len;

	len = ft_strlen(str);
	if (ft_strncmp(".cub", str + len - 4, 4) == 0)
		return (true);
	return (false);
}

void	space_to_wall(t_vector *map)
{
	size_t	row;
	size_t	col;

	if (map == NULL || map->symbols == NULL)
	{
		ft_putstr_fd("Error\nMap or map symbols are NULL.\n", 2);
		return ;
	}
	row = 0;
	while (row < map->capacity)
	{
		if (map->symbols[row] == NULL)
		{
			row++;
			continue ;
		}
		col = 0;
		while (col < ft_strlen(map->symbols[row]))
		{
			if (map->symbols[row][col] == ' ')
				map->symbols[row][col] = '1';
			col++;
		}
		row++;
	}
}

bool	validate_map(t_vector *map, t_assets *assets)
{
	if (!is_valid_boundaries(map))
		error_exit_cleanup("Invalid map boundaries.", map, assets);
	count_players(map);
	validate_map_with_flood_fill(map);
	space_to_wall(map);
	if (!validate_texture_paths(assets))
		return (false);
	if (!validate_colors(assets))
		return (false);
	return (true);
}
