#ifndef CUB3D_H
# define CUB3D_H
# include "../MLX42/include/MLX42/MLX42.h"
# include <stdbool.h>
# include <math.h>
# include <stdlib.h>
# define TILE 100
# define TRUE 1
# define FALSE 0

typedef struct s_line
{
	int	x;
	int	y;
	int	y_s;
	int	y_e;
	int	x_tex;
	int	y_tex;
}		t_line;

typedef struct s_ray
{
	double			perp_wall_dist;
	double			ray_dir_x;
	double			ray_dir_y;
	double			curr_x;
	double			curr_y;
	struct s_pos	*draw_start;
	double			draw_end;
}					t_ray;

typedef struct s_player
{
	struct s_pos	*pos;
}					t_player;

typedef struct s_pos
{
	double	x;
	double	y;
}			t_pos;

#endif
