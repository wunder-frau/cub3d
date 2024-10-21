/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vector_push_back.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 01:36:04 by istasheu          #+#    #+#             */
/*   Updated: 2024/10/22 01:36:58 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

int	vector_push_back(t_vector *current, char *new_symbol)
{
	char	**temp_symbols;
	size_t	new_size;

	if (current->capacity == current->length)
	{
		if (current->length == 0)
			new_size = 1;
		else
			new_size = current->length * 2;
		temp_symbols = (char **)ft_realloc(current->symbols,
				current->length * sizeof(char *),
				new_size * sizeof(char *));
		if (temp_symbols == NULL)
			return (1);
		current->symbols = temp_symbols;
		current->length = new_size;
	}
	current->symbols[current->capacity] = new_symbol;
	current->capacity++;
	if (current->capacity < current->length)
		current->symbols[current->capacity] = NULL;
	return (0);
}
