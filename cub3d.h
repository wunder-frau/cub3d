#ifndef CUB3D_H
# define CUB3D_H

# include "libft/libft.h"
# include "MLX42/include/MLX42/MLX42.h"
# include "../src/vector/vector.h"
# include "fcntl.h"
# include <stdio.h>

#define TILE_SIZE 32 // Size of each tile in pixels

typedef struct s_textures
{
	char	*path_NO;
	char	*path_SO;
	char	*path_EA;
	char	*path_WE;
}	t_textures;

typedef struct s_colors
{
	int	rgb_F[3];
	int	rgb_C[3];
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
} t_game;

t_vector	*read_map(char **argv);
t_assets	*initialize_assets(t_vector *map);

/* error_handling.c */
void	log_error_message(char *msg);
void	error_exit_cleanup(char *msg, t_vector *map, t_assets *assets);

/* free.c */
void	free_assets_struct(t_assets *assets);

/* validation.c */
bool process_map(t_vector *map);

bool	validate_map(t_vector *map);

int raycast_engine(t_vector *map, t_player player, t_assets *assets);

// int find_player_pos(t_vector *map, t_player *player);
t_player find_player_pos(t_vector *map);

#endif