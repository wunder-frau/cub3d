/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkarpilo <nkarpilo@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 14:37:38 by nkarpilo          #+#    #+#             */
/*   Updated: 2024/11/10 14:39:33 by nkarpilo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

t_keys	g_keys = {false, false, false, false, false, false};

void	handle_key_press(mlx_key_data_t keydata, t_game *game)
{
	if (keydata.key == MLX_KEY_W)
		g_keys.w = true;
	else if (keydata.key == MLX_KEY_A)
		g_keys.a = true;
	else if (keydata.key == MLX_KEY_S)
		g_keys.s = true;
	else if (keydata.key == MLX_KEY_D)
		g_keys.d = true;
	else if (keydata.key == MLX_KEY_LEFT)
		g_keys.left = true;
	else if (keydata.key == MLX_KEY_RIGHT)
		g_keys.right = true;
	else if (keydata.key == MLX_KEY_ESCAPE)
	{
		cleanup(game);
		exit(EXIT_SUCCESS);
	}
}

void	handle_key_release(mlx_key_data_t keydata)
{
	if (keydata.key == MLX_KEY_W)
		g_keys.w = false;
	else if (keydata.key == MLX_KEY_A)
		g_keys.a = false;
	else if (keydata.key == MLX_KEY_S)
		g_keys.s = false;
	else if (keydata.key == MLX_KEY_D)
		g_keys.d = false;
	else if (keydata.key == MLX_KEY_LEFT)
		g_keys.left = false;
	else if (keydata.key == MLX_KEY_RIGHT)
		g_keys.right = false;
}

void	key_press(mlx_key_data_t keydata, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (keydata.action == MLX_PRESS)
		handle_key_press(keydata, game);
	else if (keydata.action == MLX_RELEASE)
		handle_key_release(keydata);
}
