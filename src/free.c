/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:05:09 by istasheu          #+#    #+#             */
/*   Updated: 2024/11/14 14:48:15 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	free_assets_struct(t_assets *assets)
{
	if (assets->textures.path_EA)
		free(assets->textures.path_EA);
	if (assets->textures.path_NO)
		free(assets->textures.path_NO);
	if (assets->textures.path_SO)
		free(assets->textures.path_SO);
	if (assets->textures.path_WE)
		free(assets->textures.path_WE);
}

void	free_split_rgb_array(char **rgb_arr)
{
	int	i;

	i = 0;
	while (rgb_arr[i] != NULL)
	{
		free(rgb_arr[i]);
		i++;
	}
	free(rgb_arr);
}
