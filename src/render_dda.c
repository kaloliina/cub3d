#include "cub3D.h"

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
	dda->deltadist_x = 0.0;
	dda->deltadist_y = 0.0;
	dda->camera_x = 0.0;
	dda->perpwalldist = 0.0;
	dda->lineheight = 0.0;
	dda->map_x = dda->pos_x;
	dda->map_y = dda->pos_y;
	dda->step_x = 0;
	dda->step_y = 0;
	dda->hit = 0;
	dda->side = 0;
	dda->drawstart = 0;
	dda->drawend = 0;
}

int	get_ray_step(double raydir)
{
	if (raydir < 0) //if ray is moving left (raydir_x) or up (y)
		return (-1); //we take one step to left/up, decrease x/y
	else //if ray is moving right/down (if raydir == 0, this step will be unused so it doesn't matter)
		return (1);
}

double	get_sidedist(double raydir, double pos, int map, double deltadist)
{
	if (raydir < 0)
		return ((pos - map) * deltadist); //player position - left/up edge of current square
	else
		return ((map + 1.0 - pos) * deltadist); //right/down edge of curr square + player
}

void	update_dda(t_dda *dda, t_game *game, int x)
{
	dda->map_x = (int)dda->pos_x;	//let's start from player position
	dda->map_y = (int)dda->pos_y;
	dda->step_x = 0;
	dda->step_y = 0;
	dda->hit = 0;	//indicates whether we have hit a wall on the map while calculating ray length
	dda->side = 0;	//indicates whether we hit a NS or a EW wall (0 if vertical ie. EW, x side)
	//camera_x is the x-coordinate on the camera plane - -1 on the left side, 0 in the middle and 1 on the right side of screen
	dda->camera_x = 2 * x / (double)MAX_SCREEN_WIDTH - 1;
	dda->raydir_x = dda->dir_x + *(game->plane_x) * dda->camera_x;	//position vector + specific part of camera plane (see lodev pic)
	dda->raydir_y = dda->dir_y + *(game->plane_y) * dda->camera_x;
	if (dda->raydir_x == 0)
		dda->deltadist_x = 1e30;
	else
		dda->deltadist_x = fabs(1 / dda->raydir_x);	//this is the "hypotenuse" (see lodev pic)
	if (dda->raydir_y == 0)
		dda->deltadist_y = 1e30;
	else
		dda->deltadist_y = fabs(1 / dda->raydir_y);	//if raydir is 0, let's put a big value to not divide by 0 later
	dda->step_x = get_ray_step(dda->raydir_x);
	dda->step_y = get_ray_step(dda->raydir_y);
	dda->sidedist_x = get_sidedist(dda->raydir_x, dda->pos_x, dda->map_x, dda->deltadist_x);
	dda->sidedist_y = get_sidedist(dda->raydir_y, dda->pos_y, dda->map_y, dda->deltadist_y);
}

void	get_line_properties(t_dda *dda, t_game *game)
{
	while (dda->hit == 0)	//we keep going with the ray until we hit a wall
	{
		if (dda->sidedist_x < dda->sidedist_y)	//if the way to next x side is shorter than to next y side
		{
			dda->sidedist_x += dda->deltadist_x;	//we move in x direction to the next x side so sidedist_x is now the whole way so far
			dda->map_x += dda->step_x;	//we also move in map squares
			dda->side = 0;
		}
		else
		{
			dda->sidedist_y += dda->deltadist_y;
			dda->map_y += dda->step_y;
			dda->side = 1;
		}
		if (game->map[dda->map_y][dda->map_x] == '1')
			dda->hit = 1;
	}
	if (dda->side == 0)	//if we hit a vertical wall so x side
		dda->perpwalldist = (dda->sidedist_x - dda->deltadist_x);
	else	//if we hit horizontal wall, y side
		dda->perpwalldist = (dda->sidedist_y - dda->deltadist_y);
	dda->lineheight = MAX_SCREEN_HEIGHT / dda->perpwalldist;
	dda->drawstart = (MAX_SCREEN_HEIGHT / 2) - (dda->lineheight / 2); //below the middle of the screen, we want to center it so half is below and half above middle
	if (dda->drawstart < 0)
		dda->drawstart = 0;
	dda->drawend = (MAX_SCREEN_HEIGHT / 2) + (dda->lineheight / 2); //above the middle of the screen
	if (dda->drawend >= MAX_SCREEN_HEIGHT)
		dda->drawend = MAX_SCREEN_HEIGHT - 1;
}
