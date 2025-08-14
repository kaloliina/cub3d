/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_bonus.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 12:02:35 by sojala            #+#    #+#             */
/*   Updated: 2025/08/14 12:02:36 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D_bonus.h"

static void	draw_ceiling_floor(t_game *game)
{
	int	x;
	int	y;

	y = 0;
	while (y < MAX_SCREEN_HEIGHT / 2)
	{
		x = 0;
		while (x < MAX_SCREEN_WIDTH)
		{
			mlx_put_pixel(game->image, x, y, get_color(game->ceiling_rgb));
			x++;
		}
		y++;
	}
	y = MAX_SCREEN_HEIGHT / 2;
	while (y < MAX_SCREEN_HEIGHT)
	{
		x = 0;
		while (x < MAX_SCREEN_WIDTH)
		{
			mlx_put_pixel(game->image, x, y, get_color(game->floor_rgb));
			x++;
		}
		y++;
	}
}

/*In the loop, we go through the screen, drawing the walls one vertical stripe
at a time.
Get_line_properties finds the upcoming wall as we move along the ray, and
get_wallhitpoint tells us the exact wall hitting point of the ray.*/
void	render_map(t_game *game)
{
	int		x;
	t_dda	*dda;
	double	wallhitpoint;
	double	z_buffer[MAX_SCREEN_WIDTH];

	draw_ceiling_floor(game);
	dda = malloc(sizeof(t_dda));
	if (!dda)
		cleanup_and_exit(game, ERRMEM, 0, 1);
	init_dda(dda, game);
	x = 0;
	while (x < MAX_SCREEN_WIDTH)
	{
		update_dda(dda, game, x);
		get_line_properties(dda, game);
		get_wallhitpoint(dda, &wallhitpoint);
		draw_wall_stripe(dda, game, wallhitpoint, x);
		z_buffer[x] = dda->corr_dist;
		x++;
	}
	render_sprites(game, dda, z_buffer);
	free (dda);
	dda = NULL;
	render_minimap(game);
}

static void	init_images(t_game *game)
{
	game->image = mlx_new_image(game->mlx, MAX_SCREEN_WIDTH, MAX_SCREEN_HEIGHT);
	if (!game->image)
		cleanup_and_exit(game, ERRNEWIMG, 0, 1);
	if (mlx_image_to_window(game->mlx, game->image, 0, 0) < 0)
		cleanup_and_exit(game, ERRIMG, 0, 1);
	game->minimapimage = mlx_new_image(game->mlx, 21 * TILE,
			21 * TILE);
	if (!game->minimapimage)
		cleanup_and_exit(game, ERRNEWIMG, 0, 1);
	if (mlx_image_to_window(game->mlx, game->minimapimage,
			0, 0) < 0)
		cleanup_and_exit(game, ERRIMG, 0, 1);
}

/*Here we make and store the textures and check whether they are squares.
If not, the window has already been opened, but it closes right away and an error
message is displayed. Otherwise, the starting view and minimap are rendered.*/
void	init_maps(t_game *game)
{
	int	i;

	i = 0;
	game->asset_paths[TEXTURE_COUNT - 1] = ft_strdup("assets/sprite.png");
	if (!game->asset_paths[TEXTURE_COUNT - 1])
		cleanup_and_exit(game, ERRMEM, 0, 0);
	while (i < TEXTURE_COUNT)
	{
		game->textures[i] = mlx_load_png(game->asset_paths[i]);
		if (!game->textures[i])
		{
			i--;
			while (i >= 0)
				mlx_delete_texture(game->textures[i--]);
			cleanup_and_exit(game, ERRPNG, 0, 0);
		}
		texture_square_checker(game, i);
		i++;
	}
	init_images(game);
	render_map(game);
}
