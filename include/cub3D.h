/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 12:48:44 by khiidenh          #+#    #+#             */
/*   Updated: 2025/08/13 11:53:42 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H
# include "../MLX42/include/MLX42/MLX42.h"
# include "../libft/includes/libft.h"
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <stdio.h>
# include <math.h>
# define MAX_SCREEN_WIDTH 3840
# define MAX_SCREEN_HEIGHT 2160
# define MAX_BUFFER_SIZE 65536
# define TEXTURE_COUNT 5
# define TILE 20
# define FILE_INFO_COUNT 6
# define SPEED 0.6	//used to be 0.05 for a while, what will we decide on?
# define PLAYER_CLR (int[3]){255, 0, 0}
# define BASE_CLR (int[3]){205, 195, 210}
# define WALL_CLR (int[3]){0, 0, 0}

# define ERRMSG "Error\n"
# define ERRMEM "Memory allocation failed.\n"
# define ERRBER "File needs to end in .cub extension.\n"
# define ERRFILE "Failed to open the specified file.\n"
# define ERRSIZE "Game map is too large.\n"
# define ERRARGC "Program expects one argument to a valid map.\n"
# define ERRGEN "Operation failed.\n"
# define ERRFORMAT "Type identifier (NO, SO, WE, EA, F, C) is \
expected to be followed up by one or more spaces.\n"
# define ERRRGBFORMAT "RGB values must be provided as <F/C> <R>,<G>,<B> \
with numbers only (no signs: + or - allowed).\n"
# define ERRTHREE "Expected 3 numbers for RGB values.\n"
# define ERRRGB "RGB values expected in the range of 0 - 255.\n"
# define ERRMISSINFO "Could not find the required information in the \
specified file.\n"
# define ERRMAPGAP "The map should not contain empty rows in between!\n"
# define ERRMAPLAST "The map should be last in the file.\n"

# define ERRP "Invalid amount of players on the map!\n"
# define ERRENC "Map is not enclosed with walls!\n"
# define ERRCHARS "Map has invalid characters!\n"

# define ERRPNG "Error loading PNG.\n"
# define ERRCONV "Error converting texture to image.\n"
# define ERRTEXT "Texture needs to be a square.\n"
# define ERRRESIZE "Error resizing images.\n"
# define ERRNEWIMG "Error creating a new image buffer.\n"
# define ERRIMG "Error drawing image to window.\n"

enum e_textures
{
	NORTH,
	SOUTH,
	WEST,
	EAST,
	SPRITE
};

enum e_assets
{
	BASE,
	WALL,
	PLAYER,
	EMPTY,
};

enum e_directions
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
};

typedef struct s_texture
{
	int		size;
	int		x;
	int		y;
	double	step;
	double	pos;
}			t_texture;

typedef struct s_player
{
	double	x;
	double	y;
	double	dir_x;
	double	dir_y;
}	t_player;

typedef struct s_sprite
{
	double	x;
	double	y;
	double	dist;
}			t_sprite;

typedef struct s_render_sprite
{
	double	sprite_x;
	double	sprite_y;
	double	inv_det;
	double	corr_x;
	double	sprite_depth;
	int		sprite_screen_x;
	int		sprite_size;
	int		x_start;
	int		y_start;
	int		x_end;
	int		y_end;
	int		tex_x;
}			t_render_sprite;

typedef struct s_dda
{
	double	pos_x;
	double	pos_y;
	double	dir_x;
	double	dir_y;
	double	raydir_y;
	double	raydir_x;
	double	sidedist_x;
	double	sidedist_y;
	double	squaredist_x;
	double	squaredist_y;
	double	camera_x;
	double	corr_dist;
	int		lineheight;
	int		map_x;
	int		map_y;
	int		step_x;
	int		step_y;
	int		hit;
	int		hor_side;
	int		y_start;
	int		y_end;
}			t_dda;

typedef struct s_game
{
	mlx_t			*mlx;
	char			**map;
	int				width;
	int				height;
	double			plane_x;
	double			plane_y;
	bool			mouse_lock;
	t_player		player;
	char			*asset_paths[6];
	int				ceiling_rgb[3];
	int				floor_rgb[3];
	int				sprite_amt;
	struct s_sprite	*sprites;
	mlx_image_t		*image;
	mlx_image_t		*minimapimage;
	mlx_texture_t	*textures[TEXTURE_COUNT];
}	t_game;

typedef struct s_map_validation
{
	bool	is_enclosed;
	int		player_count;
	char	**map;
}	t_map_validation;

//parsing and validation
char	*parse_file(t_game *game, char *buffer);
void	initialize_and_validate(t_game *game);
bool	check_is_digit(char *str);

//render
void	init_maps(t_game *game);
void	render_minimap(t_game *game);
void	render_map(t_game *game);
//sprites
void	render_sprites(t_game *game, t_dda *dda, double *z_buffer);
void	sort_sprites(t_game *game, t_dda *dda, int i);
bool	set_behind_wall(t_render_sprite *data, double *z_buffer);
int		find_drawedges(t_render_sprite *data, int flag, int max);
//dda
void	init_dda(t_dda *dda, t_game *game);
void	update_dda(t_dda *dda, t_game *game, int x);
void	get_line_properties(t_dda *dda, t_game *game);
//render utils
int		minimap_edge(double coordinate, bool start, int max);
void	draw_line(t_game *game, double start_x, double start_y);
void	draw_pixels(t_game *game, enum e_assets type, int x, int y);
int		get_color(int *rgb);
void	texture_square_checker(t_game *game, int i);
//wall textures
void	get_wallhitpoint(t_dda *dda, double *wallhitpoint);
int		get_curr_color(t_game *game, enum e_textures type,
			int index, t_dda *dda);
void	draw_wall_stripe(t_dda *dda, t_game *game, double wallhitpoint, int x);

//game mechanics
void	key_hook(mlx_key_data_t keydata, void *param);
void	loop_hook(void *param);
void	cursor_hook(void *param);

//errors and exits
void	free_array(char **array, int entirety);
void	cleanup_and_exit(t_game *game, char *str, bool success, bool textures);

#endif
