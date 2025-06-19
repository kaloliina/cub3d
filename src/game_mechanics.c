/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_mechanics.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 18:07:53 by khiidenh          #+#    #+#             */
/*   Updated: 2025/06/19 13:46:36 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static void	move(t_game *game, int y, int x)
{
//	printf("Old y: %d, old x: %d\n", game->player.y, game->player.x);
	game->player.y = game->player.y + y;
	game->player.x = game->player.x + x;
//	printf("New y: %d, new x: %d\n", game->player.y, game->player.x);
	game->images[PLAYER]->instances[0].y = game->player.y * TILE;
	game->images[PLAYER]->instances[0].x = game->player.x * TILE;
}

void	key_hook(mlx_key_data_t keydata, t_game *game)
{
	if (keydata.action == 1 || keydata.action == 2)
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
