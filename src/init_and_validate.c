/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_and_validate.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 13:00:59 by khiidenh          #+#    #+#             */
/*   Updated: 2025/06/09 16:29:57 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

/* From here we can remove the collectables and exit
as they are not needed but could somehow use the flood fill to test the corners,
if we encounter something that is not 1, 0, N S E W, then we know we failed*/
//maybe we can check if we are on the top, bottom or left edge, then return false
//otherwise check if we have encountered ' ' and then say its not enclosed
#include <stdio.h>
//validation height needs to be the highest height possible in the map
//maybe also need to hceck if tab[player_y] is not null
//also need to maybe put the space or null terminator check inside the first check as well
static void	fill(char **tab, int player_x, int player_y,
t_map_validation *validation)
{
	if (player_y < 0 || player_x < 0 || player_y >= validation->height || player_x >= ft_strlen(tab[player_y]))
	{
		//outofbounds
		return ;
	}
	if (tab[player_y][player_x] == ' ' || tab[player_y][player_x] == '\0')
	{
		//outofbounds
		return ;
	}
	if (tab[y][x] == '1' || tab[y][x] == 'F')
		return ;
	tab[player_y][player_x] = 'F'
	fill(tab, player_x + 1, player_y, validation);
	fill(tab, player_x - 1, player_y, validation);
	fill(tab, player_x, player_y + 1, validation);
	fill(tab, player_x, player_y - 1, validation);
}
#include <stdio.h>
static void	flood_fill(t_game *game, t_map_validation *validation)
{
	int		y;
	char	**tab;

	y = 0;
	tab = malloc(sizeof(char *) * (game->height + 1));
	if (tab == NULL)
		cleanup_and_exit(game, ERRGEN, 0);
	validation->height = game->height;
	while (y < game->height)
	{
		tab[y] = ft_strdup(game->map[y]);
//this is for debugging purposes
		printf("%s\n", tab[y]);
		if (tab[y] == NULL)
		{
			free_floodmap(tab);
			cleanup_and_exit(game, ERRGEN, 0);
		}
		y++;
	}
	tab[y] = NULL;
	fill(tab, game->player.x, game->player.y, validation);
	free_floodmap(tab);
}

static void	validation_check(t_game *game, t_map_validation *validation
, int validation_complete)
{
	if (validation->player_count != 1)
		cleanup_and_exit(game, ERRP, 0);
	if (validation->is_enclosed == false)
		cleanup_and_exit(game, ERRENC, 0);
	if (validation->has_invalid_chars == true)
		cleanup_and_exit(game, ERRCHARS, 0);
	if (validation_complete == 0)
		return ;
}

static void	update_validation(t_game *game, t_map_validation *validation)
{
	char	current;

	current = game->map[game->height][game->width];
	if (current == 'C')
		validation->collectable_count++;
	if (current == 'E')
		validation->exit_count++;
	if (current == 'N' || current == 'S' || current == 'E' || current == 'W')
		validation->player_count++;
//this one maybe should be done aggain with flood  fill
	if (current != '1' && current != '0' && current != 'N'
		&& current != 'S' && current != 'E' && current != 'W' && current != ' ')
		validation->has_invalid_chars = true;
	// if ((game->map[0][game->width] != '1')
	// || (game->map[game->height + 1] == NULL && current != '1'))
	// 	validation->is_enclosed = false;
	// if (game->map[game->height][0] != '1'
	// || (game->map[game->height][game->width + 1] == '\0' && current != '1'))
	// 	validation->is_enclosed = false;
}

void	initialize_and_validate(t_game *game)
{
	t_map_validation	validation;

	validation = (t_map_validation){true, true, false, 0, 0, 0, 0, 0, 0};
	game->player = (t_player){0};
	while (game->map[game->height] != NULL)
	{
		game->width = 0;
		while (game->map[game->height][game->width] != '\0')
		{
			if (game->map[game->height][game->width] == 'C')
				game->collectables++;
			if (game->map[game->height][game->width] == 'P')
			{
				game->player.y = game->height;
				game->player.x = game->width;
			}
			update_validation(game, &validation);
			game->width++;
		}
		game->height++;
	}
	validation_check(game, &validation, 0);
	flood_fill(game, &validation);
	validation_check(game, &validation, 1);
}
