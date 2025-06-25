/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_and_validate.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 13:00:59 by khiidenh          #+#    #+#             */
/*   Updated: 2025/06/25 13:10:04 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

/*
--7--
We run the flood fill for the map, basically we are checking if previous place was
walkable spot and the current spot is not 1 (wall) and 0 (walkable path), then we
know the map is not surrounded by walls. We also check if we have reached out of bounds
and if the previous one was walkable path, we know the map is not surrounded by walls.
*/
static void	fill(int x, int y,
t_map_validation *validation, char prev)
{
	if (y < 0 || x < 0 || validation->map[y] == NULL
		|| x >= (int)ft_strlen(validation->map[y]))
	{
		if (prev == 'v')
			validation->is_enclosed = false;
		return ;
	}
	if (validation->map[y][x] == 'v' || validation->map[y][x] == 'w')
		return ;
	if (prev == 'v' && (validation->map[y][x] != '1'
		&& validation->map[y][x] != '0'))
	{
		validation->is_enclosed = false;
		return ;
	}
	if (validation->map[y][x] == '1')
		validation->map[y][x] = 'w';
	else if (ft_strchr("NSEW0", validation->map[y][x]))
		validation->map[y][x] = 'v';
	else
		return ;
	fill(x + 1, y, validation, validation->map[y][x]);
	fill(x - 1, y, validation, validation->map[y][x]);
	fill(x, y + 1, validation, validation->map[y][x]);
	fill(x, y - 1, validation, validation->map[y][x]);
}

static void	flood_fill(t_game *game, t_map_validation *validation)
{
	int		y;

	y = 0;
	validation->map = malloc(sizeof(char *) * (game->height + 1));
	if (validation->map == NULL)
		cleanup_and_exit(game, ERRGEN, 0);
	while (y < game->height)
	{
		validation->map[y] = ft_strdup(game->map[y]);
		if (validation->map[y] == NULL)
		{
			free_array(validation->map, 1);
			cleanup_and_exit(game, ERRGEN, 0);
		}
		y++;
	}
	validation->map[y] = NULL;
	fill(game->player.x, game->player.y, validation, '1');
	free_array(validation->map, 1);
	if (validation->is_enclosed == false)
		cleanup_and_exit(game, ERRENC, 0);
}

/*
--6--
If we encounter N,S,E,W characters in the map, we mark the player's y and x location.
We also update the player count.
After this we mark the players y and x direction.
Lastly, we check if there is a character that is not 1, 0, , N, S, E, W, then there's an invalid character and we exit.
*/
void	validate_map_elements(t_game *game, t_map_validation *validation,
int x, int y)
{
	if (ft_strchr("NSEW", game->map[y][x]))
	{
		game->player.y = y;
		game->player.x = x;
		validation->player_count++;
		if (game->map[game->height][game->width] == 'N')
			game->player.dir_y = -1;
		if (game->map[game->height][game->width] == 'S')
			game->player.dir_y = 1;
		if (game->map[game->height][game->width] == 'W')
			game->player.dir_x = -1;
		if (game->map[game->height][game->width] == 'E')
			game->player.dir_x = 1;
	}
	if (!ft_strchr("10 NSEW", game->map[y][x]))
		cleanup_and_exit(game, ERRCHARS, 0);
}

/*
--5--
This function basically iterates through the entire map. We call the function
validate_map_elements to track information and we also mark the height of the map, as well as the max
width we encounter.
We also check if the player count is not 1 at the end, then we encounter an error.
*/
void	initialize_and_validate(t_game *game)
{
	int					x;
	int					y;
	t_map_validation	validation;

	x = 0;
	y = 0;
	validation = (t_map_validation){true, 0, NULL};
	game->player = (t_player){0};
	while (game->map[y] != NULL)
	{
		x = 0;
		while (game->map[y][x] != '\0')
		{
			validate_map_elements(game, &validation, x, y);
			x++;
		}
		if ((int)ft_strlen(game->map[y]) > game->width)
			game->width = (int)ft_strlen(game->map[y]);
		y++;
	}
	if (validation.player_count != 1)
		cleanup_and_exit(game, ERRP, 0);
	game->height = y;
	flood_fill(game, &validation);
}
