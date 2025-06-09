/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:46:14 by khiidenh          #+#    #+#             */
/*   Updated: 2025/02/19 13:15:54 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	draw_asset(t_game *game, enum e_assets type, int x, int y)
{
	if (mlx_image_to_window(game->mlx, game->images[BASE],
			x * TILE, y * TILE) < 0)
		cleanup_and_exit(game, ERRIMG, 0);
	if (type == WALL || type == COLLECTABLE || type == EXIT_CLOSED)
	{
		if (mlx_image_to_window(game->mlx, game->images[type],
				x * TILE, y * TILE) < 0)
			cleanup_and_exit(game, ERRIMG, 0);
		if (type == EXIT_CLOSED)
		{
			if (mlx_image_to_window(game->mlx, game->images[EXIT_OPEN],
					x * TILE, y * TILE) < 0)
				cleanup_and_exit(game, ERRIMG, 0);
			game->images[EXIT_OPEN]->instances[0].enabled = false;
		}
	}
	if (type == PLAYER_LEFT)
	{
		if ((mlx_image_to_window(game->mlx, game->images[PLAYER_LEFT],
					game->player.x * TILE, game->player.y * TILE) < 0)
			|| (mlx_image_to_window(game->mlx, game->images[PLAYER_RIGHT],
					game->player.x * TILE, game->player.y * TILE) < 0))
			cleanup_and_exit(game, ERRIMG, 0);
		game->images[PLAYER_LEFT]->instances[0].enabled = false;
	}
}

static void	render_map(t_game *game)
{
	int	x;
	int	y;

	y = 0;
	while (game->height > y)
	{
		x = 0;
		while (game->width > x)
		{
			if (game->map[y][x] == '1')
				draw_asset(game, WALL, x, y);
			else if (game->map[y][x] == 'C')
				draw_asset(game, COLLECTABLE, x, y);
			else if (game->map[y][x] == 'E')
				draw_asset(game, EXIT_CLOSED, x, y);
			else
				draw_asset(game, BASE, x, y);
			x++;
		}
		y++;
	}
	draw_asset(game, PLAYER_LEFT, x, y);
}

static const char	**get_asset_paths(void)
{
	static const char	*asset_paths[ASSET_COUNT];

	asset_paths[0] = "assets/base.png";
	asset_paths[1] = "assets/wall.png";
	asset_paths[2] = "assets/collectable.png";
	asset_paths[3] = "assets/player_left.png";
	asset_paths[4] = "assets/player_right.png";
	asset_paths[5] = "assets/exit_closed.png";
	asset_paths[6] = "assets/exit_open.png";
	return (asset_paths);
}

void	load_textures(t_game *game)
{
	int				i;
	mlx_texture_t	*texture;
	const char		**asset_paths;

	asset_paths = get_asset_paths();
	i = 0;
	while (i < ASSET_COUNT)
	{
		texture = mlx_load_png(asset_paths[i]);
		if (texture == NULL)
			cleanup_and_exit(game, ERRPNG, 0);
		game->images[i] = mlx_texture_to_image(game->mlx, texture);
		if (game->images[i] == NULL)
		{
			mlx_delete_texture(texture);
			cleanup_and_exit(game, ERRCONV, 0);
		}
		mlx_delete_texture(texture);
		if ((mlx_resize_image(game->images[i], TILE, TILE)) == false)
			cleanup_and_exit(game, ERRRESIZE, 0);
		i++;
	}
	render_map(game);
}
