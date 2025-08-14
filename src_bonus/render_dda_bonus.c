/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_dda_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 12:02:40 by sojala            #+#    #+#             */
/*   Updated: 2025/08/14 16:01:02 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D_bonus.h"

/*In case of raydir == 0, that step will not be used.*/
static int	get_ray_step(double raydir)
{
	if (raydir < 0)
		return (-1);
	else
		return (1);
}

/*Calculates the distance from starting position to next x/y side.
If raydir < 0, sidedist is player position - left/up edge of current
square. Else, it's player position + right/down edge of curr square.*/
static double	get_sidedist(double raydir, double pos, int map,
	double squaredist)
{
	if (raydir < 0)
		return ((pos - map) * squaredist);
	else
		return ((map + 1.0 - pos) * squaredist);
}

/*- Hor_side indicates whether we hit a NS or a EW wall
(0 if vertical = EW = "x" side).
- Camera_x: the x-coordinate on the camera plane (-1 on the left,
0 in the middle, and 1 on the right side of screen).
- Squaredist: the distance the ray has to travel to get from one
x or y edge to the next. In case of raydir == 0, we give it a
very large value to prevent moving in that direction and dividing
with 0.*/
void	update_dda(t_dda *dda, t_game *game, int x)
{
	dda->map_x = (int)dda->pos_x;
	dda->map_y = (int)dda->pos_y;
	dda->hit = 0;
	dda->hor_side = 0;
	dda->camera_x = 2 * x / (double)MAX_SCREEN_WIDTH - 1;
	dda->raydir_x = dda->dir_x + game->plane_x * dda->camera_x;
	dda->raydir_y = dda->dir_y + game->plane_y * dda->camera_x;
	if (dda->raydir_x == 0)
		dda->squaredist_x = 1e30;
	else
		dda->squaredist_x = fabs(1 / dda->raydir_x);
	if (dda->raydir_y == 0)
		dda->squaredist_y = 1e30;
	else
		dda->squaredist_y = fabs(1 / dda->raydir_y);
	dda->step_x = get_ray_step(dda->raydir_x);
	dda->step_y = get_ray_step(dda->raydir_y);
	dda->sidedist_x = get_sidedist(dda->raydir_x, dda->pos_x, dda->map_x,
			dda->squaredist_x);
	dda->sidedist_y = get_sidedist(dda->raydir_y, dda->pos_y, dda->map_y,
			dda->squaredist_y);
}

/*Travels along the ray to find the point where it hits a wall.
If the distance to next x-side is shorter than to the next y-side, we move
in x direction to the next x_side, so sidedist_x is now the whole way so far.
Simultaneously, we move in map squares with step_x.*/
static void	find_raydist(t_dda *dda, t_game *game)
{
	while (dda->hit == 0)
	{
		if (dda->sidedist_x < dda->sidedist_y)
		{
			dda->sidedist_x += dda->squaredist_x;
			dda->map_x += dda->step_x;
			dda->hor_side = 0;
		}
		else
		{
			dda->sidedist_y += dda->squaredist_y;
			dda->map_y += dda->step_y;
			dda->hor_side = 1;
		}
		if (game->map[dda->map_y][dda->map_x] == '1')
			dda->hit = 1;
	}
}

/*Corr_dist gives us the perpendicular distance to wall
to avoid fisheye effect.*/
void	get_line_properties(t_dda *dda, t_game *game)
{
	find_raydist(dda, game);
	if (dda->hor_side == 0)
		dda->corr_dist = (dda->sidedist_x - dda->squaredist_x);
	else
		dda->corr_dist = (dda->sidedist_y - dda->squaredist_y);
	dda->lineheight = (int)(MAX_SCREEN_HEIGHT / dda->corr_dist);
	dda->y_start = (MAX_SCREEN_HEIGHT / 2) - (dda->lineheight / 2);
	if (dda->y_start < 0)
		dda->y_start = 0;
	dda->y_end = (MAX_SCREEN_HEIGHT / 2) + (dda->lineheight / 2);
	if (dda->y_end >= MAX_SCREEN_HEIGHT)
		dda->y_end = MAX_SCREEN_HEIGHT - 1;
}
