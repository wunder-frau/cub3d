/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config_order_validation.c                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:03:52 by istasheu          #+#    #+#             */
/*   Updated: 2024/12/09 12:32:10 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static bool	is_config_line(char *str)
{
	while (ft_is_space(*str))
		str++;
	if (*str == '\0' || *str == '\n')
		return (false);
	if ((*str == 'N' && *(str + 1) == 'O' && ft_is_space(*(str + 2)))
		|| (*str == 'S' && *(str + 1) == 'O' && ft_is_space(*(str + 2)))
		|| (*str == 'E' && *(str + 1) == 'A' && ft_is_space(*(str + 2)))
		|| (*str == 'W' && *(str + 1) == 'E' && ft_is_space(*(str + 2)))
		|| (*str == 'F' && ft_is_space(*(str + 1)))
		|| (*str == 'C' && ft_is_space(*(str + 1))))
	{
		return (true);
	}
	return (false);
}

static bool	is_map_line(const char *str)
{
	while (*str)
	{
		if (!is_valid_map_symbol(*str))
			return (false);
		str++;
	}
	return (true);
}

static void	check_no_config_after_map(t_vector *map, size_t i)
{
	while (i < map->capacity)
	{
		if (is_config_line(map->symbols[i]))
			error_exit_cleanup("Texture configuration found"
				" after map data", map, NULL);
		if (!is_blank_line(map->symbols[i])
			&& !is_map_line(map->symbols[i]))
			error_exit_cleanup("Invalid line found in the map data", map, NULL);
		i++;
	}
}

static int	count_config_lines(t_vector *map, size_t *i)
{
	int	texture_info_count;

	texture_info_count = 0;
	while (*i < map->capacity && is_blank_line(map->symbols[*i]))
		(*i)++;
	while (*i < map->capacity)
	{
		if (is_config_line(map->symbols[*i]))
		{
			texture_info_count++;
		}
		else if (!is_blank_line(map->symbols[*i]))
		{
			if (!is_map_line(map->symbols[*i]))
				error_exit_cleanup \
			("Invalid map: top line symbol or texture specifier", map, NULL);
			break ;
		}
		(*i)++;
	}
	return (texture_info_count);
}

void	validate_map_file_structure(t_vector *map)
{
	size_t	i;
	int		texture_info_count;

	i = 0;
	texture_info_count = count_config_lines(map, &i);
	if (texture_info_count != 6)
	{
		error_exit_cleanup("Invalid number of texture config lines", map, NULL);
	}
	check_no_config_after_map(map, i);
}
