/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 12:01:20 by sojala            #+#    #+#             */
/*   Updated: 2025/08/14 12:01:21 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D.h"

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
		cleanup_and_exit(game, ERRBER, 0, 0);
	fd = open(str, O_RDONLY);
	if (fd == -1)
		cleanup_and_exit(game, ERRFILE, 0, 0);
	bytes_read = read(fd, buffer, MAX_BUFFER_SIZE);
	close (fd);
	if (bytes_read == -1 || bytes_read > MAX_BUFFER_SIZE - 1)
		cleanup_and_exit(game, ERRFILE, 0, 0);
	buffer[bytes_read] = '\0';
	map = parse_file(game, buffer);
	game->map = ft_split(map, '\n');
	free (map);
	if (game->map == NULL)
		cleanup_and_exit(game, ERRMEM, 0, 0);
	if (game->map[0] == NULL)
		cleanup_and_exit(game, ERRMAPLAST, 0, 0);
}

static void	print_info(void)
{
	printf("* * * * * * * * * * * * * * * * * * * * *\n");
	printf("* * * * * WELCOME TO NEW YORK * * * * * *\n");
	printf("* * * * * * * * * * * * * * * * * * * * *\n");
	printf("* * * * * * move: W, A, S, D  * * * * * *\n");
	printf("* * * rotate: left and right arrow  * * *\n");
	printf("* * * enable rotation with mouse: M * * *\n");
	printf("* * * * * * * * * * * * * * * * * * * * *\n");
}

int	main(int argc, char *argv[])
{
	t_game	game;

	game = (t_game){0};
	if (argc == 2)
	{
		parse_map_file(&game, argv[1]);
		initialize_and_validate(&game);
		game.mlx = mlx_init(MAX_SCREEN_WIDTH, MAX_SCREEN_HEIGHT,
				"cub3D", false);
		if (game.mlx == NULL)
			cleanup_and_exit(&game, ERRGEN, 0, 1);
		print_info();
		init_maps(&game);
		mlx_key_hook(game.mlx, (void *) key_hook, &game);
		mlx_loop_hook(game.mlx, (void *) loop_hook, &game);
		mlx_cursor_hook(game.mlx, (void *) cursor_hook, &game);
		mlx_loop(game.mlx);
		cleanup_and_exit(&game, NULL, 1, 1);
	}
	cleanup_and_exit(&game, ERRARGC, 0, 0);
}
