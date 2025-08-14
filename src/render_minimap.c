#include "../include/cub3D.h"

/*This function calculates the starting and end points of the portion
of the minimap that we render to be visible at the moment, focusing on
the player.*/
static int	minimap_x_edge(double player, bool start, int max, int start_point)
{
	int	point;

	if (start)
	{
		point = player - 10;
		if (point < 0)
			point = 0;
	}
	else
	{
		point = player + 10;
		if (point - start_point < 20)
			point += (20 - (point - start_point));
		if (point >= max)
			point = max;
	}
	return (point);
}

static int	minimap_y_edge(double player, int start, int max, int start_point)
{
	int	point;

	if (start)
	{
		point = player - 6;
		if (point < 0)
			point = 0;
	}
	else
	{
		point = player + 6;
		if (point - start_point < 12)
			point += (12 - (point - start_point));
		if (point >= max)
			point = max;
	}
	return (point);
}

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
		x_end = minimap_x_edge(game->player.x, false, ft_strlen(game->map[y]), x_start);
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
	int	y_start;
	int	y_end;

	x_start = minimap_x_edge(game->player.x, true, game->width, 0);
	y_start = minimap_y_edge(game->player.y, true, game->height, 0);
	y_end = minimap_y_edge(game->player.y, false, game->height, y_start);
	draw_minimap_base(game, x_start, y_start, y_end);
	draw_pixels(game, PLAYER, (game->player.x - 0.5 - x_start) * TILE,
		(game->player.y - 0.5 - y_start) * TILE);
	draw_line(game, (game->player.x - x_start) * TILE,
		(game->player.y - y_start) * TILE);
}
