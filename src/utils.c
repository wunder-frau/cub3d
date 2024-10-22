#include "../cub3d.h"

bool	is_empty_string(const char *line)
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

int	remove_empty_line(t_vector *map)
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

/**
 * Trims the identifier (NO, SO, EA, WE, F, C)
 * from the config line and removes trailing spaces and newlines.
 */
char	*trim_and_extract(char *str, unsigned int prefix_len)
{
	unsigned int	end;
	char			*trimmed;

	while (str[prefix_len] == ' ' || str[prefix_len] == '\n')
		prefix_len++;
	end = ft_strlen(str);
	while (end > prefix_len && (str[end - 1] == ' ' || str[end - 1] == '\n'))
		end--;
	trimmed = ft_substr(str, prefix_len, end - prefix_len);
	free(str);
	str = NULL;
	return (trimmed);
}

bool	validate_map_presence(t_vector *map)
{
	if (!map || map->capacity == 0)
	{
		log_error_message("Map data is missing or empty.");
		return (false);
	}
	return (true);
}

bool	is_blank_line(const char *str)
{
	while (*str)
	{
		if (!ft_is_space(*str))
			return (false);
		str++;
	}
	return (true);
}
