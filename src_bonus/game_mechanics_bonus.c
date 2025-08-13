#include "../include/cub3D_bonus.h"

/*This function handles rotation. Cos gives us the part that stays
on the same axis while sin gives us the section that leaks to the other axis.
This 2D rotation formula uses both of these to capture the new directions.*/
static void	rotate(t_game *game, double rotation_dir)
{
	double	rotspeed;
	double	old_dir_x;
	double	old_dir_y;
	double	old_plane_x;

	rotspeed = SPEED * rotation_dir * game->mlx->delta_time;
	old_dir_x = game->player.dir_x;
	old_dir_y = game->player.dir_y;
	old_plane_x = game->plane_x;
	game->player.dir_x = old_dir_x * cos(rotspeed) - old_dir_y * sin(rotspeed);
	game->player.dir_y = old_dir_x * sin(rotspeed) + old_dir_y * cos(rotspeed);
	game->plane_x = game->plane_x * cos(rotspeed) - game->plane_y
		* sin(rotspeed);
	game->plane_y = old_plane_x * sin(rotspeed) + game->plane_y
		* cos(rotspeed);
	render_minimap(game);
	render_map(game);
}

/*This function handles movement. If the direction is either forward or
backwards, the movement either goes forward or backwards relative to the
player's direction. If the movement is left or right, we are utilising
the other axis and strafing left or right relative to the player's
direction.*/
static void	move(t_game *game, enum e_directions direction,
int y_sign, int x_sign)
{
	double	new_y;
	double	new_x;

	new_y = game->player.y;
	new_x = game->player.x;
	if (direction == FORWARD || direction == BACKWARD)
	{
		new_y += y_sign * SPEED * game->player.dir_y
			* game->mlx->delta_time;
		new_x += x_sign * SPEED * game->player.dir_x
			* game->mlx->delta_time;
	}
	if (direction == LEFT || direction == RIGHT)
	{
		new_y += y_sign * SPEED * game->player.dir_x
			* game->mlx->delta_time;
		new_x += x_sign * SPEED * game->player.dir_y
			* game->mlx->delta_time;
	}
	if (game->map[(int)new_y][(int)new_x] == '1')
		return ;
	game->player.y = new_y;
	game->player.x = new_x;
	render_minimap(game);
	render_map(game);
}

void	key_hook(mlx_key_data_t keydata, void *param)
{
	t_game	*game;

	game = (t_game *)param;
	if (keydata.key == MLX_KEY_M && keydata.action == MLX_PRESS)
	{
		game->mouse_lock = !game->mouse_lock;
		if (!game->mouse_lock)
			mlx_set_cursor_mode(game->mlx, MLX_MOUSE_HIDDEN);
		else
			mlx_set_cursor_mode(game->mlx, MLX_MOUSE_NORMAL);
	}
}

void	loop_hook(void *param)
{
	t_game	*game;

	game = param;
	if (mlx_is_key_down(game->mlx, MLX_KEY_W))
		move(game, FORWARD, 1, 1);
	if (mlx_is_key_down(game->mlx, MLX_KEY_S))
		move(game, BACKWARD, -1, -1);
	if (mlx_is_key_down(game->mlx, MLX_KEY_A))
		move(game, LEFT, -1, 1);
	if (mlx_is_key_down(game->mlx, MLX_KEY_D))
		move(game, RIGHT, 1, -1);
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT))
		rotate(game, -1);
	if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT))
		rotate(game, 1);
	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(game->mlx);
}

/*Cursor hook does the rotation if we use mouse instead of
left/right arrow key.
Sensitivity tracks how far from the center is the current mouse
position, and gives that (multiplied with 0.01) to rotation
function as the "amount" of rotation. After each rotate call, it
sets mouse position back to the center of the screen.*/
void	cursor_hook(void *param)
{
	t_game	*game;
	int		old_x;
	int		old_y;
	double	sensitivity;

	game = param;
	if (!game->mouse_lock)
	{
		mlx_get_mouse_pos(game->mlx, &old_x, &old_y);
		sensitivity = (old_x - MAX_SCREEN_WIDTH / 2) * 0.01;
		if (sensitivity != 0)
			rotate(game, sensitivity);
		mlx_set_mouse_pos(game->mlx, MAX_SCREEN_WIDTH / 2,
			MAX_SCREEN_HEIGHT / 2);
	}
}
