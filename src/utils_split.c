/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_split.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 11:17:09 by istasheu          #+#    #+#             */
/*   Updated: 2024/11/20 11:18:36 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

/**
 * Validates the number of commas in the input string.
 * Returns true if the number of commas matches the
 * expected count, false otherwise.
 */
bool	validate_comma_count(const char *str, size_t expected_count)
{
	size_t	comma_count;
	size_t	i;

	comma_count = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == ',')
			comma_count++;
		i++;
	}
	return (comma_count == expected_count);
}
