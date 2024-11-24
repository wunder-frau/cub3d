/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_print.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 01:34:21 by istasheu          #+#    #+#             */
/*   Updated: 2024/11/24 16:24:13 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

void	vector_print(t_vector *vector)
{
	size_t	i;

	if (!vector || !vector->symbols)
	{
		ft_printf("Error: Invalid vector or symbols.\n");
		return ;
	}
	i = 0;
	while (i < vector->capacity)
	{
		if (vector->symbols[i])
			printf("Line %zu: %s  line_len: %zu\n", i, vector->symbols[i],
				ft_strlen(vector->symbols[i]));
		else
			printf("Line %zu: (null)\n", i);
		i++;
	}
}
