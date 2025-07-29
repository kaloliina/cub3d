#include "../include/cub3D.h"

/*Here we calculate the exact point inside a square at which we hit a wall.
We subtract by floor(temp) which leaves us with the decimal part of the result,
so the exact point inside the square and not which square.*/
void	get_wallhitpoint(t_dda *dda, double *wallhitpoint)
{
	double	temp;

	if (dda->hor_side == 0)
		temp = dda->pos_y + dda->corr_length * dda->raydir_y;
	else
		temp = dda->pos_x + dda->corr_length * dda->raydir_x;
	*wallhitpoint = temp - floor(temp);
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

/*Pixels of the texture are stored in a 1D array. Horizontal sides are made
darker by shifting the bits to "divide by 2" ie. removing the last digit,
and then setting the first bit of every bite to zero by calling AND with
0111 1111 0111 1111 0111 1111.*/
static int	get_curr_color(t_game *game, enum e_textures type, int index,
	t_dda *dda)
{
	int	color[3];
	int	i;

	i = 0;
	while (i < 3)
	{
		color[i] = game->textures[type]->pixels[index + i];
		if (dda->hor_side)
			color[i] = (color[i] >> 1) & 8355711;
		i++;
	}
	return (get_color(color));
}

/*This function calculates the x coordinate of the texture needed. We multiply
texture width with the exact point of the square that the ray hits the wall.
In case of EW wall and ray coming from west, or NS wall and ray comes from
south, we will flip the texture.*/
static int	get_tex_x(double wallhitpoint, t_dda *dda, int tex_size)
{
	int	temp;

	temp = (int)(wallhitpoint * (double)tex_size);
	if ((dda->hor_side == 0 && dda->raydir_x > 0)
		|| (dda->hor_side == 1 && dda->raydir_y < 0))
		temp = tex_size - temp - 1;
	return (temp);
}

/*This function calculates the correct pixel from the texture to draw it in
the calculated position on the screen. We draw one vertical stripe at a time,
looping through y-axis start position to end position and moving along the
texture at the same time.
Tex.step: How much to increase the texture coordinate per screen pixel
Tex.pos = Starting coordinate of texture (y)
Tex.y: we use modulo to ensure the value is always between 0 and tex_size
Index: we multiply by 4 because each pixel color is represented in 4 bytes*/
void	draw_wall_stripe(t_dda *dda, t_game *game, double wallhitpoint, int x)
{
	t_texture		tex;
	int				index;
	enum e_textures	type;
	int				color[3];

	type = get_tex_type(dda);
	tex.size = game->textures[type]->width;
	tex.x = get_tex_x(wallhitpoint, dda, tex.size);
	tex.step = 1.0 * tex.size / dda->lineheight;
	tex.pos = (dda->drawstart - MAX_SCREEN_HEIGHT / 2 + dda->lineheight / 2)
		* tex.step;
	while (dda->drawstart < dda->drawend)
	{
		tex.y = ((int)tex.pos % tex.size + tex.size) % tex.size;
		index = 4 * (tex.size * tex.y + tex.x);
		tex.pos += tex.step;
		mlx_put_pixel(game->image, x, dda->drawstart,
			get_curr_color(game, type, index, dda));
		dda->drawstart++;
	}
}
