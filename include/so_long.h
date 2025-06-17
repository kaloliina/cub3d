/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 12:48:44 by khiidenh          #+#    #+#             */
/*   Updated: 2025/06/17 14:37:34 by khiidenh         ###   ########.fr       */
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
# define MAX_SCREEN_WIDTH 3840
# define MAX_SCREEN_HEIGHT 2160
# define MAX_BUFFER_SIZE 820
# define ASSET_COUNT 4
# define TILE 100
# define FILE_INFO_COUNT 6

# define ERRMEM "Warning: Memory allocation failed."
# define ERRBER "Warning: File needs to end in .ber extension."
# define ERRFILE "Warning: Failed to open the specified file."
# define ERREMPTY "Warning: Map file is empty!"
# define ERRSIZE "Warning: Game window is too large."
# define ERRARGC "Warning: Program expects one argument to a valid map."
# define ERRGEN "Warning: Operation failed."
# define ERRTHREE "Warning: Expected 3 numbers for RGB values"
# define ERRRGB "Warning: RGB values expected in the range of 0 - 255."
# define ERRMISSINFO "Warning: Could not find the required information in the specified file."
# define ERRMAPGAP "Warning: The map should not contain empty rows in between!"

# define ERRP "Warning: Invalid amount of players on the map!"
# define ERRENC "Warning: Map is not enclosed with walls!"
# define ERRCHARS "Warning: Map has invalid characters!"

# define ERRPNG "Warning: Error loading PNG."
# define ERRCONV "Warning: Error converting texture to image."
# define ERRRESIZE "Warning: Error resizing images."
# define ERRIMG "Warning: Error drawing image to window."

enum e_assets
{
	BASE,
	WALL,
	PLAYER,
	EMPTY,
};

typedef struct s_player
{
	int	x;
	int	y;
	int	dir_x;
	int	dir_y;
}	t_player;

typedef struct s_game
{
	mlx_t		*mlx;
	char		**map;
	int			width;
	int			height;
	t_player	player;
	mlx_image_t	*images[4];
	char	*asset_paths[5];
	int		ceiling_rgb[3];
	int		floor_rgb[3];
}	t_game;

typedef struct s_map_validation
{
	bool	is_enclosed;
	bool	has_invalid_chars;
	int		player_count;
}	t_map_validation;

void	initialize_and_validate(t_game *game);
void	load_textures(t_game *game);
void	key_hook(mlx_key_data_t keydata, t_game *game);
void	free_array(char **array, int entirety);
void	cleanup_and_exit(t_game *game, char *str, bool success);
void	early_cleanup_and_exit(char *str);
#endif
