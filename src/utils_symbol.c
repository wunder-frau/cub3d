/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_symbol.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:06:52 by istasheu          #+#    #+#             */
/*   Updated: 2024/11/23 15:57:14 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

float	assign_player_angle(char symbol)
{
	if (symbol == 'N')
		return ((3 * M_PI) / 2);
	else if (symbol == 'S')
		return (M_PI / 2);
	else if (symbol == 'E')
		return (0.0f);
	else if (symbol == 'W')
		return (M_PI);
	else
		return (0.0f);
}

bool	is_player_symbol(char c)
{
	return (c == 'N' || c == 'S'
		|| c == 'E' || c == 'W');
}

bool	is_valid_map_symbol(char c)
{
	char	*valid_symbols;

	valid_symbols = "01NSEW";
	return (ft_strchr(valid_symbols, c) != NULL
		|| c == '\n' || c == '\0' || c == ' ');
}

bool	check_file_height(t_vector *map)
{
	if (map->capacity >= MAX_FILE_NODES)
	{
		ft_putendl_fd("Error", 2);
		ft_putendl_fd("Config file too large", 2);
		return (false);
	}
	return (true);
}
