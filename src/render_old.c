#include "cub3D.h"

/*
--11--
This function "combines" the different rgb values together into one integer.
By shifting r 24 bits left, g 16 bits left, g 8 bits left and leaving a (alpha channel)
where it is.
*/
static int	get_colour(int *rgb)
{
	int	colour;
	int	a;

	a = 255;
	colour = rgb[0] << 24 | rgb[1] << 16 | rgb[2] << 8 | a;
	return (colour);
}

//The number 20 here represents the length of the line
static void	draw_line(t_game *game, double begin_x, double begin_y)
{
	double	end_x;
	double	end_y;
	double	delta_x;
	double	delta_y;
	int		pixels;

	end_x = begin_x + game->player.dir_x * 20;
	end_y = begin_y + game->player.dir_y * 20;
	delta_x = end_x - begin_x;
	delta_y = end_y - begin_y;
	pixels = sqrt((delta_x * delta_x) + (delta_y * delta_y));
	delta_x /= pixels;
	delta_y /= pixels;
	while (pixels)
	{
		mlx_put_pixel(game->minimapimage, begin_x, begin_y, 0x000099FF);
		begin_x += delta_x;
		begin_y += delta_y;
		--pixels;
	}
}

//There seems to be a bug with colours
static void	draw_pixels(t_game *game, enum e_assets type, int x, int y)
{
	int	y_tile;
	int	x_tile;
	int	original_x;

	y_tile = 0;
	original_x = x;
	while (y_tile < TILE)
	{
		x_tile = 0;
		x = original_x;
		while (x_tile < TILE)
		{
			if (type == BASE)
				mlx_put_pixel(game->minimapimage, x, y, 0x00FFFFFF);
			if (type == WALL)
				mlx_put_pixel(game->minimapimage, x, y, 0x00CC00FF);
			if (type == PLAYER && x_tile > 5 && x_tile < 14 && y_tile > 5 && y_tile < 14)
				mlx_put_pixel(game->minimapimage, x, y, 0x009933FF);
			x_tile++;
			x++;
		}
		y++;
		y_tile++;
	}
}
/*I added this to be able to draw lines for walls with specificied length, but when using textures we don't need it.
Keeping it here for now just in case..
void	draw_line_wall(void *image, int begin_x, int begin_y, int end_x, int end_y, int color)
{
	double	delta_x = end_x - begin_x;	//length of line on x axis
	double	delta_y = end_y - begin_y;	//lengths of line on y axis
	int		pixels = sqrt((delta_x * delta_x) + (delta_y * delta_y));
	//pythagorean !! so pixels gets the "length of the hypotenuse"

	delta_x /= pixels;	//we want to increment pixel_x by "one pixel" and not the whole length
	delta_y /= pixels;

	double	pixel_x = begin_x;
	double	pixel_y = begin_y;
	while (pixels)
	{
		mlx_put_pixel(image, pixel_x, pixel_y, color);
		pixel_x += delta_x;
		pixel_y += delta_y;
		pixels--;
	}
}*/

void	draw_ceiling_floor(t_game *game)
{
	int	x;
	int	y;

	x = 0;
	y = 0;
	while (y < MAX_SCREEN_HEIGHT / 2)
	{
		x = 0;
		while (x < MAX_SCREEN_WIDTH)
		{
			mlx_put_pixel(game->image, x, y, get_colour(game->ceiling_rgb));
			x++;
		}
		y++;
	}
	y = MAX_SCREEN_HEIGHT / 2;
	while (y < MAX_SCREEN_HEIGHT)
	{
		x = 0;
		while (x < MAX_SCREEN_WIDTH)
		{
			mlx_put_pixel(game->image, x, y, get_colour(game->floor_rgb));
			x++;
		}
		y++;
	}
}

int	get_step(double raydir)
{
	if (raydir < 0)	//if ray is moving left (raydir_x) or up (y)
		return (-1);	//we take one step to left/up, decrease x/y
	else	//if ray is moving right/down (if raydir == 0, this step will be unused so it doesn't matter)
		return (1);
}

double	get_sidedist(double raydir, double pos, int map, double deltadist)
{
	if (raydir < 0)
		return ((pos - map) * deltadist);	//player position - left/up edge of current square
	else
		return ((map + 1.0 - pos) * deltadist);	//right/down edge of curr square + player
}

/*--10--
This function first draws the ceiling and floor, looping through the window width and height.
Then it draws the walls by using the raycasting mathematics (will be better explained soon...)
After we have filled the image, we put image to window.

I took out putting image to window in between steps and only do it in the end, is it ok?*/
void	render_map(t_game *game)
{
	int	x;
	int	y;
	double	pos_x;
	double	pos_y;
	double	dir_x;
	double	dir_y;
	double	raydir_y;
	double	raydir_x;
	double	sidedist_x;	//the distance that ray needs to travel from start position to next x side (border of square)
	double	sidedist_y;	//the dist ray needs to travel from start to next y side
	int		map_x;
	int		map_y;	//current map position
	int		step_x;
	int		step_y;
	int		hit;
	int		side;

	draw_ceiling_floor(game);
	pos_x = game->player.x;	//player position x
	pos_y = game->player.y;	//player position y
	dir_x = game->player.dir_x;
	dir_y = game->player.dir_y;
	x = 0;	//loop through the screen, drawing the walls one vertical line at a time
	while (x < MAX_SCREEN_WIDTH)
	{
		map_x = (int)pos_x;	//let's start from player position
		map_y = (int)pos_y;
		step_x = 0;
		step_y = 0;
		hit = 0;	//indicates whether we have hit a wall on the map while calculating ray length
		side = 0;	//indicates whether we hit a NS or a EW wall (0 if vertical ie. EW, x side)
		//camera_x is the x-coordinate on the camera plane - -1 on the left side, 0 in the middle and 1 on the right side of screen
		double camera_x = 2 * x / (double)MAX_SCREEN_WIDTH - 1;
		raydir_x = dir_x + *(game->plane_x) * camera_x;	//position vector + specific part of camera plane (see lodev pic)
		raydir_y = dir_y + *(game->plane_y) * camera_x;
		double	deltadist_x = (raydir_x == 0) ? 1e30 : fabs(1 / raydir_x);	//this is the "hypotenuse" (see lodev pic)
		double	deltadist_y = (raydir_y == 0) ? 1e30 : fabs(1 / raydir_y);	//if raydir is 0, let's put a big value to not divide by 0 later
		double	perpwalldist;	//corrected wall distance to avoid fisheye

		step_x = get_step(raydir_x);
		step_y = get_step(raydir_y);
		sidedist_x = get_sidedist(raydir_x, pos_x, map_x, deltadist_x);
		sidedist_y = get_sidedist(raydir_y, pos_y, map_y, deltadist_y);
		while (hit == 0)	//we keep going with the ray until we hit a wall
		{
			if (sidedist_x < sidedist_y)	//if the way to next x side is shorter than to next y side
			{
				sidedist_x += deltadist_x;	//we move in x direction to the next x side so sidedist_x is now the whole way so far
				map_x += step_x;	//we also move in map squares
				side = 0;
			}
			else
			{
				sidedist_y += deltadist_y;
				map_y += step_y;
				side = 1;
			}
			if (game->map[map_y][map_x] == '1')
				hit = 1;
		}
		if (side == 0)	//if we hit a vertical wall so x side
			perpwalldist = (sidedist_x - deltadist_x);
		else	//if we hit horizontal wall, y side
			perpwalldist = (sidedist_y - deltadist_y);
		//  Calculate height of the line to draw on screen
		double lineheight = MAX_SCREEN_HEIGHT / perpwalldist;
		//Calculate lowest and highest pixel to draw in current stripe
		int drawstart = (MAX_SCREEN_HEIGHT / 2) - (lineheight / 2); //below the middle of the screen, we want to center it so half is below and half above middle
		if (drawstart < 0)
			drawstart = 0;
		int drawend = (MAX_SCREEN_HEIGHT / 2) + (lineheight / 2); //above the middle of the screen
		if (drawend >= MAX_SCREEN_HEIGHT)
			drawend = MAX_SCREEN_HEIGHT - 1;
		//Now we calculate the texture
		double wallhitpoint;	//the exact spot where wall was hit - can be x or y coordinate depending on side, but in texture always x
		int	texsize = 128;	//pixel size of the texture (we prefer a square so no need for sep. height and width, this size is of white.png)
		if (side == 0)	//if we hit vertical wall
			wallhitpoint = pos_y + perpwalldist * raydir_y;
		else
			wallhitpoint = pos_x + perpwalldist * raydir_x;
		wallhitpoint -= floor(wallhitpoint);	//floor means taking the largest int value that is not greater than wallhitpoint
		int tex_x = (int)(wallhitpoint * texsize);
		if ((side == 0 && raydir_x > 0) || (side == 1 && raydir_y < 0)) //or raydir_x < 0 || raydir_y > 0?? diff sources
			tex_x = texsize - tex_x - 1;	//we have to flip the texture if the wall is EW and ray comes from west, or if NS wall and ray comes from south
		double step = 1.0 * texsize / lineheight;	//how much to increase the texture coordinate per screen pixel, has to be 1.0 * to make it a float
		double texpos = (drawstart - MAX_SCREEN_HEIGHT / 2 + lineheight / 2) * step;	//starting coordinate of texture (y)
		int y = drawstart;
		enum e_textures type = NORTH;
		if (side == 0 && raydir_x > 0)
			type = EAST;
		else if (side == 0 && raydir_x < 0)
			type = WEST;
		else if (side == 1 && raydir_y > 0)
			type = SOUTH;
		int	color[3];
		uint8_t	*pixels = game->textures[type]->pixels;
		while (y < drawend)	//drawing the vertical line one pixel at a time
		{
			int tex_y = (int)texpos & (texsize - 1);	//the bitwise & makes sure tex_y always is between 0 and 127 so it wraps it
			int	index = 4 * (texsize * tex_y	+ tex_x);
			texpos += step;
			//We have to handle different textures depending on the direction, but now just tested with texture[0]
			color[0] = pixels[index];
			color[1] = pixels[index + 1];
			color[2] = pixels[index + 2];
			int color_curr = get_colour(color);
			mlx_put_pixel(game->image, x, y, color_curr);
			y++;
		}
		// draw_line_wall(game->image, x, drawStart, x, drawEnd, 0xFF0000);
		x++;
	}
	mlx_image_to_window(game->mlx, game->image, 0, 0);
}

/*
--9--
This is basically just rendering the minimap on the top left corner, we can remove it if we want! :D
The function also calls to the render actual map which right now adds the floor and ceiling colors.
*/
void	render_minimap(t_game *game)
{
	int	x;
	int	y;

	y = 0;
	while (game->map[y])
	{
		x = 0;
		while (x < (int)ft_strlen(game->map[y]))
		{
			if (game->map[y][x] == '1')
				draw_pixels(game, WALL, x * TILE, y * TILE);
			else if (ft_strchr("0NSEW", game->map[y][x]))
				draw_pixels(game, BASE, x * TILE, y * TILE);
			x++;
		}
		y++;
	}
	draw_pixels(game, PLAYER, (game->player.x - 0.5) * TILE, (game->player.y - 0.5) * TILE);
	draw_line(game, game->player.x * TILE, game->player.y * TILE);
	if (mlx_image_to_window(game->mlx, game->minimapimage,
			0, 0) < 0)
		cleanup_and_exit(game, ERRIMG, 0, 1);
}

/*
--8--
Here we are creating a new image that we will use for drawing floor and ceiling.
We are also loading the wall textures and putting the textures to image.
In addition, we are creating an image for the minimap.
After this, we "render map"
*/
void	load_textures(t_game *game)
{
	int				i;
	mlx_texture_t	*texture;
	const char		**asset_paths;

	game->image = mlx_new_image(game->mlx, MAX_SCREEN_WIDTH, MAX_SCREEN_HEIGHT);
	game->minimapimage = mlx_new_image(game->mlx, game->width * TILE, game->height * TILE);
	i = 0;
	while (i < ASSET_COUNT)
	{
		texture = mlx_load_png(game->asset_paths[i]);
		if (texture == NULL)
			cleanup_and_exit(game, ERRPNG, 0, 1);
		game->images[i] = mlx_texture_to_image(game->mlx, texture);
		if (game->images[i] == NULL)
		{
			mlx_delete_texture(texture);
			cleanup_and_exit(game, ERRCONV, 0, 1);
		}
		mlx_delete_texture(texture);
		if ((mlx_resize_image(game->images[i], TILE, TILE)) == false)
			cleanup_and_exit(game, ERRRESIZE, 0, 1);
		i++;
	}
	render_map(game);
	render_minimap(game);
}
