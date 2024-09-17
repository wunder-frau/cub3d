#ifndef CUB3D_H
# define CUB3D_H

# include "libft/libft.h"
# include "MLX42/include/MLX42/MLX42.h"
# include "vector.h"
# include "fcntl.h"
# include <stdio.h>

typedef struct s_textures
{
	char	*path_NO;
	char	*path_SO;
	char	*path_EA;
	char	*path_WE;
}	t_textures;

typedef struct s_colors
{
	int	floor_rgb[3];
	int	ceiling_rgb[3];
}	t_colors;

typedef struct s_assets
{
	t_textures	textures;
	t_colors		colors;
	char				view_direction;
}	t_assets;

t_vector	*read_map(char **argv);
t_assets	*initialize_assets(t_vector *map);

#endif