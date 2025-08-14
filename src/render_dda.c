/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_dda.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 12:01:52 by sojala            #+#    #+#             */
/*   Updated: 2025/08/14 12:01:53 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D.h"

/*This function calculates which way we move as we move along the ray to find
the wall. We calculate it each time for both x and y, but each round of the
find_raydist loop we take only one of these steps.
If ray is moving left (raydir_x) or up (raydir_y), we take one step left/up,
decreasing x/y, and if ray is moving right/down, we step that way.
If raydir == 0, this step will be unused.*/
static int	get_ray_step(double raydir)
{
	if (raydir < 0)
		return (-1);
	else
		return (1);
}

/*Sidedist indicates the distance from start position to next x/y-side.
If raydir is negative, the distance will be player position - left/up edge
of current square. Else, it will be right/down edge of curr square
+ player position.*/
static double	get_sidedist(double raydir, double pos, int map,
	double squaredist)
{
	if (raydir < 0)
		return ((pos - map) * squaredist);
	else
		return ((map + 1.0 - pos) * squaredist);
}

/*This function refreshes and updates dda variable values each time we
move forward to the right on the screen (= increase x), as we draw each
vertical wall stripe.
Hit tracks whether we have hit a wall while calculating ray length.
Hor_side indicates whether we hit a NS or a EW wall
(0 if vertical = EW = "x" side).
Camera_x is the x-coordinate on the camera plane (-1 on the left, 0 in
the middle, and 1 on the right side of screen).
In case of a straight vertical or horizontal line, we give squaredist a
very big value, so we won't divide with zero, and after that we won't
choose that direction when comparing sidedists.
Squaredist indicates the distance the ray has to travel to get from one
x or y edge to the next.*/
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

/*Here we travel along the ray to find the point where it hits a wall.
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

/*Here we first call find_raydist to get the length of the ray until a wall.
Then we correct that length to avoid fisheye effect. We then calculate the
height of the current line, and the y coordinates at which the line starts
and ends. We want to center the line, so half is below and half above middle
of the screen (y-axis).*/
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
