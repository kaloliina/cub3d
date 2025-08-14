/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_textures.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 12:02:06 by sojala            #+#    #+#             */
/*   Updated: 2025/08/14 16:07:48 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D.h"

/*Calculates the exact point inside a square at which we hit a wall.
Subtracting by floor(temp) leaves us with the decimal part of the result.*/
void	get_wallhitpoint(t_dda *dda, double *wallhitpoint)
{
	double	temp;

	if (dda->hor_side == 0)
		temp = dda->pos_y + dda->corr_dist * dda->raydir_y;
	else
		temp = dda->pos_x + dda->corr_dist * dda->raydir_x;
	*wallhitpoint = temp - floor(temp);
}

/*Finds out the direction of the wall we are drawing.*/
static enum e_textures	get_tex_type(t_dda *dda)
{
	if (dda->hor_side == 0 && dda->raydir_x > 0)
		return (EAST);
	else if (dda->hor_side == 0 && dda->raydir_x < 0)
		return (WEST);
	else if (dda->hor_side == 1 && dda->raydir_y > 0)
		return (SOUTH);
	else
		return (NORTH);
}

/*In case of east/west wall and ray coming from west, or north/south wall
and ray comes from south, the texture will be flipped.*/
static int	get_tex_x(double wallhitpoint, t_dda *dda, int tex_size)
{
	int	temp;

	temp = (int)(wallhitpoint * (double)tex_size);
	if ((dda->hor_side == 0 && dda->raydir_x > 0)
		|| (dda->hor_side == 1 && dda->raydir_y < 0))
		temp = tex_size - temp - 1;
	return (temp);
}

/*Calculates the correct pixel from the texture to draw it in the right
position on the screen.
Tex.y: we use modulo to ensure the value is always between 0 and tex_size
Index: we multiply by 4 because each pixel color is represented in 4 bytes*/
void	draw_wall_stripe(t_dda *dda, t_game *game, double wallhitpoint, int x)
{
	t_texture		tex;
	int				index;
	enum e_textures	type;

	type = get_tex_type(dda);
	tex.size = game->textures[type]->width;
	tex.x = get_tex_x(wallhitpoint, dda, tex.size);
	tex.step = 1.0 * tex.size / dda->lineheight;
	tex.pos = (dda->y_start - MAX_SCREEN_HEIGHT / 2 + dda->lineheight / 2)
		* tex.step;
	while (dda->y_start < dda->y_end)
	{
		tex.y = ((int)tex.pos % tex.size + tex.size) % tex.size;
		index = 4 * (tex.size * tex.y + tex.x);
		tex.pos += tex.step;
		mlx_put_pixel(game->image, x, dda->y_start,
			get_curr_color(game, type, index, dda));
		dda->y_start++;
	}
}
