/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_and_validate.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 13:00:59 by khiidenh          #+#    #+#             */
/*   Updated: 2025/06/13 13:02:06 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	fill(char **tab, int player_x, int player_y,
t_map_validation *validation, char prev)
{
	printf("Location y: %d x: %d\n", player_y, player_x);
	if (player_y < 0 || player_x < 0 || tab[player_y] == NULL || player_x > (int)ft_strlen(tab[player_y]) || tab[player_y][player_x] == '\0')
	{
		if (prev == 'v')
			validation->is_enclosed = false;
		return ;
	}
	if (tab[player_y][player_x] == 'v' || tab[player_y][player_x] == 'w' || tab[player_y][player_x] == 'l')
		return ;
	if (prev == 'v' && (tab[player_y][player_x] != '1' && tab[player_y][player_x] != '0'))
	{
		validation->is_enclosed = false;
		return ;
	}
	if (tab[player_y][player_x] == '1')
		tab[player_y][player_x] = 'w';
	else if (tab[player_y][player_x] == '0')
		tab[player_y][player_x] = 'v';
	else if (tab[player_y][player_x] == 'N')
		tab[player_y][player_x] = 'v';
	else if (tab[player_y][player_x] == ' ')
		tab[player_y][player_x] = 'l';
	else
		return ;
	fill(tab, player_x + 1, player_y, validation, tab[player_y][player_x]);
	fill(tab, player_x - 1, player_y, validation, tab[player_y][player_x]);
	fill(tab, player_x, player_y + 1, validation, tab[player_y][player_x]);
	fill(tab, player_x, player_y - 1, validation, tab[player_y][player_x]);
	return ;
}

static void	flood_fill(t_game *game, t_map_validation *validation)
{
	int		y;
	char	**tab;

	y = 0;
	tab = malloc(sizeof(char *) * (game->height + 1));
	if (tab == NULL)
		cleanup_and_exit(game, ERRGEN, 0);
	while (y < game->height)
	{
		tab[y] = ft_strdup(game->map[y]);
		if (tab[y] == NULL)
		{
			free_floodmap(tab);
			cleanup_and_exit(game, ERRGEN, 0);
		}
		y++;
	}
	tab[y] = NULL;
	fill(tab, game->player.x, game->player.y, validation, '1');
	free_floodmap(tab);
}

static void	validation_check(t_game *game, t_map_validation *validation)
{
	if (validation->player_count != 1)
		cleanup_and_exit(game, ERRP, 0);
	if (validation->is_enclosed == false)
		cleanup_and_exit(game, ERRENC, 0);
	if (validation->has_invalid_chars == true)
		cleanup_and_exit(game, ERRCHARS, 0);
}

void	initialize_and_validate(t_game *game)
{
	t_map_validation	validation;

	validation = (t_map_validation){true, false, 0};
	game->player = (t_player){0};
	while (game->map[game->height] != NULL)
	{
		game->width = 0;
		while (game->map[game->height][game->width] != '\0')
		{
			if (ft_strchr("NSEW", game->map[game->height][game->width]))
			{
				game->player.y = game->height;
				game->player.x = game->width;
				validation.player_count++;
				if (game->map[game->height][game->width] == 'N')
					game->player.dir_y = -1;
				if (game->map[game->height][game->width] == 'S')
					game->player.dir_y = 1;
				if (game->map[game->height][game->width] == 'W')
					game->player.dir_x = -1;
				if (game->map[game->height][game->width] == 'E')
					game->player.dir_x = 1;
			}
			if (!ft_strchr("10 NSEW", game->map[game->height][game->width]))
				validation.has_invalid_chars = true;
			game->width++;
		}
		game->height++;
	}
	flood_fill(game, &validation);
	validation_check(game, &validation);
}
