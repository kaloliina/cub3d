/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_sprites_utils_bonus.c                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 12:02:57 by sojala            #+#    #+#             */
/*   Updated: 2025/08/14 17:24:47 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D_bonus.h"

/*This distance is only used to sort the sprites, so to save some
time and energy, the square root will not be calculated
in this Pythagorean formula*/
static double	find_sprite_dist(t_game *game, t_dda *dda, int i)
{
	return ((dda->pos_x - game->sprites[i].x)
		* (dda->pos_x - game->sprites[i].x)
		+ (dda->pos_y - game->sprites[i].y)
		* (dda->pos_y - game->sprites[i].y));
}

/*Sorts the sprites so they will be drawn from furthest to
closest, calculating distance from the player.*/
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
			if (game->sprites[i].dist < game->sprites[j].dist)
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

/*Calculates the starting and ending points of the line
that draws the sprite.

We scale the y start and end points with the size of sprite texture,
because we draw it as half of its original size, and we don't want
it to float but to stand on ground.*/
int	find_drawedges(t_render_sprite *data, int flag, int max)
{
	int	value;
	int	scaler;

	scaler = (int)(218 / data->sprite_depth);
	if (flag < 2)
	{
		if (flag == 0)
			value = -data->sprite_size / 2 + data->sprite_screen_x;
		else
			value = (-data->sprite_size) / 2 + MAX_SCREEN_HEIGHT / 2 + scaler;
		if (value < 0)
			value = 0;
	}
	else
	{
		if (flag == 2)
			value = data->sprite_size / 2 + data->sprite_screen_x;
		else
			value = data->sprite_size / 2 + MAX_SCREEN_HEIGHT / 2 + scaler;
		if (value >= max)
			value = max - 1;
	}
	return (value);
}
