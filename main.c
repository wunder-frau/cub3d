#include "cub3d.h"

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
	map = vector_new(1);
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
		if (vector_add_back(map, map_line) == 1)
		{
			free_vector(map);
			close(map_fd);
			ft_putstr_fd("Malloc failure", 2);
		}
	}
	close(map_fd);
	return (map);
}


int	main(int argc, char **argv)
{
	t_vector	*map;
	t_assets	*assets;

	if (argc != 2)
		ft_putstr_fd("Incorrect number of arguments\n", 2);
	map = read_map(argv);
	assets = initialize_assets(map);

	printf("North Texture: %s\n", assets->textures.path_NO);
	printf("South Texture: %s\n", assets->textures.path_SO);
	printf("East Texture: %s\n", assets->textures.path_EA);
	printf("West Texture: %s\n", assets->textures.path_WE);
	printf("Floor Color: %d, %d, %d\n", assets->colors.rgb_F[0], assets->colors.rgb_F[1], assets->colors.rgb_F[2]);
	printf("Ceiling Color: %d, %d, %d\n", assets->colors.rgb_C[0], assets->colors.rgb_C[1], assets->colors.rgb_C[2]);


	
	free_vector(map);
	return (0);
}