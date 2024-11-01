/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_map.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: istasheu <istasheu@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/22 10:12:14 by istasheu          #+#    #+#             */
/*   Updated: 2024/11/01 10:05:14 by istasheu         ###   ########.fr       */
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

	if (!is_cub(argv[1]))
	{
		log_error_message("Invalid file format. Use a .cub file.");
		exit(1);
	}
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

// static bool check_file_height(t_vector *map, char *map_line)
// {
//     if (map->length >= 500)
//     {
//         free(map_line);
//         log_error_message("Map is too large.");
//         return false;  // Indicate failure to the caller
//     }
//     return true;
// }


static void check_file_height(t_vector *map, char *map_line, int fd)
{
    printf("Checking file height: current number of lines read: %zu\n", map->capacity);

    if (map->capacity >= MAX_FILE_NODES)
    {
        printf("File too large: %zu lines read (maximum allowed is %d)\n", map->capacity, MAX_FILE_NODES);
        free(map_line);
        vector_free(map);
        close(fd);
				ft_putendl_fd("Error", 2);
				ft_putendl_fd("Config file too large", 2);
				exit(1);
    }
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
		check_file_height(map, map_line, map_fd);
	}
	return (true);
}

// t_vector	*read_map(char **argv)
// {
// 	int			map_fd;
// 	t_vector	*map;

// 	map_fd = open_map_file(argv);
// 	if (map_fd < 0)
// 		return (NULL);
// 	map = initialize_map_vector(map_fd);
// 	if (map == NULL)
// 		return (NULL);
// 	if (!load_map_lines(map_fd, map))
// 	{
// 		vector_free(map);
// 		close(map_fd);
// 		return (NULL);
// 	}
// 	close(map_fd);
// 	if (map->length >= 500)
// 	{
// 		ft_printf("cap:%d\n", map->capacity);
// 		ft_printf("len:%d\n", map->length);
// 		log_error_message("Map is to large.");
// 		vector_free(map);
// 		return (NULL);
// 	}
// 	if (map->capacity == 0)
// 	{
// 		log_error_message("Map data is missing or empty.");
// 		vector_free(map);
// 		return (NULL);
// 	}
// 	return (map);
// }

t_vector	*read_map(char **argv)
{
	int			map_fd;
	t_vector	*map;
	char		*map_line;

	map_fd = safe_open(argv[1]);
	if (map_fd < 0)
	{
		ft_putstr_fd("Failed to open map\n", 2);
		return (NULL);
	}
	map = vector_create(1);
	if (map == NULL)
	{
		close(map_fd);
		ft_putstr_fd("Malloc failure\n", 2);
		return (NULL);
	}
	while (1)
	{
		map_line = get_next_line(map_fd);
		if (map_line == NULL)
			break ;
		if (vector_push_back(map, map_line) == 1)
		{
			ft_putstr_fd("Malloc failure during map loading\n", 2);
			free(map_line);
			vector_free(map);
			close(map_fd);
			return (NULL);
		}
		//vector_print(map);
	}
	close(map_fd);
	if (map->capacity == 0)
	{
		ft_putstr_fd("Map data is missing or empty.\n", 2);
		vector_free(map);
		return NULL;
	}
	// 	if (map->length >= 500)
	// {
	// 	// ft_printf("cap:%d\n", map->capacity);
	// 	// ft_printf("len:%d\n", map->length);
	// 	ft_putstr_fd("Map is to large.\n", 2);
	// 	vector_free(map);
	// 	return (NULL);
	// }
	return (map);
}