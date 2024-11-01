/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:21:14 by istasheu          #+#    #+#             */
/*   Updated: 2024/11/01 11:01:12 by istasheu         ###   ########.fr       */
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
bool	error_msg(char *msg)
{
	ft_putendl_fd("Error", 2);
	ft_putendl_fd(msg, 2);
	return (false);
}

bool	check_map_dimensions(t_vector *map)
{
	size_t i;
	size_t j;

	printf("Starting check_map_dimensions\n");
	// Check the number of lines in the map (map height)
	printf("Map capacity (number of lines): %zu\n", map->capacity);
	if (map->capacity > MAX_MAP_LINES)
	{
		printf("Map too large - too many lines: %zu (Max allowed: %d)\n", map->capacity, MAX_MAP_LINES);  // Debug
		return (error_msg("Map too large - too many lines"));
	}
	// Check the length of each line in the map (map width)
	for (j = 0; j < map->capacity; j++)
	{
			i = 0;
			printf("Checking line %zu: %s", j, map->symbols[j]);
			while (map->symbols[j][i] != '\0')
			{
				i++;
				if (i > MAX_LINE_LENGTH)
				{
					printf("Line %zu too long: %zu characters (Max allowed: %d)\n", j, i, MAX_LINE_LENGTH);  // Debug
					return (error_msg("Map too large - line(s) too long"));
				}
			}
			printf("Line %zu length: %zu\n", j, i);
	}
	printf("Map dimensions are within acceptable limits.\n");
	return (true);
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
	if (!check_map_dimensions(map))
{
    vector_free(map);
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
