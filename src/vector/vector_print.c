/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 01:34:21 by istasheu          #+#    #+#             */
/*   Updated: 2024/10/22 01:35:28 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

void	vector_print(t_vector *vector)
{
	size_t	i;
		// printf("helppppppp %zu: %s\n", i + 1, vector->symbols[i]);
		// printf("len: %zu\n", vector->length);
	i = 0;
	while (i < vector->capacity)
	{
		printf("Line %zu: %s  line_len: %zu    ", i, vector->symbols[i],
			ft_strlen(vector->symbols[i]));
		//printf("Total lines: %zu\n", vector->capacity + 1);
		//printf("len: %zu\n", vector->length);
		//printf("line_len: %zu    ", ft_strlen(vector->symbols[i]));
		i++;
	}
}
