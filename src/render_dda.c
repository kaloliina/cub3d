#include "cub3D.h"

/*This function calculates which way we move as we move along the ray to find the wall.
We calculate it each time for both x and y, but each round of the find_raydist loop
we take only one of these steps*/
static int	get_ray_step(double raydir)
{
	if (raydir < 0) //if ray is moving left (raydir_x) or up (y)
		return (-1); //we take one step to left/up, decrease x/y
	else //if ray is moving right/down (if raydir == 0, this step will be unused so it doesn't matter)
		return (1);
}

/*Sidedist indicates the distance from start position to next x/y-side*/
static double	get_sidedist(double raydir, double pos, int map, double squaredist)
{
	if (raydir < 0)
		return ((pos - map) * squaredist); //player position - left/up edge of current square
	else
		return ((map + 1.0 - pos) * squaredist); //right/down edge of curr square + player
}

/*This function refreshes and updates dda variable values each time we move forward to the right
on the screen (= increase x), as we draw each vertical wall stripe.*/
void	update_dda(t_dda *dda, t_game *game, int x)
{
	dda->map_x = (int)dda->pos_x;	//let's start from player position
	dda->map_y = (int)dda->pos_y;
	dda->step_x = 0;
	dda->step_y = 0;
	dda->hit = 0; //indicates whether we have hit a wall on the map while calculating ray length
	dda->hor_side = 0; //indicates whether we hit a NS or a EW wall (0 if vertical ie. EW, x side)
	//camera_x is the x-coordinate on the camera plane - -1 on the left side, 0 in the middle and 1 on the right side of screen
	dda->camera_x = 2 * x / (double)MAX_SCREEN_WIDTH - 1;
	dda->raydir_x = dda->dir_x + *game->plane_x * dda->camera_x; //position vector + specific part of camera plane (see lodev pic)
	dda->raydir_y = dda->dir_y + *game->plane_y * dda->camera_x;
	/*we give squaredist a very big value in case of a straight vertical or horizontal line, so that
	we avoid dividing by zero, and also after that we won't choose that direction when comparing sidedists.
	squaredist indicates how much the value of x changes when y changes by one, and vice versa.*/
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
	dda->sidedist_x = get_sidedist(dda->raydir_x, dda->pos_x, dda->map_x, dda->squaredist_x);
	dda->sidedist_y = get_sidedist(dda->raydir_y, dda->pos_y, dda->map_y, dda->squaredist_y);
}

/*In this function we travel along the ray to find the point where it hits a wall*/
static void	find_raydist(t_dda *dda, t_game *game)
{
	while (dda->hit == 0)
	{
		if (dda->sidedist_x < dda->sidedist_y) //if the way to next x side is shorter than to next y side
		{
			dda->sidedist_x += dda->squaredist_x; //we move in x direction to the next x side so sidedist_x is now the whole way so far
			dda->map_x += dda->step_x; //we also move in map squares
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

/*Here we first call find_raydist to get the length of the ray until a wall. Then we correct
that length to avoid fisheye effect. We then calculate the height of the current line, and
the y coordinates at which the line starts and ends.*/
void	get_line_properties(t_dda *dda, t_game *game)
{
	find_raydist(dda, game);
	if (dda->hor_side == 0) //if we hit a vertical wall so x side
		dda->corr_length = (dda->sidedist_x - dda->squaredist_x);
	else //if we hit horizontal wall, y side
		dda->corr_length = (dda->sidedist_y - dda->squaredist_y);
	dda->lineheight = MAX_SCREEN_HEIGHT / dda->corr_length;
	dda->drawstart = (MAX_SCREEN_HEIGHT / 2) - (dda->lineheight / 2); //below the middle of the screen, we want to center it so half is below and half above middle
	if (dda->drawstart < 0)
		dda->drawstart = 0;
	dda->drawend = (MAX_SCREEN_HEIGHT / 2) + (dda->lineheight / 2); //above the middle of the screen
	if (dda->drawend >= MAX_SCREEN_HEIGHT)
		dda->drawend = MAX_SCREEN_HEIGHT - 1;
}
