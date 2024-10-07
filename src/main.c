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
	{
		ft_putstr_fd("Failed to open map\n", 2);
		return (NULL);
	}
	map = vector_create(1);
	if (map == NULL)
	{
		close(map_fd);
		ft_putstr_fd("Malloc failure\n", 2);
		return (NULL);
	}
	while (1)
	{
		map_line = get_next_line(map_fd);
		if (map_line == NULL)
			break ;
		if (vector_push_back(map, map_line) == 1)
		{
			ft_putstr_fd("Malloc failure during map loading\n", 2);
			free(map_line);
			vector_free(map);
			close(map_fd);
			return (NULL);
		}
	}
	close(map_fd);
	if (map->capacity == 0)
	{
		ft_putstr_fd("Map data is missing or empty.\n", 2);
		vector_free(map);
		return NULL;
	}
	return (map);
}

int	main(int argc, char **argv)
{
	t_vector	*map;
	t_assets	*assets;
	t_player	player;

	if (argc != 2)
	{
		ft_putstr_fd("Incorrect number of arguments\n", 2);
		return (EXIT_FAILURE);
	}
	map = read_map(argv);
	if (!map)
		return (EXIT_FAILURE);
	assets = initialize_assets(map);
	if (!assets)
	{
		ft_putstr_fd("Failed to initialize assets\n", 2);
		vector_free(map);
		return (EXIT_FAILURE);
	}
	if (!validate_map(map, assets))
		error_exit_cleanup("Map validation failed\n", map, assets);
	printf("N Texture: %s\n", assets->textures.path_NO);
	printf("S Texture: %s\n", assets->textures.path_SO);
	printf("E Texture: %s\n", assets->textures.path_EA);
	printf("W Texture: %s\n", assets->textures.path_WE);
	printf("F Color: %d, %d, %d\n", assets->colors.rgb_F[0], assets->colors.rgb_F[1], assets->colors.rgb_F[2]);
	printf("C Color: %d, %d, %d\n", assets->colors.rgb_C[0], assets->colors.rgb_C[1], assets->colors.rgb_C[2]);
	player = find_player_pos(map);
	if (player.x == -1 && player.y == -1)
	{
		fprintf(stderr, "Player not found in the map.\n");
		error_exit_cleanup("Player not found in the map.\n", map, assets);
	}
	if (raycast_engine(map, player, assets) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	vector_free(map);
	return (EXIT_SUCCESS);
}
