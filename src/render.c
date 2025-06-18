/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:46:14 by khiidenh          #+#    #+#             */
/*   Updated: 2025/06/18 13:34:12 by khiidenh         ###   ########.fr       */
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

// *
//  * @param[in] image The MLX instance handle.
//  * @param[in] x The X coordinate position.
//  * @param[in] y The Y coordinate position.
//  * @param[in] color The color value to put.
//  */
// void mlx_put_pixel(mlx_image_t* image, uint32_t x, uint32_t y, uint32_t color);

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
	if (type == EMPTY)
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

static void	render_map(t_game *game)
{
	int	x;
	int	y;
	int	tx;
	int	ty;

	y = 0;
	while (game->map[y] != NULL)
	{
		x = 0;
		while (x < (int)ft_strlen(game->map[y]))
		{

			if (game->map[y][x] == '1')
				draw_asset(game, WALL, x, y);
			else if (game->map[y][x] == ' ')
				draw_asset(game, EMPTY, x, y);
			else if (game->map[y][x] == '0')
			{
				ty = 0;
				while (ty < TILE)
				{
					tx = 0;
					while (tx < TILE)
					{
						mlx_put_pixel(game->image, x * TILE + tx, y * TILE + ty,get_colour(game->floor_rgb));
						tx++;
					}
				ty++;
				}
			}
			x++;
		}
		y++;
	}
	mlx_image_to_window(game->mlx, game->image, 0, 0);
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

	game->image = mlx_new_image(game->mlx, game->width * TILE, game->height  * TILE);
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
