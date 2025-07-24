#include "../include/cub3D.h"

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

void	init_dda(t_dda *dda, t_game *game)
{
	dda->pos_x = game->player.x;
	dda->pos_y = game->player.y;
	dda->dir_x = game->player.dir_x;
	dda->dir_y = game->player.dir_y;
	dda->raydir_x = 0.0;
	dda->raydir_y = 0.0;
	dda->sidedist_x = 0.0;
	dda->sidedist_y = 0.0;
	dda->squaredist_x = 0.0;
	dda->squaredist_y = 0.0;
	dda->camera_x = 0.0;
	dda->corr_length = 0.0;
	dda->lineheight = 0.0;
	dda->map_x = dda->pos_x;
	dda->map_y = dda->pos_y;
	dda->step_x = 0;
	dda->step_y = 0;
	dda->hit = 0;
	dda->hor_side = 0;
	dda->drawstart = 0;
	dda->drawend = 0;
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
