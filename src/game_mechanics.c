/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_mechanics.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 18:07:53 by khiidenh          #+#    #+#             */
/*   Updated: 2025/06/26 15:11:32 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

//The wall check is no working proper
//We might need to adjust the players position to be x.5? So it's considered to be in the middle
static void	move(t_game *game, double y, double x)
{
	double	new_y;
	double	new_x;

	new_y = game->player.y + y;
	new_x = game->player.x + x;
	printf("This is where we are: %c", game->map[(int)new_y][(int)new_x]);
	if (game->map[(int)new_y][(int)new_x] == '1')
	{
		printf("Are we actually encountering blocker\n");
		return ;
	}
	printf("Old y: %f, old x: %f\n", game->player.y, game->player.x);
	printf("Olds y: %d, old x: %d\n", (int)game->player.y, (int)game->player.x);
	game->player.y = game->player.y + y;
	game->player.x = game->player.x + x;
	printf("New y: %f, new x: %f\n", game->player.y, game->player.x);
	printf("News y: %d, old x: %d\n", (int)game->player.y, (int)game->player.x);
	render_map(game);
}

//Not sure if this is needed longer but keeping it here just in case
void	key_hook(mlx_key_data_t keydata, t_game *game)
{
}

//Could maybe use delta time from mlx
void	loop_hook(void *param)
{
	t_game *game = param;
		if (mlx_is_key_down(game->mlx, MLX_KEY_UP) || mlx_is_key_down(game->mlx, MLX_KEY_W))
//			&& game->map[(int)game->player.y - 1][(int)game->player.x] != '1')
			move(game, -0.1, 0);
		if (mlx_is_key_down(game->mlx, MLX_KEY_DOWN) || mlx_is_key_down(game->mlx, MLX_KEY_S))
//			&& game->map[(int)game->player.y + 1][(int)game->player.x] != '1')
			move(game, 0.1, 0);
		if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT) || mlx_is_key_down(game->mlx, MLX_KEY_A))
//			&& game->map[(int)game->player.y][(int)game->player.x - 1] != '1')
			move(game, 0, -0.1);
		if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT) || mlx_is_key_down(game->mlx, MLX_KEY_D))
//			&& game->map[(int)game->player.y][(int)game->player.x + 1] != '1')
			move(game, 0, 0.1);
		if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
			mlx_close_window(game->mlx);
}
