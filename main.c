#include "cub3d.h"

static int	safe_open(const char *file)
{
	int	fd;

	fd = open(file, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("Error\n opening the file\n", 2);
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
		ft_putstr_fd("failed to open map", 2);
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
		printf("Map line: %s\n", map_line);
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

	if (argc != 2)
		ft_putstr_fd("Incorrect number of arguments", 2);
	map = read_map(argv);
	free_vector(map);
}