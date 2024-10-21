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

// t_vector	*read_map(char **argv)
// {
// 	int			map_fd;
// 	t_vector	*map;
// 	char		*map_line;

// 	map_fd = safe_open(argv[1]);
// 	if (map_fd < 0)
// 	{
// 		log_error_message("Failed to open map");
// 		return (NULL);
// 	}
// 	map = vector_create(1);
// 	if (map == NULL)
// 	{
// 		close(map_fd);
// 		log_error_message("Malloc failure");
// 		return (NULL);
// 	}
// 	while (1)
// 	{
// 		map_line = get_next_line(map_fd);
// 		if (map_line == NULL)
// 			break ;
// 		if (vector_push_back(map, map_line) == 1)
// 		{
// 			ft_putstr_fd("Malloc failure during map loading\n", 2);
// 			free(map_line);
// 			vector_free(map);
// 			close(map_fd);
// 			return (NULL);
// 		}
// 	}
// 	close(map_fd);
// 	if (map->capacity == 0)
// 	{
// 		log_error_message("Map data is missing or empty.");
// 		vector_free(map);
// 		return (NULL);
// 	}
// 	return (map);
//}

int	open_map_file(char **argv)
{
	int map_fd = safe_open(argv[1]);
	if (map_fd < 0)
	{
		log_error_message("Failed to open map");
	}
	return (map_fd);
}

static t_vector	*initialize_map_vector(int map_fd)
{
	t_vector	*map;

	map = vector_create(1);
	if (map == NULL)
	{
		close(map_fd);
		log_error_message("Malloc failure");
		return (NULL);
	}
	return (map);
}

static bool	load_map_lines(int map_fd, t_vector *map)
{
	char	*map_line;

	while (1)
	{
		map_line = get_next_line(map_fd);
		if (map_line == NULL)
				break ;
		if (vector_push_back(map, map_line) == 1)
		{
			log_error_message("Malloc failure during map loading");
			free(map_line);
			return (false);
		}
	}
	return (true);
}

t_vector	*read_map(char **argv)
{
	int			map_fd;
	t_vector	*map;

	map_fd = open_map_file(argv);
	if (map_fd < 0)
		return (NULL);
	map = initialize_map_vector(map_fd);
	if (map == NULL)
		return (NULL);
	if (!load_map_lines(map_fd, map))
	{
		vector_free(map);
		close(map_fd);
		return (NULL);
	}
	close(map_fd);
	if (map->capacity == 0)
	{
		log_error_message("Map data is missing or empty.");
		vector_free(map);
		return (NULL);
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
	if (!validate_map(map, assets))
		error_exit_cleanup("Map validation failed", map, assets);
	player = find_player_pos(map);
	if (player.x == -1 && player.y == -1)
		error_exit_cleanup("Player not found in the map.\n", map, assets);
	if (raycast_engine(map, player, assets) != EXIT_SUCCESS)
		return (EXIT_FAILURE);
	vector_free(map);
	return (EXIT_SUCCESS);
}
