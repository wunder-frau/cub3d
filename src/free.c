/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:05:09 by istasheu          #+#    #+#             */
/*   Updated: 2024/11/29 16:45:51 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void free_assets_struct(t_assets *assets)
{
    if (!assets)
        return ;
    if (assets->textures.path_ea)
    {
        free(assets->textures.path_ea);
        assets->textures.path_ea = NULL;
    }
    if (assets->textures.path_no)
    {
        free(assets->textures.path_no);
        assets->textures.path_no = NULL;
    }
    if (assets->textures.path_so)
    {
        free(assets->textures.path_so);
        assets->textures.path_so = NULL;
    }
    if (assets->textures.path_we)
    {
        free(assets->textures.path_we);
        assets->textures.path_we = NULL;
    }

    free(assets);
	assets = NULL;
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
