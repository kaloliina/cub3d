#ifndef CUB3D_H
# define CUB3D_H
# include "../MLX42/include/MLX42/MLX42.h"
# include <stdbool.h>
# include <math.h>
# include <stdlib.h>
# define TILE 64
# define TRUE 1
# define FALSE 0

typedef enum s_image
{
	WALL,
	BASE,
	PLAYER,
	COLLECTIBLE,
	EXIT,
	EXIT_OPEN,
	IMG_COUNT
}			t_image;

typedef struct s_line
{
	int	x;
	int	y;
	int	y_s;
	int	y_e;
	int	x_tex;
	int	y_tex;
}		t_line;

typedef struct s_player
{
	int	x;
	int	y;
	int	prev_x;
	int	prev_y;
}				t_player;

typedef struct s_point
{
	int	x;
	int	y;
}				t_point;

typedef struct s_game
{
	int			width;
	int			height;
	char		**board;
	t_player	*player;
	t_point		start;
	t_point		exit;
	mlx_t		*mlx;
}				t_game;

#endif
