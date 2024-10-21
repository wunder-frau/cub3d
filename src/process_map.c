#include "../cub3d.h"

static bool	is_empty_string(const char *line)
{
	size_t	i;

	i = 0;
	if (line == NULL || ft_strlen(line) == 0)
		return (true);
	while (line[i] != '\0')
	{
		if (!ft_is_space((unsigned char)line[i]))
			return (false);
		i++;
	}
	return (true);
}

static int	remove_empty_line(t_vector *map)
{
	size_t	i;
	char	*empty_line;

	i = 0;
	while (map->symbols[i] != NULL)
	{
		if (is_empty_string(map->symbols[i]))
		{
			empty_line = vector_get_at(map, i);
			if (empty_line == NULL)
			{
				log_error_message("Malloc failure");
				return (-1);
			}
			free(empty_line);
			empty_line = NULL;
			continue ;
		}
		i++;
	}
	return (0);
}

bool	process_map(t_vector *map)
{
	remove_empty_line(map);
	return (true);
}
