#include "cub3D.h"

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

void	render_map(t_game *game)
{
	int		x;
	int		y;
	t_dda	*dda;
	double	wallhitpoint;

	draw_ceiling_floor(game);
	dda = malloc(sizeof(t_dda));
	if (!dda)
		cleanup_and_exit(game, ERRMEM, 0, 1);
	init_dda(dda, game);
	x = 0;
	while (x < MAX_SCREEN_WIDTH) //loop through the screen, drawing the walls one vertical stripe at a time
	{
		update_dda(dda, game, x);
		get_line_properties(dda, game); //move along the ray to find a wall
		get_wallhitpoint(dda, &wallhitpoint); //get the exact wall hitting point of the ray
		draw_wall_stripe(dda, game, wallhitpoint, x); //draw the stripe pixel by pixel
		x++;
	}
	free (dda);
	mlx_image_to_window(game->mlx, game->image, 0, 0);
}

void	draw_minimap_base(t_game *game, int x_start, int y_start, int x_end)
{
	int	y;
	int	y1;
	int	x;
	int	x1;
	int	y_end;

	y = y_start;
	y1 = 0;
	y_end = find_minimap_edges(game->player.y, false, game->height);
	while (y < y_end)
	{
		x = x_start;
		x1 = 0;
		while (x < x_end)
		{
			if (game->map[y][x] == '1')
				draw_pixels(game, WALL, x1 * TILE, y1 * TILE);
			else if (ft_strchr("0NSEW", game->map[y][x]))
				draw_pixels(game, BASE, x1 * TILE, y1 * TILE);
			x1++;
			x++;
		}
		y1++;
		y++;
	}
}

void	render_minimap(t_game *game)
{
	int	x_start;
	int	x_end;
	int	y_start;

	x_start = find_minimap_edges(game->player.x, true, 0);
	x_end = find_minimap_edges(game->player.x, false, game->width);
	y_start = find_minimap_edges(game->player.y, true, 0);
	draw_minimap_base(game, x_start, y_start, x_end);
	draw_pixels(game, PLAYER, (game->player.x - 0.5 - x_start) * TILE, (game->player.y - 0.5 - y_start) * TILE);
	draw_line(game, (game->player.x - x_start) * TILE, (game->player.y - y_start) * TILE);
	if (mlx_image_to_window(game->mlx, game->minimapimage,
			0, 0) < 0)
		cleanup_and_exit(game, ERRIMG, 0, 1);
}

/*
We have to decide whether we will handle assets of various sizes (current stage)
and whether we  require them to be squares (atm we handle also non-square assets).
The math is easy to change to only-square if we want - does the raycasting work as
intended now if they are not squares?*/
void	init_maps(t_game *game)
{
	int	i;

	i = 0;
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
		i++;
	}
	game->image = mlx_new_image(game->mlx, MAX_SCREEN_WIDTH, MAX_SCREEN_HEIGHT);
	if (!game->image)
		cleanup_and_exit(game, ERRNEWIMG, 0, 1);
	game->minimapimage = mlx_new_image(game->mlx, game->width * TILE, game->height * TILE);
	if (!game->minimapimage)
		cleanup_and_exit(game, ERRNEWIMG, 0, 1);
	render_map(game);
	render_minimap(game);
}
