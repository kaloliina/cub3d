/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:46:14 by khiidenh          #+#    #+#             */
/*   Updated: 2025/06/25 13:06:26 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

/*
--11--
This function "combines" the different rgb values together into one integer.
By shifting r 24 bits left, g 16 bits left, g 8 bits left and leaving a (alpha channel)
where it is.
*/
static int	get_colour(int *rgb)
{
	int	colour;
	int	a;

	a = 255;
	colour = rgb[0] << 24 | rgb[1] << 16 | rgb[2] << 8 | a;
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

/*
--10--
This function right now goes through the height and width of the map and puts pixel by pixel to the image.
After we have filled the image, we put image to window.
*/
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

/*
--9--
This is basically just rendering the minimap on the top left corner, we can remove it if we want! :D
The function also calls to the render actual map which right now adds the floor and ceiling colors.
*/
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
/*
--8--
Here we are creating a new image that we will use for drawing floor and ceiling.
We are also loading the wall textures and putting the textures to image.
After this, we "render map"
*/
void	load_textures(t_game *game)
{
	int				i;
	mlx_texture_t	*texture;
	const char		**asset_paths;

	game->image = mlx_new_image(game->mlx, MAX_SCREEN_WIDTH, MAX_SCREEN_HEIGHT);
	i = 0;
	while (i < ASSET_COUNT)
	{
		texture = mlx_load_png(game->asset_paths[i]);
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
