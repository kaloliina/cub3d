#include "../include/cub3D.h"

/*This function calculates the starting and end points of the portion of the minimap
that we render to be visible at the moment, focusing on the player.*/
int	find_minimap_edges(double coordinate, bool start, int max)
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

/*
--11--
This function "combines" the different rgb values together into one integer.
By shifting r 24 bits left, g 16 bits left, g 8 bits left and leaving a (alpha channel)
where it is.
*/
int	get_color(int *rgb)
{
	int	color;
	int	a;

	a = 255;
	color = rgb[0] << 24 | rgb[1] << 16 | rgb[2] << 8 | a;
	return (color);
}

//The number 20 here represents the length of the line
void	draw_line(t_game *game, double begin_x, double begin_y)
{
	double	end_x;
	double	end_y;
	double	delta_x;
	double	delta_y;
	int		pixels;

	end_x = begin_x + game->player.dir_x * 20;
	end_y = begin_y + game->player.dir_y * 20;
	delta_x = end_x - begin_x;
	delta_y = end_y - begin_y;
	pixels = sqrt((delta_x * delta_x) + (delta_y * delta_y));
	delta_x /= pixels;
	delta_y /= pixels;
	while (pixels)
	{
		mlx_put_pixel(game->minimapimage, begin_x, begin_y, get_color(PLAYER_COLOR));
		begin_x += delta_x;
		begin_y += delta_y;
		--pixels;
	}
}

void	draw_pixels(t_game *game, enum e_assets type, int x, int y)
{
	int	y_tile;
	int	x_tile;
	int	original_x;

	y_tile = 0;
	original_x = x;
	while (y_tile < TILE)
	{
		x_tile = 0;
		x = original_x;
		while (x_tile < TILE)
		{
			if (type == BASE)
				mlx_put_pixel(game->minimapimage, x, y, get_color(BASE_COLOR));
			if (type == WALL)
				mlx_put_pixel(game->minimapimage, x, y, get_color(WALL_COLOR));
			if (type == PLAYER && x_tile > 5 && x_tile < 14 && y_tile > 5 && y_tile < 14)
				mlx_put_pixel(game->minimapimage, x, y, get_color(PLAYER_COLOR));
			x_tile++;
			x++;
		}
		y++;
		y_tile++;
	}
}

/*This function checks whether the texture being created is a square.*/
void	texture_square_checker(t_game *game, int i)
{
	if (game->textures[i]->width != game->textures[i]->height)
	{
		i--;
		while (i >= 0)
			mlx_delete_texture(game->textures[i--]);
		cleanup_and_exit(game, ERRTEXT, 0, 0);
	}
}
