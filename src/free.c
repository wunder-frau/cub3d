#include "../cub3d.h"

void	free_assets_struct(t_assets *assets)
{
	if (assets->textures.path_EA)
		free(assets->textures.path_EA);
	if (assets->textures.path_NO)
		free(assets->textures.path_NO);
	if (assets->textures.path_SO)
		free(assets->textures.path_SO);
	if (assets->textures.path_WE)
		free(assets->textures.path_WE);
}