#include "../include/cub3D_bonus.h"

/*This function calculates the starting and end points of the portion
of the minimap that we render to be visible at the moment, focusing on
the player.*/
static int	minimap_edge(double coordinate, int start, int max, int start_point)
{
	int	point;

	if (start == 0 || start == 2)
	{
		point = coordinate - 10;
		if (point < 0)
			point = 0;
		printf("coord %f point %d max %d\n", coordinate, point, max);
		if (start == 0 && (point + 20) > max)
			point -= point + 20 - max;
	}
	else
	{
		point = coordinate + 10;
		if (start == 1 && point - start_point < 20)
			point += (20 - (point - start_point));
		if (start == 1 && point - start_point > 20)
			point -= (point - start_point) - 20;
		if (start == 3 && point - start_point < 12)
			point += (12 - (point - start_point));
		if (start == 3 && point - start_point > 12)
			point -= (point - start_point) - 12;
		printf("point %d max %d\n", point, max);
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
		x_end = minimap_edge(game->player.x, 1, ft_strlen(game->map[y]), x_start);
		printf("x start %d end %d y start %d end %d\n", x_start, x_end, y_start, y_end);
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

void	render_minimap(t_game *game)
{
	int	x_start;
	int	y_start;
	int	y_end;

	printf("player x %f y %f\n", game->player.x, game->player.y);
	x_start = minimap_edge(game->player.x, 0, game->width, 0);
	y_start = minimap_edge(game->player.y, 2, game->height, 0);
	y_end = minimap_edge(game->player.y, 3, game->height, y_start);
	if (y_end - game->player.y < 2 && (y_start + 2 + 12 < game->height)) //this last condition doesnt work!! check simple map bottom
	{
		y_start += 2;
		y_end = minimap_edge(game->player.y, 3, game->height, y_start);
	}
	draw_minimap_base(game, x_start, y_start, y_end);
	draw_pixels(game, PLAYER, (game->player.x - 0.5 - x_start) * TILE,
		(game->player.y - 0.5 - y_start) * TILE);
	draw_line(game, (game->player.x - x_start) * TILE,
		(game->player.y - y_start) * TILE);
}
