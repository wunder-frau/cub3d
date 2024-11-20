/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_symbol.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpilo <nkarpilo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:06:52 by istasheu          #+#    #+#             */
/*   Updated: 2024/11/20 15:14:06 by nkarpilo         ###   ########.fr       */
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
	return (c == 'N' || c == 'n'
		|| c == 'S' || c == 's'
		|| c == 'E' || c == 'e'
		|| c == 'W' || c == 'w');
}

bool	is_valid_map_symbol(char c)
{
	char	*valid_symbols;

	valid_symbols = "01NSEWnsew";
	return (ft_strchr(valid_symbols, c) != NULL
		|| c == '\n' || c == '\0' || c == ' ');
}
