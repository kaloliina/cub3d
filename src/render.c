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

/*--10--
This function first draws the ceiling and floor, looping through the window width and height.
Then it draws the walls by using the raycasting mathematics (will be better explained soon...)
After we have filled the image, we put image to window.

I took out putting image to window in between steps and only do it in the end, is it ok?*/
void	render_map(t_game *game)
{
	int		x;
	int		y;
	t_dda	*dda;
	double	wallhitpoint; //the exact spot where wall was hit - can be x or y coordinate depending on side, but in texture always x

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

/*
--9--
void	render_minimap(t_game *game)
{
	int	x;
	int	x1;
	int	y;
	int	y1;
	int	p_x = game->player.x;
	int	p_y = game->player.y;

	int	x_start = game->player.x - 10;
	if (x_start < 0)
		x_start = 0;
	int	x_end = game->player.x + 10;
	if (x_end > game->width)
		x_end = game->width;
	int	y_start = game->player.y - 10;
	if (y_start < 0)
		y_start = 0;
	int	y_end = game->player.y + 10;
	if (y_end > game->height)
		y_end = game->height;

	y = y_start;
	y1 = 0;
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
	draw_pixels(game, PLAYER, (x_start + game->player.x - 0.5) * TILE, (y_start + game->player.y - 0.5) * TILE);
	draw_line(game, (x_start + game->player.x) * TILE, (y_start + game->player.y) * TILE);
	if (mlx_image_to_window(game->mlx, game->minimapimage,
			0, 0) < 0)
		cleanup_and_exit(game, ERRIMG, 0, 1);
}*/

void	render_minimap(t_game *game)
{
	int	x;
	int	y;

	y = 0;
	while (game->map[y])
	{
		x = 0;
		while (x < (int)ft_strlen(game->map[y]))
		{
			if (game->map[y][x] == '1')
				draw_pixels(game, WALL, x * TILE, y * TILE);
			else if (ft_strchr("0NSEW", game->map[y][x]))
				draw_pixels(game, BASE, x * TILE, y * TILE);
			x++;
		}
		y++;
	}
	draw_pixels(game, PLAYER, (game->player.x - 0.5) * TILE, (game->player.y - 0.5) * TILE);
	draw_line(game, game->player.x * TILE, game->player.y * TILE);
	if (mlx_image_to_window(game->mlx, game->minimapimage,
			0, 0) < 0)
		cleanup_and_exit(game, ERRIMG, 0, 1);
}

/*
--8--
Here we are creating images for the actual map and the minimap.
After this, we start rendering the maps.
*/
void	init_maps(t_game *game)
{
	game->image = mlx_new_image(game->mlx, MAX_SCREEN_WIDTH, MAX_SCREEN_HEIGHT);
	if (!game->image)
		cleanup_and_exit(game, ERRNEWIMG, 0, 1);
	game->minimapimage = mlx_new_image(game->mlx, game->width * TILE, game->height * TILE);
	if (!game->minimapimage)
		cleanup_and_exit(game, ERRNEWIMG, 0, 1);
	render_map(game);
	render_minimap(game);
}
