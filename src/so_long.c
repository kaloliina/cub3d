/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   so_long.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/24 12:02:12 by khiidenh          #+#    #+#             */
/*   Updated: 2025/06/13 11:20:37 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

bool	check_if_number_in_range(int rgb)
{
	if (rgb < 0 || rgb > 255)
		return (false);
	return (true);
}

bool	check_is_digit(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-' || str[i] == ' ')
		i++;
	if (str[i] == '\0')
		return (false);
	while (str[i] != '\0')
	{
		if (!(str[i] >= 48 && str[i] <= 57))
			return (false);
		i++;
	}
	return (true);
}

static void extract_rgb_info(t_game *game, char *temp)
{
	int		i;
	char	**textures;

	i = 0;
	textures = ft_split(ft_substr(temp, 2, ft_strlen(temp) - ft_strlen(ft_strchr(temp, '\n')) - 2), ',');
	if (textures[3] != NULL)
		cleanup_and_exit(game, ERRTHREE, 0);
	while (textures[i] != NULL)
	{
		if (check_is_digit(textures[i]) == false)
			cleanup_and_exit(game, ERRRGB, 0);
		i++;
	}
	i = 0;
	if (temp[0] == 'F')
	{
		while (i < 3)
		{
			game->floor_rgb[i] = ft_atoi(textures[i]);
			if (check_if_number_in_range(game->floor_rgb[i]) == false)
				cleanup_and_exit(game, ERRRGB, 0);
			printf("%d\n", game->floor_rgb[i]);
			i++;
		}
		free(textures);
	}
	if (temp[0] == 'C')
	{
		while (i < 3)
		{
			game->ceiling_rgb[i] = ft_atoi(textures[i]);
			if (check_if_number_in_range(game->ceiling_rgb[i]) == false)
				cleanup_and_exit(game, ERRRGB, 0);
			printf("%d\n", game->ceiling_rgb[i]);
			i++;
		}
		free(textures);
	}
}

static char	*extract_info(t_game *game, char *buffer)
{
	int		i;
	int		j;
	int		index;
	char	*temp;
	static const char	*information[6];

	information[0] = "NO";
	information[1] = "SO";
	information[2] = "WE";
	information[3] = "EA";
	information[4] = "F";
	information[5] = "C";
	i = 0;
	j = 0;
	index = -1;
	while (i < 6)
	{
		temp = ft_strnstr(buffer, information[i], MAX_BUFFER_SIZE);
		if (temp == NULL)
			cleanup_and_exit(game, ERRMISSINFO, 0);
		if (index == -1 || index > (int)ft_strlen(temp))
			index = ft_strlen(ft_strchr(temp, '\n'));
		if (i < 4)
			game->asset_paths[i] = ft_substr(temp, 3, ft_strlen(temp) - ft_strlen(ft_strchr(temp, '\n')) - 3);
		else
		{
			extract_rgb_info(game, temp);
		}
		if (i < 4)
			printf("Path: %s\n", game->asset_paths[i]);
		i++;
	}
		printf("The actual map: %s\n", &buffer[ft_strlen(buffer) - index]);
		return (&buffer[ft_strlen(buffer) - index]);
}

static void	parse_map(t_game *game, char *str)
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
	if (close (fd) == -1)
		cleanup_and_exit(game, ERRFILE, 0);
	map = extract_info(game, buffer);
	if (ft_strnstr(ft_strchr(map, '1'), "\n\n", MAX_BUFFER_SIZE))
		cleanup_and_exit(game, ERRMAPGAP, 0);
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
		parse_map(&game, argv[1]);
		initialize_and_validate(&game);
		if ((game.width * TILE) > MAX_SCREEN_WIDTH
			|| (game.height * TILE) > MAX_SCREEN_HEIGHT)
			cleanup_and_exit(&game, ERRSIZE, 0);
		game.mlx = mlx_init(game.width * TILE, game.height * TILE,
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



