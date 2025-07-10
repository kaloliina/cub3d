#include "include/cub3d.h"

// #define screenWidth 640
// #define screenHeight 480

int map[10][10]=
{
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0, 0, 0, 0, 0, 1},
  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};

#include "MLX42/include/MLX42/MLX42.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// Bytes Per Pixel. Since each pixel is represented as an integer, it will be four bytes for four channels.
#define BPP sizeof(int32_t)

void	draw_line(void *image, int beginX, int beginY, int endX, int endY, int color)
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

int32_t	main(void)
{
	double	width = 10;
	double	height = 10;

    mlx_t* mlx = mlx_init(width * TILE, height * TILE, "MLX42", true);

    if (!mlx) exit(EXIT_FAILURE);

    mlx_image_t* img = mlx_new_image(mlx, width * TILE, height * TILE);

    memset(img->pixels, 200, img->width * img->height * BPP);

	double	posX = 2;	//player position X
	double	posY = 8;	//player position Y
	double	dirX = 0, dirY = -1;	//direction is along y-axis upwards (normalized so between -1 and 1)
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
	int		hor_side;
	int		x = 0;

	draw_line(img, 0 * TILE, 0 * TILE, 10 * TILE, 0 * TILE, 0xFF0000);
	draw_line(img, 0 * TILE, 1 * TILE, 10 * TILE, 1 * TILE, 0xFF0000);
	draw_line(img, 0 * TILE, 2 * TILE, 10 * TILE, 2 * TILE, 0xFF0000);
	draw_line(img, 0 * TILE, 3 * TILE, 10 * TILE, 3 * TILE, 0xFF0000);
	draw_line(img, 0 * TILE, 4 * TILE, 10 * TILE, 4 * TILE, 0xFF0000);
	draw_line(img, 0 * TILE, 5 * TILE, 10 * TILE, 5 * TILE, 0xFF0000);
	draw_line(img, 0 * TILE, 6 * TILE, 10 * TILE, 6 * TILE, 0xFF0000);
	draw_line(img, 0 * TILE, 7 * TILE, 10 * TILE, 7 * TILE, 0xFF0000);
	draw_line(img, 0 * TILE, 8 * TILE, 10 * TILE, 8 * TILE, 0xFF0000);
	draw_line(img, 0 * TILE, 9 * TILE, 10 * TILE, 9 * TILE, 0xFF0000);
	draw_line(img, 0 * TILE, 0 * TILE, 0 * TILE, 10 * TILE, 0xFF0000);
	draw_line(img, 1 * TILE, 0 * TILE, 1 * TILE, 10 * TILE, 0xFF0000);
	draw_line(img, 2 * TILE, 0 * TILE, 2 * TILE, 10 * TILE, 0xFF0000);
	draw_line(img, 3 * TILE, 0 * TILE, 3 * TILE, 10 * TILE, 0xFF0000);
	draw_line(img, 4 * TILE, 0 * TILE, 4 * TILE, 10 * TILE, 0xFF0000);
	draw_line(img, 5 * TILE, 0 * TILE, 5 * TILE, 10 * TILE, 0xFF0000);
	draw_line(img, 6 * TILE, 0 * TILE, 6 * TILE, 10 * TILE, 0xFF0000);
	draw_line(img, 7 * TILE, 0 * TILE, 7 * TILE, 10 * TILE, 0xFF0000);
	draw_line(img, 8 * TILE, 0 * TILE, 8 * TILE, 10 * TILE, 0xFF0000);
	draw_line(img, 9 * TILE, 0 * TILE, 9 * TILE, 10 * TILE, 0xFF0000);

	while (x < width)
	{
		mapX = (int)posX;
		mapY = (int)posY;
		hit = 0;
		hor_side = 0;
		double cameraX = 2.0 * (x + 0.5) / (double)width - 1.0;
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
				hor_side = 0;
			}
			else
			{
				sidedistY += deltadistY;
				mapY += stepY;
				hor_side = 1;
			}
			if (map[mapY][mapX] == 1)
				hit = 1;
		}
		if (hor_side == 0)
		{
			perpwalldist = (sidedistX - deltadistX);
		}
		else
		{
			perpwalldist = (sidedistY - deltadistY);
		}
		//This section draws just single rays in 2d top-down view
		double hitX = posX + raydirX * perpwalldist;
		double hitY = posY + raydirY * perpwalldist;
		draw_line(img, posX * TILE, posY * TILE, hitX * TILE, hitY * TILE, 0xFFFFFF);

		/*This section draws the '3d' view but it is veeeery initial version
		//  Calculate height of line to draw on screen
    	double lineHeight = (height / perpwalldist);
      	//calculate lowest and highest pixel to fill in current stripe
     	int drawStart = -lineHeight / 2 + height / 2;
		if(drawStart < 0)drawStart = 0;
      	printf("drawStart %d\n", drawStart);
      	int drawEnd = lineHeight / 2 + height / 2;
     	if(drawEnd >= height)drawEnd = height - 1;
		printf("drawEnd %d\n", drawEnd);
		int l = 0;
		while (l < TILE)
		{
			draw_line(img, (x * TILE) + l, drawStart * TILE, (x * TILE) + l, drawEnd * TILE, 0xFFFFFF);
			l++;
		}*/
		x++;
	}
    mlx_image_to_window(mlx, img, 0, 0);

    mlx_loop(mlx);
    mlx_terminate(mlx);

    return (EXIT_SUCCESS);
}
