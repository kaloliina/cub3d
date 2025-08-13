#include "../include/cub3D_bonus.h"

static void	draw_minimap_base(t_game *game, int x_start, int y_start, int y_end)
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
			else if (ft_strchr("0NSEWH", game->map[y][x]))
				draw_pixels(game, BASE, x1 * TILE, y1 * TILE);
			x1++;
			x++;
		}
		y1++;
		y++;
	}
}

/*This function calculates the starting and end points of the portion
of the minimap that we render to be visible at the moment, focusing on
the player.*/
int	minimap_edge(double coordinate, bool start, int max)
{
	int	point;

	if (start)
	{
		point = coordinate - 10;
		if (point < 0)
			point = 0;
	}
	else
	{
		point = coordinate + 10;
		if (point >= max)
			point = max;
	}
	return (point);
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
