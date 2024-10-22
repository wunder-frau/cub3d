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

bool	is_valid_line_len_and_nl(const char *line, t_line_type line_type)
{
	size_t	len;

	len = ft_strlen(line);
	if (len < 2 || line[len - 1] != '\n')
	{
		if (line_type == LINE_TOP)
			log_error_message("Invalid top line: "
				"line too small or missing '\\n'");
		else if (line_type == LINE_BOTTOM)
			log_error_message("Invalid bottom line: "
				"line too small or missing '\\n'");
		else if (line_type == LINE_MIDDLE)
			log_error_message("Invalid middle line: "
				"line too small or missing '\\n'");
		return (false);
	}
	return (true);
}

bool	is_valid_line_characters(const char *line, const char *valid_chars,
	t_line_type line_type)
{
	size_t	len;
	size_t	i;

	len = ft_strlen(line);
	i = 0;
	while (i < len - 1)
	{
		if (ft_strchr(valid_chars, line[i]) == NULL)
		{
			if (line_type == LINE_TOP)
				log_error_message("Invalid character "
					"found in the top line of the map");
			else if (line_type == LINE_BOTTOM)
				log_error_message("Invalid character "
					"found in the bottom line of the map");
			else if (line_type == LINE_MIDDLE)
				log_error_message("Invalid character "
					"found in the middle line of the map");
			return (false);
		}
		i++;
	}
	return (true);
}
