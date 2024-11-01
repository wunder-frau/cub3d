/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:21:14 by istasheu          #+#    #+#             */
/*   Updated: 2024/11/01 10:34:54 by istasheu         ###   ########.fr       */
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

bool check_map_dimensions(t_vector *map)
{
    size_t i;
    size_t j;

    i = 0;
    j = 0;

    printf("Starting check_map_dimensions\n");
    printf("len: %zu\n", map->length);

    if (map->capacity >= 65)
    {
        printf("capacity: %zu (maximum allowed is 64)\n", map->capacity);
        return (error_msg("Map too large - too many lines"));
    }

    while (j < map->capacity)
    {
        printf("Checking line %zu\n", j);
        while (map->symbols[j][i] != '\0')
        {
            if (i == 0)
            {
                printf("Line %zu content: ", j);
            }
            printf("%c", map->symbols[j][i]); // Print each character
            i++;
            if (i == 65)
            {
                printf("\nLine %zu is too long: %zu characters (maximum allowed is 64)\n", j, i);
                return (error_msg("Map too large - line(s) too long"));
            }
        }
        printf("\nLine %zu length: %zu characters\n", j, i);
        i = 0;
        j++;
    }

    printf("Map dimensions are valid.\n");
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
