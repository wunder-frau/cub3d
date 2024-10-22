/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:12:14 by istasheu          #+#    #+#             */
/*   Updated: 2024/10/22 10:12:16 by istasheu         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub3d.h"

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

static int	open_map_file(char **argv)
{
	int	map_fd;

	map_fd = safe_open(argv[1]);
	if (map_fd < 0)
	{
		log_error_message("Failed to open map");
	}
	return (map_fd);
}

static t_vector	*initialize_map_vector(int map_fd)
{
	t_vector	*map;

	map = vector_create(1);
	if (map == NULL)
	{
		close(map_fd);
		log_error_message("Malloc failure");
		return (NULL);
	}
	return (map);
}

static bool	load_map_lines(int map_fd, t_vector *map)
{
	char	*map_line;

	while (1)
	{
		map_line = get_next_line(map_fd);
		if (map_line == NULL)
			break ;
		if (vector_push_back(map, map_line) == 1)
		{
			log_error_message("Malloc failure during map loading");
			free(map_line);
			return (false);
		}
	}
	return (true);
}

t_vector	*read_map(char **argv)
{
	int			map_fd;
	t_vector	*map;

	map_fd = open_map_file(argv);
	if (map_fd < 0)
		return (NULL);
	map = initialize_map_vector(map_fd);
	if (map == NULL)
		return (NULL);
	if (!load_map_lines(map_fd, map))
	{
		vector_free(map);
		close(map_fd);
		return (NULL);
	}
	close(map_fd);
	if (map->capacity == 0)
	{
		log_error_message("Map data is missing or empty.");
		vector_free(map);
		return (NULL);
	}
	return (map);
}