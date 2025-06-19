/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 12:02:12 by khiidenh          #+#    #+#             */
/*   Updated: 2025/06/19 13:49:56 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	parse_map_file(t_game *game, char *str)
{
	int		len;
	int		fd;
	int		bytes_read;
	char	buffer[MAX_BUFFER_SIZE];
	char	*map;

	len = ft_strlen(str) - 1;
	if (len < 4 || str[len] != 'b' || str[len - 1] != 'u'
		|| str[len - 2] != 'c' || str[len - 3] != '.')
		cleanup_and_exit(game, ERRBER, 0);
	fd = open(str, O_RDONLY);
	if (fd == -1)
		cleanup_and_exit(game, ERRFILE, 0);
	bytes_read = read(fd, buffer, MAX_BUFFER_SIZE);
	if (bytes_read == -1 || bytes_read > MAX_BUFFER_SIZE - 1)
	{
		close (fd);
		cleanup_and_exit(game, ERRFILE, 0);
	}
	buffer[bytes_read] = '\0';
	close (fd);
	map = parse_file(game, buffer);
	game->map = ft_split(map, '\n');
	if (game->map == NULL || game->map[0] == NULL)
		cleanup_and_exit(game, ERREMPTY, 0);
}

int	main(int argc, char *argv[])
{
	t_game	game;

	game = (t_game){0};
	if (argc == 2)
	{
		parse_map_file(&game, argv[1]);
		initialize_and_validate(&game);
		if ((game.width * TILE) > MAX_SCREEN_WIDTH
			|| (game.height * TILE) > MAX_SCREEN_HEIGHT)
			cleanup_and_exit(&game, ERRSIZE, 0);
		game.mlx = mlx_init(MAX_SCREEN_WIDTH, MAX_SCREEN_HEIGHT,
				"Piscine", false);
		if (game.mlx == NULL)
			cleanup_and_exit(&game, ERRGEN, 0);
		load_textures(&game);
		mlx_key_hook(game.mlx, (void *) key_hook, &game);
		mlx_loop(game.mlx);
		cleanup_and_exit(&game, NULL, 1);
	}
	cleanup_and_exit(&game, ERRARGC, 0);
}
