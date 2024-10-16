#include "../cub3d.h"

static bool	is_config_line(char *str)
{
	while (ft_is_space(*str))
		str++;
	if (*str == '\0' || *str == '\n')
		return (false);
	if (ft_isalpha(*str) && ft_strlen(str) > 1)
		return (true);
	return (false);
}

static bool	is_blank_line(const char *str)
{
	while (*str)
	{
		if (!ft_is_space(*str))
			return (false);
		str++;
	}
	return (true);
}

static void	check_no_config_after_map(t_vector *map, size_t i)
{
	while (i < map->capacity)
	{
		if (is_config_line(map->symbols[i]))
			error_exit_cleanup("Texture configuration found after map data", map, NULL);
		i++;
	}
}

static int	count_config_lines(t_vector *map, size_t *i)
{
	int	texture_info_count;

	texture_info_count = 0;
	while (*i < map->capacity && is_blank_line(map->symbols[*i]))
		(*i)++;
	while (*i < map->capacity)
	{
		printf("Checking line %zu: '%s'\n", *i, map->symbols[*i]);
		if (is_config_line(map->symbols[*i]))
		{
			printf("-> Counted as asset line.\n");
			texture_info_count++;
		}
		else if (!is_blank_line(map->symbols[*i]))
		{
			printf("-> Found map data start.\n");
			break;
		}
		else
		{
			printf("-> Blank line.\n");
		}
		(*i)++;
	}
	return (texture_info_count);
}


void	validate_map_file_structure(t_vector *map)
{
	size_t i;
	int texture_info_count;

	i = 0;
	texture_info_count = count_config_lines(map, &i);
	printf("texture_lines_count:[%d]\n", texture_info_count);
	if (texture_info_count != 6)
	{
		error_exit_cleanup("Invalid number of texture config lines", map, NULL);
	}
	check_no_config_after_map(map, i);
}
