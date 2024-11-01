/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpilo <nkarpilo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:05:09 by istasheu          #+#    #+#             */
/*   Updated: 2024/11/01 16:13:29 by nkarpilo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static void	cleanup_textures_and_images(t_game *game)
{
	if (!game)
		return ;
	if (game->assets.textures.NO)
		mlx_delete_texture(game->assets.textures.NO);
	if (game->assets.textures.SO)
		mlx_delete_texture(game->assets.textures.SO);
	if (game->assets.textures.EA)
		mlx_delete_texture(game->assets.textures.EA);
	if (game->assets.textures.WE)
		mlx_delete_texture(game->assets.textures.WE);
	if (game->image)
		mlx_delete_image(game->mlx, game->image);
}

static void	cleanup_map_and_game(t_game *game)
{
	int	y;

	if (!game)
		return ;
	if (game->map_grid && game->map_grid->symbols)
	{
		y = 0;
		while (y < game->map_grid->capacity)
		{
			if (game->map_grid->symbols[y])
			{
				free(game->map_grid->symbols[y]);
				game->map_grid->symbols[y] = NULL;
			}
			y++;
		}
		free(game->map_grid->symbols);
		game->map_grid->symbols = NULL;
	}
	if (game->map_grid)
		free(game->map_grid);
	if (game->mlx)
		mlx_terminate(game->mlx);
	free(game);
}

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

void	cleanup(t_game *game)
{
	cleanup_textures_and_images(game);
	cleanup_map_and_game(game);
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
