/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   boundary_lines_validation.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:03:40 by istasheu          #+#    #+#             */
/*   Updated: 2024/10/22 10:38:37 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static bool	is_valid_boundary_line(const char *line)
{
	size_t	len;
	size_t	i;
	char	c;

	len = ft_strlen(line);
	if (len < 2 || line[len - 1] != '\n')
	{
		log_error_message("Invalid line format: "
			"line too small or missing '\\n'");
		return (false);
	}
	i = 0;
	while (i < len - 1)
	{
		c = line[i];
		if (c != '1' && c != ' ')
		{
			log_error_message("Invalid character found in the line of the map");
			return (false);
		}
		i++;
	}
	return (true);
}

static bool	starts_with_wall(const char *line)
{
	size_t	i;

	i = 0;
	while (line[i] == ' ')
		i++;
	return (line[i] == '1');
}

static bool	ends_with_wall(const char *line)
{
	size_t	i;

	i = ft_strlen(line) - 1;
	while (i > 0 && (line[i] == ' ' || line[i] == '\n'))
		i--;
	return (line[i] == '1');
}

static bool	validate_line(const char *line, const char *valid_chars,
	t_line_type line_type)
{
	if (!is_valid_line_len_and_nl(line, line_type))
		return (false);
	if (!is_valid_line_characters(line, valid_chars, line_type))
		return (false);
	if (line_type == LINE_MIDDLE)
	{
		if (!starts_with_wall(line))
		{
			log_error_message("Map middle line must start with wall '1'");
			return (false);
		}
		if (!ends_with_wall(line))
		{
			log_error_message("Map middle line must end with wall '1'");
			return (false);
		}
	}
	return (true);
}

bool	is_valid_boundaries(t_vector *map)
{
	size_t	i;

	if (!validate_map_presence(map) || map->capacity < 3)
	{
		log_error_message("Map too small or not initialized properly");
		return (false);
	}
	if (!validate_line(map->symbols[0], "1 ", LINE_TOP))
		return (false);
	i = 1;
	while (i < (map->capacity - 1))
	{
		if (!validate_line(map->symbols[i], "NESW10 ", LINE_MIDDLE))
			return (false);
		i++;
	}
	if (!validate_line(map->symbols[map->capacity - 1], "1 ", LINE_BOTTOM))
		return (false);
	return (true);
}
