#include "../cub3d.h"

static bool	validate_map_presence(t_vector *map)
{
	if (!map || map->capacity == 0)
	{
		log_error_message("Map data is missing or empty.");
		return (false);
	}
	return (true);
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

static bool	is_valid_boundary_line(const char *line)
{
	size_t	len;
	size_t	i;
	char	c;

	len = ft_strlen(line);
	if (len < 2 || line[len - 1] != '\n')
	{
		log_error_message("Invalid line format: "
			"line too small or missing '\\n'");
		return (false);
	}
	i = 0;
	while (i < len - 1)
	{
		c = line[i];
		if (c != '1' && c != ' ')
		{
			log_error_message("Invalid character found in the line of the map");
			return (false);
		}
		i++;
	}
	return (true);
}

bool	validate_line(const char *line, const char *valid_chars,
	t_line_type line_type)
{
	size_t	i;

	if (!is_valid_line_len_and_nl(line, line_type))
		return (false);
	if (!is_valid_line_characters(line, valid_chars, line_type))
		return (false);
	if (line_type == LINE_MIDDLE)
	{
		i = 0;
		while (line[i] == ' ')
			i++;
		if (line[i] != '1')
		{
			log_error_message("Map middle line must start with wall '1'");
			return (false);
		}
		while (line[i] != '\n')
			i++;
		if (i > 0 && line[i - 1] != '1')
		{
			log_error_message("Map middle line must end with wall '1'");
			return (false);
		}
	}
	return (true);
}

bool	is_valid_boundaries(t_vector *map)
{
	size_t	i;

	if (!validate_map_presence(map) || map->capacity < 3)
	{
		log_error_message("Map too small or not initialized properly");
		return (false);
	}
	if (!validate_line(map->symbols[0], "1 ", LINE_TOP))
		return (false);
	i = 1;
	while (i < (map->capacity - 1))
	{
		if (!validate_line(map->symbols[i], "NESW10 ", LINE_MIDDLE))
			return (false);
		i++;
	}
	if (!validate_line(map->symbols[map->capacity - 1], "1 ", LINE_BOTTOM))
		return (false);
	return (true);
}
