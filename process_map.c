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

static void print_vector(t_vector *vector)
{
	size_t i;

	printf("Vector contains %zu lines:\n", vector->capacity);
	for (i = 0; i < vector->capacity; i++)
	{
		if (vector->symbols[i] != NULL)
		{
			if (is_empty_string(vector->symbols[i]))
				printf("Line %zu: (Empty)\n", i);
			else
				printf("Line %zu: %s", i, vector->symbols[i]);
		}
		else
			printf("Line %zu: (NULL)\n", i);
	}
}
static int	remove_empty_line(t_vector *map)
{
	size_t	i;
	char		*empty_line;

	i = 0;
	while (map->symbols[i] != NULL)
	{
		if (is_empty_string(map->symbols[i]))
		{
			empty_line = get_element_from_vector(map, i);
			if (empty_line == NULL)
			{
				log_error_message("Malloc failure");
				return (-1);
			}
			free(empty_line);
			empty_line = NULL;
			continue;
		}
		i++;
	}
	return (0);
}

bool	process_map(t_vector *map)
{
	remove_empty_line(map);
	print_vector(map);
	return (true);
}
