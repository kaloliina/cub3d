#include "cub3D.h"

double	find_sprite_dist(t_game *game, t_dda *dda, int i)
{
	return ((dda->pos_x - game->sprites[i].x)
		* (dda->pos_x - game->sprites[i].x)
		+ (dda->pos_y - game->sprites[i].y)
		* (dda->pos_y - game->sprites[i].y));
}

void	sort_sprites(t_game *game, t_dda *dda, int i)
{
	int			j;
	t_sprite	tmp;

	j = 0;
	while (i < game->sprite_amt)
	{
		game->sprites[i].dist = find_sprite_dist(game, dda, i);
		i++;
	}
	i = 0;
	while (i < game->sprite_amt)
	{
		while (j < game->sprite_amt)
		{
			if (game->sprites[i].dist > game->sprites[j].dist)
			{
				tmp = game->sprites[i];
				game->sprites[i] = game->sprites[j];
				game->sprites[j] = tmp;
			}
			j++;
		}
		i++;
	}
}

int	find_drawedges(t_render_sprite *data, int flag, int max)
{
	int	value;

	if (flag < 2)
	{
		if (flag == 0)
			value = -data->sprite_size / 2 + data->sprite_screen_x;
		else
			value = -data->sprite_size / 2 + MAX_SCREEN_HEIGHT / 2;
		if (value < 0)
			value = 0;
	}
	else
	{
		if (flag == 2)
			value = data->sprite_size / 2 + data->sprite_screen_x;
		else
			value = data->sprite_size / 2 + MAX_SCREEN_HEIGHT / 2;
		if (value >= max)
			value = max - 1;
	}
	return (value);
}
