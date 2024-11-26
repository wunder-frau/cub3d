/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpilo <nkarpilo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 14:37:38 by nkarpilo          #+#    #+#             */
/*   Updated: 2024/11/25 14:56:20 by nkarpilo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

void	handle_key_press(mlx_key_data_t keydata, t_game *game)
{
	if (keydata.key == MLX_KEY_W)
		game->keys.w = true;
	else if (keydata.key == MLX_KEY_A)
		game->keys.a = true;
	else if (keydata.key == MLX_KEY_S)
		game->keys.s = true;
	else if (keydata.key == MLX_KEY_D)
		game->keys.d = true;
	else if (keydata.key == MLX_KEY_LEFT)
		game->keys.left = true;
	else if (keydata.key == MLX_KEY_RIGHT)
		game->keys.right = true;
	else if (keydata.key == MLX_KEY_ESCAPE)
	{
		cleanup(game);
		exit(EXIT_SUCCESS);
	}
}

void	handle_key_release(mlx_key_data_t keydata, t_game *game)
{
	if (keydata.key == MLX_KEY_W)
		game->keys.w = false;
	else if (keydata.key == MLX_KEY_A)
		game->keys.a = false;
	else if (keydata.key == MLX_KEY_S)
		game->keys.s = false;
	else if (keydata.key == MLX_KEY_D)
		game->keys.d = false;
	else if (keydata.key == MLX_KEY_LEFT)
		game->keys.left = false;
	else if (keydata.key == MLX_KEY_RIGHT)
		game->keys.right = false;
}

void	key_press(mlx_key_data_t keydata, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (keydata.action == MLX_PRESS)
		handle_key_press(keydata, game);
	else if (keydata.action == MLX_RELEASE)
		handle_key_release(keydata, game);
}
