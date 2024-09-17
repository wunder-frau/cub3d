#include "../cub3d.h"

void	print_error_map(char *msg, t_vector *map, t_assets *assets)
{
	if (assets)
		free_assets_struct(assets);
	if (map)
		free_vector(map);
	if (msg)
	{
		ft_putendl_fd("Error", 2);
		ft_putendl_fd(msg, 2);
	}
	exit(1);
}
