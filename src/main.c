#include "../cub3d.h"

static int	safe_open(const char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("Error opening the file\n", 2);
		close(fd);
		exit(1);
	}
	return (fd);
}

t_vector	*read_map(char **argv)
{
	int			map_fd;
	t_vector	*map;
	char		*map_line;

	map_fd = safe_open(argv[1]);
	if (map_fd < 0)
		ft_putstr_fd("Failed to open map", 2);
	map = vector_create(1);
	if (map == NULL)
	{
		close(map_fd);
		ft_putstr_fd("Malloc failure", 2);
	}
	while (1)
	{
		map_line = get_next_line(map_fd);
		if (map_line == NULL)
			break ;
		if (vector_push_back(map, map_line) == 1)
		{
			vector_free(map);
			close(map_fd);
			ft_putstr_fd("Malloc failure", 2);
		}
	}
	close(map_fd);
	return (map);
}


// int	main(int argc, char **argv)
// {
// 	t_vector	*map;
// 	t_assets	*assets;

// 	if (argc != 2)
// 		ft_putstr_fd("Incorrect number of arguments\n", 2);
// 	map = read_map(argv);
// 	assets = initialize_assets(map);

// 	printf("North Texture: %s\n", assets->textures.path_NO);
// 	printf("South Texture: %s\n", assets->textures.path_SO);
// 	printf("East Texture: %s\n", assets->textures.path_EA);
// 	printf("West Texture: %s\n", assets->textures.path_WE);
// 	printf("Floor Color: %d, %d, %d\n", assets->colors.rgb_F[0], assets->colors.rgb_F[1], assets->colors.rgb_F[2]);
// 	printf("Ceiling Color: %d, %d, %d\n", assets->colors.rgb_C[0], assets->colors.rgb_C[1], assets->colors.rgb_C[2]);

// 	printf("Original Vector:\n");
// 	vector_print(map->symbols[1]);

// 	// t_vector *copied_vector = vector_copy(map);
// 	// if (!copied_vector)
// 	// {
// 	// 	fprintf(stderr, "Failed to copy vector\n");
// 	// 	vector_free(map);
// 	// 	return EXIT_FAILURE;
// 	// }
// 	raycast_engine();
// 	printf("\nCopied Vector:\n");
// 	//vector_print(copied_vector);


// 	vector_free(map);
// //	vector_free(copied_vector);
// 	return (0);
// }

int main(int argc, char **argv) {
    t_vector *map;
    t_assets *assets;
   // t_game game; // Create an instance of your game structure

  	if (argc != 2)
		ft_putstr_fd("Incorrect number of arguments\n", 2);
	map = read_map(argv);
	assets = initialize_assets(map);
    if (!assets) {
        ft_putstr_fd("Failed to initialize assets\n", 2);
        vector_free(map);
        return EXIT_FAILURE;
    }

    // Print out the asset information for verification
    printf("North Texture: %s\n", assets->textures.path_NO);
    printf("South Texture: %s\n", assets->textures.path_SO);
    printf("East Texture: %s\n", assets->textures.path_EA);
    printf("West Texture: %s\n", assets->textures.path_WE);
    printf("Floor Color: %d, %d, %d\n", assets->colors.rgb_F[0], assets->colors.rgb_F[1], assets->colors.rgb_F[2]);
    printf("Ceiling Color: %d, %d, %d\n", assets->colors.rgb_C[0], assets->colors.rgb_C[1], assets->colors.rgb_C[2]);

    printf("Original Vector   llllllllll:\n");
    vector_print(map);

    // Pass the map to the game structure
    //game.mapGrid = map; // Assign the read map to the game structure
	//	vector_print(game.mapGrid);
		//printf("now_what_:%c\n", map->symbols[4][2]); 
		//printf("game__:%c\n", game.mapGrid->symbols[4][2]);
    // Initialize the game structure (including mlx, image, player)
    //init_game(&game, assets); // Assume init_game takes assets as an argument to setup the game

    // Start the raycasting engine or main game loop
    raycast_engine(map, assets); // Pass the game structure to the raycasting engine
		//raycast_engine();
    // Free the allocated resources
    vector_free(map); // Free the map
    //free_assets(assets); // Assume a function to free assets

    return EXIT_SUCCESS;
}