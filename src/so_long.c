#include "cub3D.h"

/*
--1--
In this function we are reading the file data to a buffer. We also cover the cases:
- If file does not end in .cub extension
- If file cannot be found or we don't have permissions to open
- If file is bigger than screen size (we might need to adjust this)
We then move to parsing the file and with the remaining buffer we split it into
array of strings.
*/
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
	free (map);
	if (game->map == NULL || game->map[0] == NULL)
		cleanup_and_exit(game, ERRMAPLAST, 0);
}

/*Ive marked the sections with 1-n, to go step by step over the parsing flow*/
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
		mlx_loop_hook(game.mlx, (void *) loop_hook, &game);
		mlx_loop(game.mlx);
		cleanup_and_exit(&game, NULL, 1);
	}
	cleanup_and_exit(&game, ERRARGC, 0);
}
