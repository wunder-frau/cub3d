/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_create.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 01:31:16 by istasheu          #+#    #+#             */
/*   Updated: 2024/12/09 15:15:53 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

/**
 * `capacity` - total allocated size of the 'symbols' array
 * `length` - number of elements currently stored in 'symbols'
 */
t_vector	*vector_create(size_t initial_capacity)
{
	t_vector	*new_vector;

	new_vector = (t_vector *)ft_calloc(sizeof(t_vector), 1);
	if (new_vector == NULL)
		return (NULL);
	new_vector->symbols = (char **)ft_calloc(sizeof(char *), \
			initial_capacity + 1);
	if (new_vector->symbols == NULL)
	{
		free(new_vector);
		return (NULL);
	}
	new_vector->length = initial_capacity;
	new_vector->capacity = 0;
	new_vector->symbols[0] = NULL;
	return (new_vector);
}
