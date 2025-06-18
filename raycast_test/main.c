#include "include/cub3d.h"

// #define screenWidth 640
// #define screenHeight 480

int map[5][5]=
{
  {1, 1, 1, 1, 1},
  {1, 0, 0, 0, 1},
  {1, 1, 0, 0, 1},
  {1, 0, 0, 0, 1},
  {1, 1, 1, 1, 1}
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
	double	width = 5;
	double	height = 5;

    // Init mlx with a canvas size of 256x256 and the ability to resize the window.
    mlx_t* mlx = mlx_init(width * TILE, height * TILE, "MLX42", true);

    if (!mlx) exit(EXIT_FAILURE);

    // Create a 128x128 image.
    mlx_image_t* img = mlx_new_image(mlx, width * TILE, height * TILE);

    // Set the channels of each pixel in our image to the maximum byte value of 255.
    memset(img->pixels, 200, img->width * img->height * BPP);

	double	posX = 3;
	double	posY = 2;
	double	dirX = -1, dirY = 0;	//direction is along x-axis to left
	double	planeX = 0;	//planeX is 0 bc it has to be perpendicular
	double	planeY = 0.90;	// this is to make the field of view 66 degrees
	double	raydirY;
	double	raydirX;
	double	sidedistX;
	double	sidedistY;
	int		mapX = (int)posX;
	int		mapY = (int)posY;
	int		stepX;
	int		stepY;
	int		hit = 0;
	int		side = 0;
	int		x = 0;

	draw_line(img, 0 * TILE, 0 * TILE, 5 * TILE, 0 * TILE, 0xFF0000);
	draw_line(img, 0 * TILE, 1 * TILE, 5 * TILE, 1 * TILE, 0xFF0000);
	draw_line(img, 0 * TILE, 2 * TILE, 5 * TILE, 2 * TILE, 0xFF0000);
	draw_line(img, 0 * TILE, 3 * TILE, 5 * TILE, 3 * TILE, 0xFF0000);
	draw_line(img, 0 * TILE, 4 * TILE, 5 * TILE, 4 * TILE, 0xFF0000);
	draw_line(img, 0 * TILE, 0 * TILE, 0 * TILE, 5 * TILE, 0xFF0000);
	draw_line(img, 1 * TILE, 0 * TILE, 1 * TILE, 5 * TILE, 0xFF0000);
	draw_line(img, 2 * TILE, 0 * TILE, 2 * TILE, 5 * TILE, 0xFF0000);
	draw_line(img, 3 * TILE, 0 * TILE, 3 * TILE, 5 * TILE, 0xFF0000);
	draw_line(img, 4 * TILE, 0 * TILE, 4 * TILE, 5 * TILE, 0xFF0000);

	while (x < 15)
	{
		mapX = (int)posX;
		mapY = (int)posY;
		hit = 0;
		double cameraX = 2.0 * x / (double)width - 1.0;
		raydirX = dirX + planeX * cameraX;
		raydirY = dirY + planeY * cameraX;
		// printf("raydirX %f raydirY %f\n", raydirX, raydirY);
		double	deltadistX = fabs(1 / raydirX);
		double	deltadistY = fabs(1 / raydirY);
		double	perpwalldist;

		if (raydirX < 0)
		{
			stepX = -1;
			sidedistX = (posX - mapX) * deltadistX;
		}
		else
		{
			stepX = 1;
			sidedistX = (mapX + 1.0 - posX) * deltadistX;
		}
		if (raydirY < 0)
		{
			stepY = -1;
			sidedistY = (posY - mapY) * deltadistY;
		}
		else
		{
			stepY = 1;
			sidedistY = (mapY + 1.0 - posY) * deltadistY;
		}

		double startX = mapX, startY = mapY;
		while (hit == 0)
		{
			// printf("mapX %d mapY %d\n", mapX, mapY);
			if (sidedistX < sidedistY)
			{
				// printf("now we here\n");
				sidedistX += deltadistX;
				// printf("so sidedistX %f\n", sidedistX);
				mapX += stepX;
				side = 0;
			}
			else
			{
				// printf("now disty increase\n");
				sidedistY += deltadistY;
				// printf("so sidedistY %f\n", sidedistY);
				mapY += stepY;
				side = 1;
			}
			// printf("after change mapX %d mapY %d\n", mapX, mapY);
			if (map[mapY][mapX] == 1)
				hit = 1;
		}
		if (side == 0)
			perpwalldist = (sidedistX - deltadistX);
		else
			perpwalldist = (sidedistY - deltadistY);
		printf("now we draw startX %f startY %f mapX %d mapY %d\n", startX, startY, mapX, mapY);
		draw_line(img, startX * TILE, startY * TILE, mapX * TILE, mapY * TILE, 0xFFFFFF);
		x++;
	}
    // Draw the image at coordinate (0, 0).
    mlx_image_to_window(mlx, img, 0, 0);

    // Run the main loop and terminate on quit.
    mlx_loop(mlx);
    mlx_terminate(mlx);

    return (EXIT_SUCCESS);
}
