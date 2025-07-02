/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/30 16:46:14 by khiidenh          #+#    #+#             */
/*   Updated: 2025/07/02 18:04:28 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
static void	draw_line(t_game *game, double beginX, double beginY)
{
	double	end_x;
	double	end_y;
	double	delta_x;
	double	delta_y;
	int		pixels;

	end_x = beginX + game->player.dir_x * 20;
	end_y = beginY + game->player.dir_y * 20;
	delta_x = end_x - beginX;
	delta_y = end_y - beginY;
	pixels = sqrt((delta_x * delta_x) + (delta_y * delta_y));
	delta_x /= pixels;
	delta_y /= pixels;
	while (pixels)
	{
		mlx_put_pixel(game->minimapimage, beginX, beginY, 0x000099FF);
		beginX += delta_x;
		beginY += delta_y;
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

/*
--10--
This function right now goes through the height and width of the map and puts pixel by pixel to the image.
After we have filled the image, we put image to window.

static void	render_map(t_game *game)
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
	mlx_image_to_window(game->mlx, game->image, 0, 0);
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
	mlx_image_to_window(game->mlx, game->image, 0, 0);
}*/

void	draw_line_wall(void *image, int beginX, int beginY, int endX, int endY, int color)
{
	double	deltaX = endX - beginX;	//length of line on x axis
	double	deltaY = endY - beginY;	//lengths of line on y axis
	int		pixels = sqrt((deltaX * deltaX) + (deltaY * deltaY));
	//pythagorean !! so pixels gets the "length of the hypotenuse"

	deltaX /= pixels;	//we want to increment pixelX by "one pixel" and not the whole length
	deltaY /= pixels;

	double	pixelX = beginX;
	double	pixelY = beginY;
	while (pixels)
	{
		mlx_put_pixel(image, pixelX, pixelY, color);
		pixelX += deltaX;
		pixelY += deltaY;
		pixels--;
	}
}

void	render_map(t_game *game)
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
	mlx_image_to_window(game->mlx, game->image, 0, 0);
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
	mlx_image_to_window(game->mlx, game->image, 0, 0);

	double	posX = game->player.x;	//player position X
	double	posY = game->player.y;	//player position Y
	double	dirX = game->player.dir_x;
	double	dirY = game->player.dir_y;	//direction is along y-axis upwards (normalized so between -1 and 1)
	double	planeX = 0.66;	// this is to make the field of view 66 degrees
	double	planeY = 0; //planeY is 0 bc it has to be perpendicular
	double	raydirY;
	double	raydirX;
	double	sidedistX;
	double	sidedistY;
	int		mapX, mapY;
	int		stepX;
	int		stepY;
	int		hit = 0;
	int		side;

	x = 0;
	while (x < MAX_SCREEN_WIDTH)
	{
		mapX = (int)posX;
		mapY = (int)posY;
		hit = 0;
		side = 0;
		double cameraX = 2.0 * x / (double)game->width - 1.0;
		raydirX = dirX + planeX * cameraX;	//position vector + specific part of camera plane
		raydirY = dirY + planeY * cameraX;
		double	deltadistX = (raydirX == 0) ? 1e30 : fabs(1 / raydirX);
		double	deltadistY = (raydirY == 0) ? 1e30 : fabs(1 / raydirY);
		double	perpwalldist;

		if (raydirX < 0)	//if ray is moving left
		{
			stepX = -1;
			sidedistX = (posX - mapX) * deltadistX;
		}
		else
		{
			stepX = 1;
			sidedistX = (mapX + 1.0 - posX) * deltadistX;
		}
		if (raydirY < 0)	//if ray is moving up
		{
			stepY = -1;
			sidedistY = (posY - mapY) * deltadistY;
		}
		else
		{
			stepY = 1;
			sidedistY = (mapY + 1.0 - posY) * deltadistY;
		}

		while (hit == 0)
		{
			if (sidedistX < sidedistY)
			{
				sidedistX += deltadistX;
				mapX += stepX;
				side = 0;
			}
			else
			{
				sidedistY += deltadistY;
				mapY += stepY;
				side = 1;
			}
			if (game->map[mapY][mapX] == 1)
				hit = 1;
		}
		if (side == 0)
		{
			perpwalldist = (sidedistX - deltadistX);
		}
		else
		{
			perpwalldist = (sidedistY - deltadistY);
		}
		//This section draws the '3d' view but it is veeeery initial version
		//  Calculate height of line to draw on screen
    	double lineHeight = MAX_SCREEN_HEIGHT / perpwalldist;
      	//calculate lowest and highest pixel to fill in current stripe
     	int drawStart = -lineHeight / 2 + MAX_SCREEN_HEIGHT / 2;
		if(drawStart < 0)drawStart = 0;
      	printf("drawStart %d x %d\n", drawStart, x);
      	int drawEnd = lineHeight / 2 + MAX_SCREEN_HEIGHT / 2;
     	if(drawEnd >= MAX_SCREEN_HEIGHT)drawEnd = MAX_SCREEN_HEIGHT - 1;
		printf("drawEnd %d\n", drawEnd);
		draw_line_wall(game->image, x, drawStart, x, drawEnd, 0xFF0000);
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
	while (game->map[y] != NULL)
	{
		x = 0;
		while (x < (int)ft_strlen(game->map[y]))
		{
			if (game->map[y][x] == '1')
				draw_pixels(game, WALL, x * TILE, y * TILE);
			else if (ft_strchr("0NSEW", game->map[y][x]))
			{
				draw_pixels(game, BASE, x * TILE, y * TILE);
			}
			x++;
		}
		y++;
	}
	draw_pixels(game, PLAYER, (game->player.x - 0.5) * TILE, (game->player.y - 0.5) * TILE);
	draw_line(game, game->player.x * TILE, game->player.y * TILE);
	if (mlx_image_to_window(game->mlx, game->minimapimage,
			0, 0) < 0)
		cleanup_and_exit(game, ERRIMG, 0);
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
			cleanup_and_exit(game, ERRPNG, 0);
		game->images[i] = mlx_texture_to_image(game->mlx, texture);
		if (game->images[i] == NULL)
		{
			mlx_delete_texture(texture);
			cleanup_and_exit(game, ERRCONV, 0);
		}
		mlx_delete_texture(texture);
		if ((mlx_resize_image(game->images[i], TILE, TILE)) == false)
			cleanup_and_exit(game, ERRRESIZE, 0);
		i++;
	}
	render_map(game);
	render_minimap(game);
}
