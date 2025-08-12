#include "../include/cub3D.h"

/*
This function "combines" the different rgb values together into one integer.
By shifting r 24 bits left, g 16 bits left, g 8 bits left and leaving a
(alpha channel) where it is.
*/
int	get_color(int *rgb)
{
	int	color;
	int	a;

	a = 255;
	color = rgb[0] << 24 | rgb[1] << 16 | rgb[2] << 8 | a;
	return (color);
}

/*Pixels of the texture are stored in a 1D array. Horizontal sides are made
darker by shifting the bits to "divide by 2" ie. removing the last digit,
and then setting the first bit of every bite to zero by calling AND with
0111 1111 0111 1111 0111 1111.*/
int	get_curr_color(t_game *game, enum e_textures type, int index,
	t_dda *dda)
{
	int	color[3];
	int	i;

	i = 0;
	while (i < 3)
	{
		color[i] = game->textures[type]->pixels[index + i];
		if (type != SPRITE && dda->hor_side)
			color[i] = (color[i] >> 1) & 8355711;
		i++;
	}
	return (get_color(color));
}

/*This function is used for drawing the line in minimap. In order to do so,
we get end x and y coordinates, the delta distance between start and end.
We use the "Pythagoran lause" to find the straight line and we begin filling
it one by one*/
void	draw_line(t_game *game, double start_x, double start_y)
{
	double	end_x;
	double	end_y;
	double	delta_x;
	double	delta_y;
	int		pixels;

	end_x = start_x + game->player.dir_x * 20;
	end_y = start_y + game->player.dir_y * 20;
	delta_x = end_x - start_x;
	delta_y = end_y - start_y;
	pixels = sqrt((delta_x * delta_x) + (delta_y * delta_y));
	delta_x /= pixels;
	delta_y /= pixels;
	while (pixels)
	{
		mlx_put_pixel(game->minimapimage, start_x, start_y,
			get_color(PLAYER_CLR));
		start_x += delta_x;
		start_y += delta_y;
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
				mlx_put_pixel(game->minimapimage, x, y, get_color(BASE_CLR));
			if (type == WALL)
				mlx_put_pixel(game->minimapimage, x, y, get_color(WALL_CLR));
			if (type == PLAYER && x_tile > 5 && x_tile < 14 && y_tile > 5
				&& y_tile < 14)
				mlx_put_pixel(game->minimapimage, x, y, get_color(PLAYER_CLR));
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
