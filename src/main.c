#include "../cub3d.h"

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
	if (!is_cub(argv[1]))
	{
		log_error_message("Error\nInvalid file format. Use a .cub file.");
		exit(1);
	}
	map = read_map(argv);
	validate_map_file_structure(map);
	if (!map)
		return (EXIT_FAILURE);
	assets = initialize_assets(map);
	if (!assets)
	{
		log_error_message("Failed to initialize assets");
		vector_free(map);
		return (EXIT_FAILURE);
	}
	// if (!validate_map(map, assets))
	// 	error_exit_cleanup("Map validation failed", map, assets);
	player = find_player_pos(map);
	if (player.x == -1 && player.y == -1)
		error_exit_cleanup("Player not found in the map.\n", map, assets);
	if (raycast_engine(map, player, assets) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	vector_free(map);
	return (EXIT_SUCCESS);
}
