/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:46:14 by khiidenh          #+#    #+#             */
/*   Updated: 2025/06/18 14:57:49 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static int	get_colour(int *rgb)
{
	int	colour;
	int a;

	a = 255;
	colour =  rgb[0] << 24 | rgb[1] << 16 | rgb[2] << 8 | a;
	return (colour);
}

static void	draw_asset(t_game *game, enum e_assets type, int x, int y)
{
	if (type == BASE)
	{
	if (mlx_image_to_window(game->mlx, game->images[BASE],
			x * TILE, y * TILE) < 0)
		cleanup_and_exit(game, ERRIMG, 0);
	}
	if (type == WALL)
	{
		if (mlx_image_to_window(game->mlx, game->images[type],
				x * TILE, y * TILE) < 0)
			cleanup_and_exit(game, ERRIMG, 0);
	}
	if (type == PLAYER)
	{
		if (mlx_image_to_window(game->mlx, game->images[PLAYER],
					game->player.x * TILE, game->player.y * TILE) < 0)
			cleanup_and_exit(game, ERRIMG, 0);
	}
}

static void	render_actualmap(t_game *game)
{
	int	x;
	int	y;

	x = 0;
	y = 0;
	while (y < MAX_SCREEN_HEIGHT / 2)
	{
		x = 0;
		while (x < MAX_SCREEN_WIDTH)
		{
			mlx_put_pixel(game->image, x, y, get_colour(game->ceiling_rgb));
			x++;
		}
		y++;
	}
	mlx_image_to_window(game->mlx, game->image, 0, 0);
	y = MAX_SCREEN_HEIGHT / 2;
	while (y < MAX_SCREEN_HEIGHT)
	{
		x = 0;
		while (x < MAX_SCREEN_WIDTH)
		{
			mlx_put_pixel(game->image, x, y, get_colour(game->floor_rgb));
			x++;
		}
		y++;
	}
	mlx_image_to_window(game->mlx, game->image, 0, 0);
}

static void	render_map(t_game *game)
{
	int	x;
	int	y;

	y = 0;
	render_actualmap(game);
	while (game->map[y] != NULL)
	{
		x = 0;
		while (x < (int)ft_strlen(game->map[y]))
		{
			if (game->map[y][x] == '1')
				draw_asset(game, WALL, x, y);
			else if (game->map[y][x] == '0')
			{
				draw_asset(game, BASE, x, y);
			}
			x++;
		}
		y++;
	}
	draw_asset(game, PLAYER, x, y);
}

static const char	**get_asset_paths(void)
{
	static const char	*asset_paths[ASSET_COUNT];

	asset_paths[0] = "assets/base.png";
	asset_paths[1] = "assets/wall.png";
	asset_paths[2] = "assets/player_left.png";
	asset_paths[3] = "assets/exit_open.png";
	return (asset_paths);
}

void	load_textures(t_game *game)
{
	int				i;
	mlx_texture_t	*texture;
	const char		**asset_paths;

	game->image = mlx_new_image(game->mlx, MAX_SCREEN_WIDTH, MAX_SCREEN_HEIGHT);
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
