/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_realloc.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 01:30:00 by istasheu          #+#    #+#             */
/*   Updated: 2024/12/09 15:02:57 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "vector.h"

void	*ft_realloc(void *ptr, size_t current_size, size_t new_size)
{
	char		*old_ptr;
	char		*new_ptr;
	size_t		i;

	old_ptr = (char *)ptr;
	if (!new_size)
	{
		free(ptr);
		return (NULL);
	}
	if (!ptr)
		return (ft_calloc(sizeof(char), new_size));
	new_ptr = ft_calloc(sizeof(char), new_size);
	if (!new_ptr)
		return (NULL);
	i = 0;
	while (i < current_size)
	{
		new_ptr[i] = old_ptr[i];
		i++;
	}
	free(ptr);
	return (new_ptr);
}
