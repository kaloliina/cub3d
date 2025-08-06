#include "../include/cub3D.h"

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
	int		y;
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
		z_buffer[x] = dda->corr_length;
		x++;
	}
	render_sprites(game, dda, z_buffer);
	free (dda);
	dda = NULL;
	mlx_image_to_window(game->mlx, game->image, 0, 0);
}

void	draw_minimap_base(t_game *game, int x_start, int y_start, int y_end)
{
	int	y;
	int	y1;
	int	x;
	int	x1;
	int	x_end;

	y = y_start;
	y1 = 0;
	while (y < y_end)
	{
		x = x_start;
		x_end = minimap_edge(game->player.x, false, ft_strlen(game->map[y]));
		x1 = 0;
		while (x < x_end)
		{
			if (game->map[y][x] == '1')
				draw_pixels(game, WALL, x1 * TILE, y1 * TILE);
			else if (ft_strchr("0NSEWT", game->map[y][x]))
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
	int	y_start;
	int	y_end;

	x_start = minimap_edge(game->player.x, true, 0);
	y_start = minimap_edge(game->player.y, true, 0);
	y_end = minimap_edge(game->player.x, false, game->height);
	draw_minimap_base(game, x_start, y_start, y_end);
	draw_pixels(game, PLAYER, (game->player.x - 0.5 - x_start) * TILE,
		(game->player.y - 0.5 - y_start) * TILE);
	draw_line(game, (game->player.x - x_start) * TILE,
		(game->player.y - y_start) * TILE);
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
	game->asset_paths[TEXTURE_COUNT - 1] = ft_strdup("assets/sprite_test.png");
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
	game->image = mlx_new_image(game->mlx, MAX_SCREEN_WIDTH, MAX_SCREEN_HEIGHT);
	if (!game->image)
		cleanup_and_exit(game, ERRNEWIMG, 0, 1);
	game->minimapimage = mlx_new_image(game->mlx, game->width * TILE,
		game->height * TILE);
	if (!game->minimapimage)
		cleanup_and_exit(game, ERRNEWIMG, 0, 1);
	render_map(game);
	render_minimap(game);
}
