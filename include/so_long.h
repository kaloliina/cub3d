/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 12:48:44 by khiidenh          #+#    #+#             */
/*   Updated: 2025/06/12 16:40:08 by khiidenh         ###   ########.fr       */
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
# define ASSET_COUNT 7
# define TILE 100

# define ERRBER "Warning: File needs to end in .ber extension."
# define ERRFILE "Warning: Failed to open the specified file."
# define ERREMPTY "Warning: Map file is empty!"
# define ERRSIZE "Warning: Game window is too large."
# define ERRARGC "Warning: Program expects one argument to a valid map."
# define ERRGEN "Warning: Operation failed."
# define ERRRGB "Warning: Expected 3 colours in the range of 0 - 255."

# define ERRC "Warning: No collectables found on the map!"
# define ERRE "Warning: Invalid amount of exits on the map!"
# define ERRP "Warning: Invalid amount of players on the map!"
# define ERRREC "Warning: Map is not rectangular!"
# define ERRENC "Warning: Map is not enclosed with walls!"
# define ERRCHARS "Warning: Map has invalid characters!"
# define ERRCUNR "Warning: Player cannot reach all collectables!"
# define ERREUNR "Warning: Player cannot reach the exit!"

# define ERRPNG "Warning: Error loading PNG."
# define ERRCONV "Warning: Error converting texture to image."
# define ERRRESIZE "Warning: Error resizing images."
# define ERRIMG "Warning: Error drawing image to window."

enum e_assets
{
	BASE,
	WALL,
	COLLECTABLE,
	PLAYER_LEFT,
	PLAYER_RIGHT,
	EXIT_CLOSED,
	EXIT_OPEN,
};

typedef struct s_player
{
	int	x;
	int	y;
	int	collected;
	int	movements;
}	t_player;

typedef struct s_game
{
	mlx_t		*mlx;
	char		**map;
	int			width;
	int			height;
	t_player	player;
	mlx_image_t	*images[7];
	char	*asset_paths[4];
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
void	free_floodmap(char **tab);
void	cleanup_and_exit(t_game *game, char *str, bool success);
#endif
