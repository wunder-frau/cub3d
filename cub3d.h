#ifndef CUB3D_H
# define CUB3D_H

# include "libft/libft.h"
# include "MLX42/include/MLX42/MLX42.h"
# include "../src/vector/vector.h"
# include <fcntl.h>
# include <stdlib.h>
# include <stdio.h>
# include <math.h>

# ifndef M_PI
#  define M_PI 3.14159265358979323846
# endif

# define WIDTH 1200
# define HEIGHT 800
# define MAX_DEPTH 16 // Increase from 16 to 64
# define MAX_RENDER_DISTANCE 1000.0f // Adjust as needed
# define MAX_VIEW_DISTANCE 600.0f 

# define FOV 75 // Field of View in degrees
# define NUM_RAYS WIDTH // One ray per screen column

# define TILE_SIZE 32 // Size of each tile in pixels

#define MAX_MAP_LINES 64        // Maximum number of lines in the map
#define MAX_LINE_LENGTH 64      // Maximum number of characters in a line
#define MAX_FILE_NODES 500

typedef struct s_textures
{
	char	*path_NO;
	char	*path_SO;
	char	*path_EA;
	char	*path_WE;
	mlx_texture_t	*NO;
	mlx_texture_t	*SO;
	mlx_texture_t	*EA;
	mlx_texture_t	*WE;
}	t_textures;

typedef struct s_colors
{
	int	rgb_C[3];
	int	rgb_F[3];
}	t_colors;

typedef struct s_assets
{
	t_textures	textures;
	t_colors		colors;
	char				view_direction;
}	t_assets;

typedef struct s_player {
    float x;
    float y;
    float angle;
} t_player;

typedef struct s_game {
    mlx_t *mlx;
    mlx_image_t *image;
    t_player player;
    t_vector *mapGrid;
		t_assets assets;
} t_game;

typedef enum e_line_type
{
	LINE_TOP,
	LINE_BOTTOM,
	LINE_MIDDLE
} t_line_type;

/* allocate.c */
t_game	*initialize_game(t_vector *map, t_player player, t_assets *assets);

/* boundary_lines_validation.c */
bool	is_valid_boundaries(t_vector *map);

/* config_order_validation.c */
void	validate_map_file_structure(t_vector *map);

/* error_handling.c */
void	log_error_message(char *msg);
void	error_exit_cleanup(char *msg, t_vector *map, t_assets *assets);
t_player	set_player_not_found(char *error_message);
bool	is_valid_line_len_and_nl(const char *line, t_line_type line_type);
bool	is_valid_line_characters(const char *line, const char *valid_chars,
	t_line_type line_type);

/* flood_fill.c */
void	validate_map_with_flood_fill(t_vector *map);

/* free.c */
void	free_assets_struct(t_assets *assets);
void	free_split_rgb_array(char **rgb_arr);
void	cleanup(t_game *game);
void	free_split_rgb_array(char **rgb_arr);

/* main.c */
int	main(int argc, char **argv);

/* map_validation.c */
bool	is_cub(const char *str);
void	space_to_wall(t_vector *map);
bool	validate_map(t_vector *map, t_assets *assets);

/* parser_config.c */
t_assets	*initialize_assets(t_vector *map);

/* player_position.c */
int	count_players(t_vector *map);
t_player	find_player_pos(t_vector *map);

/*read_map.c */
t_vector	*read_map(char **argv);

/* utils.c */
char	*trim_and_extract(char *str, unsigned int prefix_len);
bool	is_empty_string(const char *line);
int	remove_empty_line(t_vector *map);
bool	validate_map_presence(t_vector *map);
bool	is_blank_line(const char *str);

/* utils_symbol.c */
float	assign_player_angle(char symbol);
bool	is_player_symbol(char c);
bool	is_valid_map_symbol(char c, t_vector *map);

/* cub3d.c */
int raycast_engine(t_vector *map, t_player player, t_assets *assets);

#endif