#include "../cub3d.h"

void	log_error_message(char *msg)
{
	ft_putstr_fd("Error", 2);
	ft_putstr_fd("\n", 2);
	ft_putstr_fd(msg, 2);
	ft_putstr_fd("\n", 2);
}

void	error_exit_cleanup(char *msg, t_vector *map, t_assets *assets)
{
	if (assets)
		free_assets_struct(assets);
	if (map)
		vector_free(map);
	if (msg)
		log_error_message(msg);
	exit(1);
}

t_player	set_player_not_found(char *error_message)
{
	t_player	player;

	player = (t_player){0};
	player.x = -1;
	player.y = -1;
	player.angle = -1.0f;
	if (error_message != NULL)
		log_error_message(error_message);
	return (player);
}
