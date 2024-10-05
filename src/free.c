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

static void cleanup_textures_and_images(t_game *game)
{
	if (!game)
		return;

	if (game->assets.textures.NO)
		mlx_delete_texture(game->assets.textures.NO);
	if (game->assets.textures.SO)
		mlx_delete_texture(game->assets.textures.SO);
	if (game->assets.textures.EA)
		mlx_delete_texture(game->assets.textures.EA);
	if (game->assets.textures.WE)
		mlx_delete_texture(game->assets.textures.WE);
	if (game->image)
		mlx_delete_image(game->mlx, game->image);
}

static void	cleanup_map_and_game(t_game *game)
{
	int	y;

	if (!game)
		return;
	if (game->mapGrid && game->mapGrid->symbols)
	{
		y = 0;
		while (y < game->mapGrid->capacity)
		{
			if (game->mapGrid->symbols[y])
			{
				free(game->mapGrid->symbols[y]);
				game->mapGrid->symbols[y] = NULL;
			}
			y++;
		}
		free(game->mapGrid->symbols);
		game->mapGrid->symbols = NULL;
	}
	if (game->mapGrid)
		free(game->mapGrid);
	if (game->mlx)
		mlx_terminate(game->mlx);
	free(game);
}

void	cleanup(t_game *game)
{
	cleanup_textures_and_images(game);
	cleanup_map_and_game(game);
}
