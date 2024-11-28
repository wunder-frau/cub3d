/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_config_rgb.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 10:07:16 by istasheu          #+#    #+#             */
/*   Updated: 2024/11/28 18:15:01 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static bool is_all_digits(const char *str)
{
	while (*str)
	{
		if (!ft_isdigit(*str) && !ft_is_space(*str))
			return false;
		str++;
	}
	return true;
}

static char	**parse_and_validate_rgb(char *rgb_trimmed)
{
	char	**rgb_arr;

	if (!validate_comma_count(rgb_trimmed, 2))
	{
		free(rgb_trimmed);
		return (NULL);
	}
	rgb_arr = ft_split(rgb_trimmed, ',');
	free(rgb_trimmed);
	if (!rgb_arr || !rgb_arr[0] || !rgb_arr[1] || !rgb_arr[2] ||
	!is_all_digits(rgb_arr[0]) || !is_all_digits(rgb_arr[1]) || !is_all_digits(rgb_arr[2]))
	{
		if (rgb_arr)
			free_split_rgb_array(rgb_arr);
		return (NULL);
	}
	return (rgb_arr);
}

static bool	convert_and_store_rgb(char **rgb_arr, t_assets *assets,
				const char *id)
{
	int	r;
	int	g;
	int	b;

	r = ft_atoi(rgb_arr[0]);
	g = ft_atoi(rgb_arr[1]);
	b = ft_atoi(rgb_arr[2]);
	if (id[0] == 'F')
	{
		assets->colors.rgb_f[0] = r;
		assets->colors.rgb_f[1] = g;
		assets->colors.rgb_f[2] = b;
	}
	else
	{
		assets->colors.rgb_c[0] = r;
		assets->colors.rgb_c[1] = g;
		assets->colors.rgb_c[2] = b;
	}
	free_split_rgb_array(rgb_arr);
	return (true);
}

static bool	store_rgb(char *rgb_trimmed, t_assets *assets, const char *id)
{
	char	**rgb_arr;

	rgb_arr = parse_and_validate_rgb(rgb_trimmed);
	if (!rgb_arr)
		return (false);
	if (!convert_and_store_rgb(rgb_arr, assets, id))
		return (false);
	return (true);
}

char	*get_rgb(const char *id, t_vector *map, t_assets *assets)
{
	size_t	i;
	char	*rgb;
	char	*rgb_trimmed;

	i = 0;
	while (i < map->length)
	{
		if (ft_strncmp(id, map->symbols[i], 2) == 0)
		{
			rgb = vector_get_at(map, i);
			if (!rgb)
				error_exit_cleanup("Failed to retrieve RGB line", map, assets);
			rgb_trimmed = trim_and_extract(rgb, 2);
			if (!rgb_trimmed)
				error_exit_cleanup("Failed to extract RGB line", map, assets);
			if (!store_rgb(rgb_trimmed, assets, id))
				error_exit_cleanup("Failed to copy RGB values", map, assets);
			return (rgb_trimmed);
		}
		i++;
	}
	error_exit_cleanup("Missing RGB information", map, assets);
	return (NULL);
}
