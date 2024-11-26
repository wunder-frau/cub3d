/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   allocate.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpilo <nkarpilo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:03:11 by istasheu          #+#    #+#             */
/*   Updated: 2024/11/25 15:01:31 by nkarpilo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static t_game	*allocate_game_structure(t_vector *map, t_player player,
		t_assets *assets)
{
	t_game	*game;

	game = malloc(sizeof(t_game));
	if (!game)
	{
		ft_putstr_fd("Failed to allocate memory for game structure\n", 2);
		return (NULL);
	}
	game->mlx = NULL;
	game->image = NULL;
	game->map_grid = map;
	game->assets = *assets;
	game->player.x = player.x;
	game->player.y = player.y;
	game->player.angle = player.angle;
	ft_memcpy(game->assets.colors.rgb_c, assets->colors.rgb_c,
		sizeof(assets->colors.rgb_c));
	ft_memcpy(game->assets.colors.rgb_f, assets->colors.rgb_f,
		sizeof(assets->colors.rgb_f));
	return (game);
}

static mlx_texture_t	*load_texture_from_path(const char *path)
{
	mlx_texture_t	*texture;

	texture = mlx_load_png(path);
	if (!texture)
	{
		ft_putstr_fd("Error\n", 2);
		ft_putstr_fd("Failed to load texture. Wrong path: ", 2);
		ft_putstr_fd((char *)path, 2);
		ft_putstr_fd("\n", 2);
	}
	return (texture);
}

int	load_textures_into_assets(t_vector *map, t_assets *assets)
{
	assets->textures.no = load_texture_from_path(assets->textures.path_no);
	assets->textures.so = load_texture_from_path(assets->textures.path_so);
	assets->textures.ea = load_texture_from_path(assets->textures.path_ea);
	assets->textures.we = load_texture_from_path(assets->textures.path_we);
	if (!assets->textures.no || !assets->textures.so
		|| !assets->textures.ea || !assets->textures.we)
	{
		ft_putstr_fd("Failed to load all required textures\n", 2);
		if (assets->textures.no)
			mlx_delete_texture(assets->textures.no);
		if (assets->textures.so)
			mlx_delete_texture(assets->textures.so);
		if (assets->textures.ea)
			mlx_delete_texture(assets->textures.ea);
		if (assets->textures.we)
			mlx_delete_texture(assets->textures.we);
		error_exit_cleanup("Map validation failed", map, assets);
		return (-1);
	}
	return (0);
}

static int	setup_mlx_and_image(t_game *game)
{
	game->mlx = mlx_init(WIDTH, HEIGHT, "CUB 3D", true);
	if (!game->mlx)
	{
		ft_putstr_fd("Failed to initialize MLX42\n", 2);
		return (-1);
	}
	game->image = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	if (!game->image)
	{
		ft_putstr_fd("Failed to create image\n", 2);
		mlx_terminate(game->mlx);
		return (-1);
	}
	if (mlx_image_to_window(game->mlx, game->image, 0, 0) < 0)
	{
		ft_putstr_fd("Failed to attach image to window\n", 2);
		mlx_delete_image(game->mlx, game->image);
		mlx_terminate(game->mlx);
		return (-1);
	}
	return (0);
}

t_game	*initialize_game(t_vector *map, t_player player, t_assets *assets)
{
	t_game	*game;

	game = allocate_game_structure(map, player, assets);
	if (!game)
		return (NULL);
	initialize_keys(&game->keys);
	if (load_textures_into_assets(game->map_grid, &game->assets) != 0)
	{
		cleanup(game);
		return (NULL);
	}
	if (setup_mlx_and_image(game) != 0)
	{
		cleanup(game);
		return (NULL);
	}
	return (game);
}
