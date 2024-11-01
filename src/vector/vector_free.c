/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 01:32:14 by istasheu          #+#    #+#             */
/*   Updated: 2024/11/01 17:43:14 by istasheu         ###   ########.fr       */
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

// void vector_free(t_vector *vector)
// {
//     size_t i;

//     if (!vector)
//         return;
    
//     for (i = 0; i < vector->capacity; i++)
//     {
//         if (vector->symbols[i])
//         {
//             free(vector->symbols[i]);
//             vector->symbols[i] = NULL; // Set to NULL to prevent double-free
//         }
//     }
//     free(vector->symbols);
//     vector->symbols = NULL;  // Ensure symbols pointer is NULL to prevent future frees
//     free(vector);
// }