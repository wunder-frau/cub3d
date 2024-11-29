/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/10 14:37:35 by nkarpilo          #+#    #+#             */
/*   Updated: 2024/11/29 16:29:43 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

uint32_t	retrieve_color_at_coordinates(int x, int y, t_texture_data *texture)
{
	int	index;

	if (x < 0 || x >= texture->width || y < 0 || y >= texture->height)
		return (0x000000FF);
	index = y * texture->width + x;
	return (texture->pixels[index]);
}

void	update(void *param)
{
	t_game	*game;
	float	move_speed;
	float	rot_speed;

	game = (t_game *)param;
	clear_image(game->image, 0x000000FF);
	move_speed = 1.0f;
	rot_speed = 2.0f * M_PI / 180.0f;
	handle_rotation(game, rot_speed);
	handle_movement(game, move_speed);
	render_frame(game);
}

static void	handle_close(void *param)
{
	t_game	*game;

	game = (t_game *)param;
	cleanup(game);
	exit(0);
}

int	raycast_engine(t_vector *map, t_player player, t_assets *assets)
{
	t_game	*game;

	game = initialize_game(map, player, assets);
	if (!game)
		return (EXIT_FAILURE);
	mlx_set_setting(MLX_STRETCH_IMAGE, true);
	mlx_loop_hook(game->mlx, &update, game);
	mlx_key_hook(game->mlx, &key_press, game);
	mlx_close_hook(game->mlx, &handle_close, game);
	mlx_loop(game->mlx);
	cleanup(game);
	return (EXIT_SUCCESS);
}
