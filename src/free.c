/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:05:09 by istasheu          #+#    #+#             */
/*   Updated: 2024/11/24 19:45:37 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	free_assets_struct(t_assets *assets)
{
	if (assets->textures.path_ea)
		free(assets->textures.path_ea);
	if (assets->textures.path_no)
		free(assets->textures.path_no);
	if (assets->textures.path_so)
		free(assets->textures.path_so);
	if (assets->textures.path_we)
		free(assets->textures.path_we);
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
