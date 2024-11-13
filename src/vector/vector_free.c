/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 01:32:14 by istasheu          #+#    #+#             */
/*   Updated: 2024/11/02 11:37:32 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

void	vector_free(t_vector *vector)
{
	size_t	i;

	i = 0;
	while (i < vector->capacity)
	{
		if (vector->symbols[i])
			free(vector->symbols[i]);
		i++;
	}
	free(vector->symbols);
	free(vector);
}
