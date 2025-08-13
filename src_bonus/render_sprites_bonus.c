#include "../include/cub3D_bonus.h"

/*Tex y helper tells us how far vertically inside the sprite this pixel is.
We use it to calculate the y position in the texture of this pixel.*/
static void	draw_sprite_helper(t_render_sprite *data, t_game *game,
	t_dda *dda, int tex_x)
{
	int	tex_y_helper;
	int	tex_y;
	int	index;
	int	color;

	while (data->y_start < data->y_end)
	{
		tex_y_helper = (data->y_start - (int)(436 / data->sprite_depth))
			* 256 - MAX_SCREEN_HEIGHT * 128 + data->sprite_size * 128;
		tex_y = ((tex_y_helper * 218) / data->sprite_size) / 256;
		index = 4 * (218 * tex_y + tex_x);
		color = get_curr_color(game, SPRITE, index, dda);
		if (game->textures[SPRITE]->pixels[index + 3] != 0)
			mlx_put_pixel(game->image, data->x_start,
				data->y_start, color);
		data->y_start++;
	}
}

/*We will only draw the vertical line in question, if
- it's in front of the camera plane (depth is positive)
- it's on the screen (x_start x between 0 and screen width)
- it is not behind a wall (corr_dist is bigger than sprite's dist).
Tex x converts the current screen column to corresponding x coordinate
on sprite texture. We multiply and divide with 256 to maintain int
precision. 32 is set manually as the tex size (width and height).*/
static void	draw_sprite(t_render_sprite *data, t_game *game, t_dda *dda,
	double *z_buffer)
{
	int		tex_x;
	bool	behind_wall;

	behind_wall = set_behind_wall(data, z_buffer);
	while (data->x_start < data->x_end)
	{
		data->y_start = find_drawedges(data, 1, 0);
		tex_x = (int)(256
				* (data->x_start - (-data->sprite_size / 2
						+ data->sprite_screen_x))
				* 218 / data->sprite_size) / 256;
		if (data->sprite_depth > 0 && data->x_start > 0
			&& data->x_start < MAX_SCREEN_WIDTH
			&& !behind_wall)
			draw_sprite_helper(data, game, dda, tex_x);
		data->x_start++;
	}
}

/*We need to calculate the x and y location of the sprite in relation to
the player.
Inverse of determinant is used to correctly transform the sprite's location
in the map("world") to camera space. Determinant means the area limited by
two direction vectors (dir and plane).
"Corrected" x and sprite depth tell us how far in x and y is the sprite from
player's view center.
Sprite screen x tells us exactly where on the screen the sprite's
vertical center will be drawn.*/
static void	set_sprite_values(t_render_sprite *data, t_game *game,
	t_dda *dda, int i)
{
	data->sprite_x = game->sprites[i].x - dda->pos_x;
	data->sprite_y = game->sprites[i].y - dda->pos_y;
	data->inv_det = 1.0 / (game->plane_x
			* dda->dir_y - dda->dir_x * game->plane_y);
	data->corr_x = data->inv_det * (dda->dir_y * data->sprite_x - dda->dir_x
			* data->sprite_y);
	data->sprite_depth = data->inv_det * (-game->plane_y
			* data->sprite_x + game->plane_x * data->sprite_y);
	data->sprite_screen_x = (int)((MAX_SCREEN_WIDTH / 2)
			* (1 + data->corr_x / data->sprite_depth));
	data->sprite_size = abs((int)(MAX_SCREEN_HEIGHT / data->sprite_depth))
		* 0.5;
	data->x_start = find_drawedges(data, 0, 0);
	data->x_end = find_drawedges(data, 2, MAX_SCREEN_WIDTH);
	data->y_end = find_drawedges(data, 3, MAX_SCREEN_HEIGHT);
}

void	render_sprites(t_game *game, t_dda *dda, double	*z_buffer)
{
	int				i;
	t_render_sprite	data;

	i = 0;
	sort_sprites(game, dda, 0);
	while (i < game->sprite_amt)
	{
		set_sprite_values(&data, game, dda, i);
		draw_sprite(&data, game, dda, z_buffer);
		i++;
	}
}
