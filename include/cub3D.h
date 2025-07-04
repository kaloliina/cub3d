/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 12:48:44 by khiidenh          #+#    #+#             */
/*   Updated: 2025/07/04 15:37:43 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SO_LONG_H
# define SO_LONG_H
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
# define MAX_BUFFER_SIZE 820
# define TEXTURE_COUNT 4
# define ASSET_COUNT 4
# define TILE 20
# define FILE_INFO_COUNT 6
# define SPEED 0.1

# define ERRMEM "Warning: Memory allocation failed."
# define ERRBER "Warning: File needs to end in .ber extension."
# define ERRFILE "Warning: Failed to open the specified file."
# define ERRSIZE "Warning: Game window is too large."
# define ERRARGC "Warning: Program expects one argument to a valid map."
# define ERRGEN "Warning: Operation failed."
# define ERRFORMAT "Warning: Type identifier (NO, SO, WE, EA, F, C) is expected to be followed up by one or more spaces."
# define ERRRGBFORMAT "Warning: RGB values must be provided as <F/C> <R>,<G>,<B> with numbers only (no signs: + or - allowed)."
# define ERRTHREE "Warning: Expected 3 numbers for RGB values."
# define ERRRGB "Warning: RGB values expected in the range of 0 - 255."
# define ERRMISSINFO "Warning: Could not find the required information in the specified file."
# define ERRMAPGAP "Warning: The map should not contain empty rows in between!"
# define ERRMAPLAST "Warning: The map should be last in the file."

# define ERRP "Warning: Invalid amount of players on the map!"
# define ERRENC "Warning: Map is not enclosed with walls!"
# define ERRCHARS "Warning: Map has invalid characters!"

# define ERRPNG "Warning: Error loading PNG."
# define ERRCONV "Warning: Error converting texture to image."
# define ERRRESIZE "Warning: Error resizing images."
# define ERRIMG "Warning: Error drawing image to window."

enum e_textures
{
	NORTH,
	SOUTH,
	WEST,
	EAST
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

typedef struct s_player
{
	double	x;
	double	y;
	double	dir_x;
	double	dir_y;
}	t_player;

typedef struct s_game
{
	mlx_t		*mlx;
	char		**map;
	int			width;
	int			height;
	double		*planeX;
	double		*planeY;
	t_player	player;
	mlx_image_t	*images[4];
	char	*asset_paths[5];
	int		ceiling_rgb[3];
	int		floor_rgb[3];
	mlx_image_t *image;
	mlx_image_t *minimapimage;
	mlx_texture_t	*textures[TEXTURE_COUNT];
}	t_game;

typedef struct s_map_validation
{
	bool	is_enclosed;
	int		player_count;
	char	**map;
}	t_map_validation;

char	*parse_file(t_game *game, char *buffer);
void	initialize_and_validate(t_game *game);
void	load_textures(t_game *game);
void	render_minimap(t_game *game);
void	render_map(t_game *game);
void	key_hook(mlx_key_data_t keydata, t_game *game);
void	loop_hook(void *param);
void	free_array(char **array, int entirety);
void	cleanup_and_exit(t_game *game, char *str, bool success);

#endif
