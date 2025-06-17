#include "include/cub3d.h"

// #define screenWidth 640
// #define screenHeight 480

// int worldMap[5][5]=
// {
//   {1, 1, 1, 1, 1},
//   {1, 0, 0, 0, 1},
//   {1, 0, 0, 0, 1},
//   {1, 0, 0, 0, 1},
//   {1, 1, 1, 1, 1}
// };

/*static void	build_view_helper(t_game *game, int x, int y)
{
	int	i;
	int	j;

	i = x * TILE;
	j = y * TILE;
	if (game->board[y][x] == 'C')
	{
		if (mlx_image_to_window(game->mlx, game->image[COLLECTIBLE], i, j) < 0)
			exit_game(game, FALSE);
	}
	else if (game->board[y][x] == 'E')
	{
		if (mlx_image_to_window(game->mlx, game->image[EXIT], i, j) < 0)
			exit_game(game, FALSE);
	}
}

void	build_view(t_game *game)
{
	int				x;
	int				y;

	y = 0;
	while (y < game->height)
	{
		x = 0;
		while (x < game->width)
		{
			if (mlx_image_to_window(game->mlx, game->image[BASE],
					x * TILE, y * TILE) < 0)
				exit_game(game, FALSE);
			if (game->board[y][x] == '1')
			{
				if (mlx_image_to_window(game->mlx, game->image[WALL],
						x * TILE, y * TILE) < 0)
					exit_game(game, FALSE);
			}
			if (game->board[y][x] != '1' && game->board[y][x] != '0')
				build_view_helper(game, x, y);
			x++;
		}
		y++;
	}
}

static void	init_texture_path(char **texture_path)
{
	texture_path[0] = "./textures/wall.png";
	texture_path[1] = "./textures/base.png";
	texture_path[2] = "./textures/player.png";
	texture_path[3] = "./textures/collectible.png";
	texture_path[4] = "./textures/exit.png";
	texture_path[5] = "./textures/exit_open.png";
}

void	make_images(t_game *game)
{
	int				i;
	mlx_texture_t	*texture[IMG_COUNT];
	char			*texture_path[IMG_COUNT];

	i = 0;
	init_texture_path(texture_path);
	while (i < IMG_COUNT)
	{
		texture[i] = mlx_load_png(texture_path[i]);
		// if (!texture[i])
		// 	exit_game(game, FALSE);
		game->image[i] = mlx_texture_to_image(game->mlx, texture[i]);
		mlx_delete_texture(texture[i]);
		// if (!game->image[i])
		// 	exit_game(game, FALSE);
		i++;
	}
}

void	myhook(mlx_key_data_t keydata, void *param)
{
	t_line	*line;
	int		x;

	line = (t_line *)param;
	x = 0;
}

int	main()
{
	t_game game;
	double posX = 22.0, posY = 11.5;
	double dirX = -1.0, dirY = 0.0;
	double planeX = 0.0, planeY = 0.66;

	mlx_t	*mlx;
	t_line	*line;

	game.mlx = mlx_init(screenWidth, screenHeight, "test", 0);
	mlx_loop_hook(game.mlx, myhook, line);
	mlx_terminate(game.mlx);
	return (0);
}*/

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
	// double	posX = 2, posY = 3;
	// double dirX = -1, dirY = 0;
	// double planeX = 0, planeY = 0.66;
	// int	x = 0;
	// while (x < 5)
	// {
	// 	double cameraX = 2 * x / 5 - 1;
	// 	double rayDirX = dirX + planeX * cameraX;
	// 	double rayDirY = dirY + planeY * cameraX;
	// 	x++;
	// }

	double	playerX = TILE * 2;
	double	playerY = TILE * 0;
	double	width = TILE * 5;
	double	height = TILE * 5;

    // Init mlx with a canvas size of 256x256 and the ability to resize the window.
    mlx_t* mlx = mlx_init(width, height, "MLX42", true);

    if (!mlx) exit(EXIT_FAILURE);

    // Create a 128x128 image.
    mlx_image_t* img = mlx_new_image(mlx, width, height);

    // Set the channels of each pixel in our image to the maximum byte value of 255.
    memset(img->pixels, 200, img->width * img->height * BPP);

	draw_line(img, playerX, playerY, playerX, (playerY + (3 * TILE)), 0x00FF00);

    // Draw the image at coordinate (0, 0).
    mlx_image_to_window(mlx, img, 0, 0);

    // Run the main loop and terminate on quit.
    mlx_loop(mlx);
    mlx_terminate(mlx);

    return (EXIT_SUCCESS);
}
