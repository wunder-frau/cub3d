/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_validation.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:05:34 by istasheu          #+#    #+#             */
/*   Updated: 2024/10/22 10:05:36 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	if (!assets->textures.path_NO || !assets->textures.path_SO
		|| !assets->textures.path_EA || !assets->textures.path_WE)
		return (false);
	return (true);
}

static bool	validate_colors(t_assets *assets)
{
	int	i;

	i = 0;
	while (i < 3)
	{
		if (assets->colors.rgb_C[i] < 0 || assets->colors.rgb_C[i] > 255)
		{
			log_error_message("Invalid ceiling color component");
			ft_printf("at index %d: %d\n", i, assets->colors.rgb_C[i]);
			return (false);
		}
		if (assets->colors.rgb_F[i] < 0 || assets->colors.rgb_F[i] > 255)
		{
			log_error_message("Invalid floor color component ");
			ft_printf("at index %d: %d\n", i, assets->colors.rgb_F[i]);
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
	int	row;
	int	col;

	if (map == NULL || map->symbols == NULL)
	{
		ft_putstr_fd("Error\nMap or map symbols are NULL.\n", 2);
		return ;
	}
	row = 0;
	while (row < map->length)
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
	printf("DEBUG: Map successfully passed flood fill validation.\n");
	space_to_wall(map);
	if (!validate_texture_paths(assets))
		return (false);
	if (!validate_colors(assets))
		return (false);
	return (true);
}
