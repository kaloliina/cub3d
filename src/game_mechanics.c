/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_mechanics.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 18:07:53 by khiidenh          #+#    #+#             */
/*   Updated: 2025/06/12 13:04:10 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

static void	check_progress(t_game *game)
{
	size_t	i;

	i = 0;
	if (game->map[game->player.y][game->player.x] == 'C')
	{
		game->map[game->player.y][game->player.x] = '0';
		game->player.collected++;
		while (i < game->images[COLLECTABLE]->count)
		{
			if (game->images[COLLECTABLE]->instances[i].x
				== game->player.x * TILE
				&& game->images[COLLECTABLE]->instances[i].y
				== game->player.y * TILE)
			{
				game->images[COLLECTABLE]->instances[i].enabled = false;
			}
			i++;
		}
	}
//	if (game->player.collected == game->collectables
//		&& game->map[game->player.y][game->player.x] == 'E')
//		mlx_close_window(game->mlx);
}

static void	print_movements(t_game *game)
{
	ft_putstr_fd("Player movements: ", 1);
	ft_putnbr_fd(game->player.movements, 1);
	ft_putstr_fd("\n", 1);
}

static void	move(t_game *game, int y, int x)
{
	game->player.y = game->player.y + y;
	game->player.x = game->player.x + x;
	game->images[PLAYER_LEFT]->instances[0].y = game->player.y * TILE;
	game->images[PLAYER_RIGHT]->instances[0].y = game->player.y * TILE;
	game->images[PLAYER_LEFT]->instances[0].x = game->player.x * TILE;
	game->images[PLAYER_RIGHT]->instances[0].x = game->player.x * TILE;
	if (x == -1)
	{
		game->images[PLAYER_RIGHT]->instances[0].enabled = false;
		game->images[PLAYER_LEFT]->instances[0].enabled = true;
	}
	if (x == 1)
	{
		game->images[PLAYER_RIGHT]->instances[0].enabled = true;
		game->images[PLAYER_LEFT]->instances[0].enabled = false;
	}
	game->player.movements++;
	print_movements(game);
	check_progress(game);
//	if (game->player.collected == game->collectables)
//	{
//		game->images[EXIT_CLOSED]->instances[0].enabled = false;
//		game->images[EXIT_OPEN]->instances[0].enabled = true;
//	}
}

void	key_hook(mlx_key_data_t keydata, t_game *game)
{
	if (keydata.action == 1)
	{
		if ((keydata.key == MLX_KEY_W || keydata.key == MLX_KEY_UP)
			&& game->map[game->player.y - 1][game->player.x] != '1')
			move(game, -1, 0);
		if ((keydata.key == MLX_KEY_S || keydata.key == MLX_KEY_DOWN)
			&& game->map[game->player.y + 1][game->player.x] != '1')
			move(game, 1, 0);
		if ((keydata.key == MLX_KEY_A || keydata.key == MLX_KEY_LEFT)
			&& game->map[game->player.y][game->player.x - 1] != '1')
			move(game, 0, -1);
		if ((keydata.key == MLX_KEY_D || keydata.key == MLX_KEY_RIGHT)
			&& game->map[game->player.y][game->player.x + 1] != '1')
			move(game, 0, 1);
		if (keydata.key == MLX_KEY_ESCAPE)
			mlx_close_window(game->mlx);
	}
}
