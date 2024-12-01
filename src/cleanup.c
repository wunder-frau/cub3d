/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/14 14:49:37 by istasheu          #+#    #+#             */
/*   Updated: 2024/12/01 14:35:45 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static void	cleanup_textures(t_game *game)
{
	if (!game || !game->assets)
		return ;
	if (game->assets->textures.no)
	{
		mlx_delete_texture(game->assets->textures.no);
		game->assets->textures.no = NULL;
	}
	if (game->assets->textures.so)
	{
		mlx_delete_texture(game->assets->textures.so);
		game->assets->textures.so = NULL;
	}
	if (game->assets->textures.ea)
	{
		mlx_delete_texture(game->assets->textures.ea);
		game->assets->textures.ea = NULL;
	}
	if (game->assets->textures.we)
	{
		mlx_delete_texture(game->assets->textures.we);
		game->assets->textures.we = NULL;
	}
}

static void	cleanup_textures_and_images(t_game *game)
{
	if (!game)
		return ;
	cleanup_textures(game);
	if (game->mlx && game->image)
	{
		mlx_delete_image(game->mlx, game->image);
		game->image = NULL;
	}
}

static void	cleanup_map(t_game *game)
{
	size_t	y;

	if (!game || !game->map_grid)
		return ;
	if (game->map_grid->symbols)
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
	free(game->map_grid);
	game->map_grid = NULL;
}

static void	cleanup_game(t_game *game)
{
	if (!game)
		return ;
	if (game->mlx)
		mlx_terminate(game->mlx);
	free(game);
}

void	cleanup(t_game *game)
{
	if (!game)
		return ;
	cleanup_textures_and_images(game);
	cleanup_map(game);
	cleanup_game(game);
}
