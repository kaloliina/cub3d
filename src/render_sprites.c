#include "cub3D.h"

static void	draw_sprite_helper(t_render_sprite *data, t_game *game, t_dda *dda, int tex_x)
{
	int	d;
	int	tex_y;
	int	index;
	int	color;

	while (data->drawstart_y < data->drawend_y)
	{
		d = data->drawstart_y * 256 - MAX_SCREEN_HEIGHT
			* 128 + data->sprite_size * 128;
		tex_y = ((d * 32) / data->sprite_size) / 256;
		index = 4 * (32 * tex_y + tex_x);
		color = get_curr_color(game, SPRITE, index, dda);
		if (color != 255)
			mlx_put_pixel(game->image, data->drawstart_x,
				data->drawstart_y, get_curr_color(game,
					SPRITE, index, dda));
		data->drawstart_y++;
	}
}

static void	draw_sprite(t_render_sprite *data, t_game *game, t_dda *dda,
	double *z_buffer)
{
	int	tex_x;

	while (data->drawstart_x < data->drawend_x)
	{
		data->drawstart_y = find_drawedges(data, 1, 0);
		tex_x = (int)(256
				* (data->drawstart_x - (-data->sprite_size / 2 + data->sprite_screen_x))
				* 32 / data->sprite_size) / 256;
		if (data->transf_y > 0 && data->drawstart_x > 0
			&& data->drawstart_x < MAX_SCREEN_WIDTH
			&& data->transf_y < z_buffer[data->drawstart_x] + 0.0001)
			draw_sprite_helper(data, game, dda, tex_x);
		data->drawstart_x++;
	}
}

static void	set_sprite_values(t_render_sprite *data, t_game *game,
	t_dda *dda, int i)
{
	data->sprite_x = game->sprites[i].x - dda->pos_x;
	data->sprite_y = game->sprites[i].y - dda->pos_y;
	data->inv_det = 1.0 / ((*game->plane_x)
			* dda->dir_y - dda->dir_x * (*game->plane_y));
	data->transf_x = data->inv_det * (dda->dir_y * data->sprite_x - dda->dir_x
			* data->sprite_y);
	data->transf_y = data->inv_det * (-(*game->plane_y)
			* data->sprite_x + (*game->plane_x) * data->sprite_y);
	data->sprite_screen_x = (int)((MAX_SCREEN_WIDTH / 2)
			* (1 + data->transf_x / data->transf_y));
	data->sprite_size = abs((int)(MAX_SCREEN_HEIGHT / data->transf_y));
	data->drawend_y = find_drawedges(data, 3, MAX_SCREEN_HEIGHT);
	data->drawstart_x = find_drawedges(data, 0, 0);
	data->drawend_x = find_drawedges(data, 2, MAX_SCREEN_WIDTH);
}

void	render_sprites(t_game *game, t_dda *dda, double	*z_buffer)
{
	int				i;
	int				drawstart_x;
	int				drawend_x;
	int				drawend_y;
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
