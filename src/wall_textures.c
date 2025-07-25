#include "../include/cub3D.h"

/*Here we calculate the exact point inside a square at which we hit a wall.*/
void	get_wallhitpoint(t_dda *dda, double *wallhitpoint)
{
	double	temp;

	if (dda->hor_side == 0)//if we hit vertical wall
		temp = dda->pos_y + dda->corr_length * dda->raydir_y;
	else
		temp = dda->pos_x + dda->corr_length * dda->raydir_x;
	*wallhitpoint = temp - floor(temp);//floor means taking the largest int value that is not greater than wallhitpoint
}

/*This function finds out the direction of the wall we hit (= are drawing).*/
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

/*Pixels of the texture are stored in a 1D array.*/
static int	get_curr_color(t_game *game, enum e_textures type, int index, t_dda *dda)
{
	int	color[3];
	int	i;

	i = 0;
	while (i < 3)
	{
		color[i] = game->textures[type]->pixels[index + i];
		if (dda->hor_side)	//let's make horizontal sides darker
			color[i] = (color[i] >> 1) & 8355711;	//how does this math work?
		i++;
	}
	return (get_color(color));
}

/*This function calculates the x coordinate of the texture needed. We multiply texture width
with the exact point of the square that the ray hits the wall.*/
static int	get_tex_x(double wallhitpoint, t_dda *dda, int tex_size)
{
	int	temp;

	temp = (int)(wallhitpoint * (double)tex_size);
	if ((dda->hor_side == 0 && dda->raydir_x > 0) || (dda->hor_side == 1 && dda->raydir_y < 0)) //or raydir_x < 0 || raydir_y > 0?? diff sources
		temp = tex_size - temp - 1; //we have to flip the texture if the wall is EW and ray comes from west, or if NS wall and ray comes from south
	return (temp);
}

void	draw_wall_stripe(t_dda *dda, t_game *game, double wallhitpoint, int x)
{
	int				tex_size;
	int				tex_x;
	int				tex_y;
	int				index;
	double			step;
	double			tex_pos;
	enum e_textures	type;
	int				color[3];

	type = get_tex_type(dda);
	tex_size = game->textures[type]->width; //pixel size of the texture (we prefer a square)
	tex_x = get_tex_x(wallhitpoint, dda, tex_size);
	step = 1.0 * tex_size / dda->lineheight; //how much to increase the texture coordinate per screen pixel, has to be 1.0 * to make it a float
	tex_pos = (dda->drawstart - MAX_SCREEN_HEIGHT / 2 + dda->lineheight / 2) * step; //starting coordinate of texture (y)
	// printf("texsize %d lineheight %f step %f tex pos %f\n", tex_size, dda->lineheight, step, tex_pos);
	while (dda->drawstart < dda->drawend) //drawing the vertical line one pixel at a time
	{
		tex_y = ((int)tex_pos % tex_size + tex_size) % tex_size; //using modulo makes sure tex_y always is between 0 and tex_h so it wraps it
		index = 4 * (tex_size * tex_y + tex_x); //*4 because each pixel is represented in 4 bytes (in get_curr_color we access three of them rgb)
		tex_pos += step; //moving along the texture
		mlx_put_pixel(game->image, x, dda->drawstart, get_curr_color(game, type, index, dda));
		dda->drawstart++;
	}
}
