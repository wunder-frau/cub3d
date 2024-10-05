#include "../cub3d.h"

t_game	*allocate_game_structure(t_vector *map, t_player player, t_assets *assets)
{
	t_game *game;

	game = malloc(sizeof(t_game));
	if (!game)
	{
		ft_putstr_fd("Failed to allocate memory for game structure\n", 2);
		return (NULL);
	}
	game->mlx = NULL;
	game->image = NULL;
	game->mapGrid = map;
	game->assets = *assets;
	game->player.x = player.x;
	game->player.y = player.y;
	game->player.angle = player.angle;
	ft_memcpy(game->assets.colors.rgb_C, assets->colors.rgb_C, sizeof(assets->colors.rgb_C));
	ft_memcpy(game->assets.colors.rgb_F, assets->colors.rgb_F, sizeof(assets->colors.rgb_F));
	return (game);
}

mlx_texture_t	*load_texture(const char *path)
{
	mlx_texture_t *texture;

	texture = mlx_load_png(path);
	printf("Loading texture from path: %s\n", path);
	if (!texture)
	{
		ft_putstr_fd("Failed to load texture: ", 2);
		ft_putstr_fd((char *)path, 2);
		ft_putstr_fd("\n", 2);
	}
	return (texture);
}

int	initialize_ass(t_vector *map, t_assets *assets)
{
	assets->textures.NO = load_texture(assets->textures.path_NO);
	assets->textures.SO = load_texture(assets->textures.path_SO);
	assets->textures.EA = load_texture(assets->textures.path_EA);
	assets->textures.WE = load_texture(assets->textures.path_WE);
	if (!assets->textures.NO || !assets->textures.SO || !assets->textures.EA || !assets->textures.WE)
	{
		ft_putstr_fd("Failed to load all required textures\n", 2);
		error_exit_cleanup("Map validation failed", map, assets);
		return (-1);
	}
	return (0);
}

int	setup_mlx_and_image(t_game *game)
{
	game->mlx = mlx_init(WIDTH, HEIGHT, "CUB 3D", true);
	if (!game->mlx)
	{
		ft_putstr_fd("Failed to initialize MLX42\n", 2);
		return (-1);
	}
	game->image = mlx_new_image(game->mlx, WIDTH, HEIGHT);
	if (!game->image)
	{
		ft_putstr_fd("Failed to create image\n", 2);
		mlx_terminate(game->mlx);
		return (-1);
	}
	if (mlx_image_to_window(game->mlx, game->image, 0, 0) < 0)
	{
		ft_putstr_fd("Failed to attach image to window\n", 2);
		mlx_delete_image(game->mlx, game->image);
		mlx_terminate(game->mlx);
		return (-1);
	}
	return (0);
}

int	initialize_graphics(t_game *game)
{
	if (setup_mlx_and_image(game) != 0)
		return (-1);
	if (initialize_ass(game->mapGrid, &game->assets) != 0)
	{
		mlx_delete_image(game->mlx, game->image);
		mlx_terminate(game->mlx);
		return (-1);
	}
	return (0);
}

t_game	*initialize_game(t_vector *map, t_player player, t_assets *assets)
{
	t_game *game;

	game = allocate_game_structure(map, player, assets);
	if (!game)
		return (NULL);
	if (initialize_graphics(game) != 0)
	{
		cleanup(game);
		return (NULL);
	}
	return (game);
}
