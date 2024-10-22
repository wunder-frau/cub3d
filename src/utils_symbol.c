/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_symbol.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:06:52 by istasheu          #+#    #+#             */
/*   Updated: 2024/10/22 10:06:54 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

float	assign_player_angle(char symbol)
{
	if (symbol == 'N' || symbol == 'n')
		return (0.0f);
	else if (symbol == 'E' || symbol == 'e')
		return (90.0f);
	else if (symbol == 'S' || symbol == 's')
		return (180.0f);
	else if (symbol == 'W' || symbol == 'w')
		return (270.0f);
	return (0.0f);
}

bool	is_player_symbol(char c)
{
	return (c == 'N' || c == 'n'
		|| c == 'S' || c == 's'
		|| c == 'E' || c == 'e'
		|| c == 'W' || c == 'w');
}

bool	is_valid_map_symbol(char c, t_vector *map)
{
	char	*valid_symbols;

	valid_symbols = "01NSEW";
	return (ft_strchr(valid_symbols, c) != NULL
		|| c == '\n' || c == '\0' || c == ' ');
}
