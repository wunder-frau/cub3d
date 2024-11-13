/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:21:14 by istasheu          #+#    #+#             */
/*   Updated: 2024/11/13 13:31:53 by istasheu         ###   ########.fr       */
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

bool	check_map_dimensions(t_vector *map)
{
	size_t	i;
	size_t	j;

	if (map->capacity > MAX_MAP_LINES)
	{
		log_error_message("Map too large - too many lines");
		return (false);
	}
	j = 0;
	while (j < map->capacity)
	{
	i = 0;
	while (map->symbols[j][i] != '\0')
	{
		i++;
		if (i > MAX_LINE_LENGTH)
		{
			log_error_message("Map too large - line(s) too long");
			return (false);
		}
	}
	j++;
	}
	return (true);
}

// int	main(int argc, char **argv)
// {
// 	t_vector	*map;
// 	t_assets	*assets;
// 	t_player	player;

// 	if (argc != 2)
// 	{
// 		log_error_message("Incorrect number of arguments");
// 		return (EXIT_FAILURE);
// 	}
// 	map = read_map(argv);
// 	if (map == NULL)
// 	{
// 		// VALGRIND!!!!!!!! change error message
// 		ft_putstr_fd("Error: Map is NULL.\n", 2);
// 		return (EXIT_FAILURE);
// 	}
// 	if (!check_map_dimensions(map))
// 	{
// 		vector_free(map);
// 		return (EXIT_FAILURE);
// 	}
// 	validate_map_file_structure(map);
// 	if (handle_assets_and_player(map, &assets, &player) != EXIT_SUCCESS)
// 		return (EXIT_FAILURE);
// 	if (raycast_engine(map, player, assets) != EXIT_SUCCESS)
// 		return (EXIT_FAILURE);
// 	vector_free(map);
// 	return (EXIT_SUCCESS);
// }

int	main(int argc, char **argv)
{
	t_vector	*map = NULL;
	t_assets	*assets = NULL;
	t_player	player;
	t_game		*game = malloc(sizeof(t_game));  // Assuming game struct is needed for cleanup
	if (!game)
		return (EXIT_FAILURE); // Exit if memory allocation for game fails

	game->mlx = NULL;  // Initialize any other fields in game to NULL for safety
	game->assets = (t_assets){0};  // Ensure assets are initialized

	if (argc != 2)
	{
		log_error_message("Incorrect number of arguments");
		cleanup(game);
		return (EXIT_FAILURE);
	}

	map = read_map(argv);
	if (!map)
	{
		ft_putstr_fd("Error: Map is NULL.\n", 2);
		cleanup(game);
		return (EXIT_FAILURE);
	}

	game->mapGrid = map;  // Assuming mapGrid is part of game struct
	if (!check_map_dimensions(map))
	{
		vector_free(map);
		cleanup(game);
		return (EXIT_FAILURE);
	}

	validate_map_file_structure(map);

	if (handle_assets_and_player(map, &assets, &player) != EXIT_SUCCESS)
	{
		cleanup(game);
		return (EXIT_FAILURE);
	}
	game->assets = *assets;  // Store assets in game for cleanup

	if (raycast_engine(map, player, assets) != EXIT_SUCCESS)
	{
		cleanup(game);
		return (EXIT_FAILURE);
	}

	// Normal program exit
	cleanup(game);
	return (EXIT_SUCCESS);
}

// int main(int argc, char **argv)
// {
// 	t_vector	*map;
// 	t_assets	*assets = NULL;
// 	t_player	player;
// 	if (argc != 2)
// 		return (log_error_message("Incorrect number of arguments"), EXIT_FAILURE);

// 	map = read_map(argv);
// 	if (!map || !check_map_dimensions(map))
// 		return (ft_putstr_fd("Error: Map is invalid.\n", 2), vector_free(map), EXIT_FAILURE);
// 	validate_map_file_structure(map);
// 	if (handle_assets_and_player(map, &assets, &player) != EXIT_SUCCESS)
// 	{
// 		vector_free(map);
// 		if (assets)
// 			free_assets_struct(assets); // Cleanup assets if allocated
// 		return (EXIT_FAILURE);
// 	}
// 	if (raycast_engine(map, player, assets) != EXIT_SUCCESS)
// 	{
// 		vector_free(map);
// 		if (assets)
// 			free_assets_struct(assets);
// 		return (EXIT_FAILURE);
// 	}
// 	vector_free(map);
// 	if (assets)
// 		free_assets_struct(assets);
// 	return (EXIT_SUCCESS);
// }
