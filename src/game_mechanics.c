/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   game_mechanics.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 18:07:53 by khiidenh          #+#    #+#             */
/*   Updated: 2025/06/27 13:11:33 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

/*This function rotates "the player", right now it's reflected only on minimap*/
static void	rotate(t_game *game, double speed, double rotation_dir)
{
	double	rotspeed;
	double	old_dir_x;
	double	old_dir_y;

	printf("Before rotation: dir_x=%f, dir_y=%f\n", game->player.dir_x, game->player.dir_y);
	rotspeed = speed * rotation_dir;
	old_dir_x = game->player.dir_x;
	old_dir_y = game->player.dir_y;
	game->player.dir_x = old_dir_x * cos(rotspeed) - old_dir_y * sin(rotspeed);
	game->player.dir_y = old_dir_x * sin(rotspeed) + old_dir_y * cos(rotspeed);
	printf("After rotation: dir_x=%f, dir_y=%f\n", game->player.dir_x, game->player.dir_y);
	render_minimap(game);
}

/*This function is responsible of moving the player in the maze. However
the player should probably move according to the direction that they are facing xd*/
static void	move(t_game *game, double y, double x)
{
	double	new_y;
	double	new_x;

	new_y = game->player.y + y;
	new_x = game->player.x + x;
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
	render_minimap(game);
}

//Not sure if this is needed longer but keeping it here just in case
void	key_hook(mlx_key_data_t keydata, t_game *game)
{
}

/*
"The WASD keys must allow you to move the point of view through the maze.
The left and right arrow keys must allow you to look left and right in the maze.
ESC must lose the window"
*/
void	loop_hook(void *param)
{
	t_game	*game;

	game = param;
	if (mlx_is_key_down(game->mlx, MLX_KEY_W))
		move(game, -0.1, 0);
	if (mlx_is_key_down(game->mlx, MLX_KEY_S))
		move(game, 0.1, 0);
	if (mlx_is_key_down(game->mlx, MLX_KEY_A))
		move(game, 0, -0.1);
	if (mlx_is_key_down(game->mlx, MLX_KEY_D))
		move(game, 0, 0.1);
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT))
		rotate(game, 0.1, -1);
	if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT))
		rotate(game, 0.1, 1);
	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(game->mlx);
}
