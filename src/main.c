/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:21:14 by istasheu          #+#    #+#             */
/*   Updated: 2024/10/31 23:49:35 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

static int	handle_assets_and_player(t_vector *map, t_assets **assets,
		t_player *player)
{
	*assets = initialize_assets(map);
	if (!*assets)
	{
		log_error_message("Failed to initialize assets");
		vector_free(map);
		return (EXIT_FAILURE);
	}
	*player = find_player_pos(map);
	if (player->x == -1 && player->y == -1)
	{
		error_exit_cleanup("Player not found in the map.", map, *assets);
		return (EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}

int	main(int argc, char **argv)
{
	t_vector	*map;
	t_assets	*assets;
	t_player	player;

	if (argc != 2)
	{
		log_error_message("Incorrect number of arguments");
		return (EXIT_FAILURE);
	}
	map = read_map(argv);
	if (map == NULL)
	{
		// VALGRIND!!!!!!!! change error message
		ft_putstr_fd("Error: Map is NULL.\n", 2);
		return (EXIT_FAILURE);
	}
	validate_map_file_structure(map);
	if (!map)
		return (EXIT_FAILURE);
	if (handle_assets_and_player(map, &assets, &player) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	if (raycast_engine(map, player, assets) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	vector_free(map);
	return (EXIT_SUCCESS);
}
