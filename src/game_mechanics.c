#include "cub3D.h"

/*This function rotates "the player", right now it's reflected only on minimap*/
static void	rotate(t_game *game, double rotation_dir)
{
	double	rotspeed;
	double	old_dir_x;
	double	old_dir_y;
	double	old_planeX;

	/*Is rotspeed correct regarding the actual map? Right now rotating messes up the
	wall in case of basic_map_tiny if eg. player starts with S and rotates to face
	the wall that is on the left*/
	printf("Before rotation: dir_x=%f, dir_y=%f\n", game->player.dir_x, game->player.dir_y);
	rotspeed = SPEED * rotation_dir;
	old_dir_x = game->player.dir_x;
	old_dir_y = game->player.dir_y;
	old_planeX = *game->planeX;
	printf("old planeX is %f\n", old_planeX);
	game->player.dir_x = old_dir_x * cos(rotspeed) - old_dir_y * sin(rotspeed);
	game->player.dir_y = old_dir_x * sin(rotspeed) + old_dir_y * cos(rotspeed);
	/*This planeX and planeY update has to be studied more!*/
	*(game->planeX) = *(game->planeX) * cos(rotspeed) - *(game->planeY) * sin(rotspeed);
	*(game->planeY) = old_planeX * sin(rotspeed) + *(game->planeY) * cos(rotspeed);
	printf("After rotation: dir_x=%f, dir_y=%f\n", game->player.dir_x, game->player.dir_y);
	render_map(game);
	render_minimap(game);
}

/*This function is responsible of moving the player in the maze. Right now it'set up in
a way that it moves relative to the player's facing direction (so forward doesnt always mean up etc)
The old version is commented out in the end of the file*/
static void	move(t_game *game, enum e_directions direction)
{
	double	old_y;
	double	old_x;

	old_y = game->player.y;
	old_x = game->player.x;
	printf("Old y: %f, old x: %f\n", game->player.y, game->player.x);
	printf("Olds y: %d, old x: %d\n", (int)game->player.y, (int)game->player.x);
	if (direction == FORWARD)
	{
		game->player.y += SPEED * game->player.dir_y;
		game->player.x += SPEED * game->player.dir_x;
	}
	if (direction == BACKWARD)
	{
		game->player.y -= SPEED * game->player.dir_y;
		game->player.x -= SPEED * game->player.dir_x;
	}
	if (direction == LEFT)
	{
		game->player.y = game->player.y - (SPEED * game->player.dir_x);
		game->player.x = game->player.x + (SPEED * game->player.dir_y);
	}
	if (direction == RIGHT)
	{
		game->player.y = game->player.y + (SPEED * game->player.dir_x);
		game->player.x = game->player.x - (SPEED * game->player.dir_y);
	}
	if (game->map[(int)game->player.y][(int)game->player.x] == '1')
	{
		game->player.y = old_y;
		game->player.x = old_x;
		return ;
	}
	printf("New y: %f, new x: %f\n", game->player.y, game->player.x);
	printf("News y: %d, old x: %d\n", (int)game->player.y, (int)game->player.x);
	render_map(game);
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
		move(game, FORWARD);
	if (mlx_is_key_down(game->mlx, MLX_KEY_S))
		move(game, BACKWARD);
	if (mlx_is_key_down(game->mlx, MLX_KEY_A))
		move(game, LEFT);
	if (mlx_is_key_down(game->mlx, MLX_KEY_D))
		move(game, RIGHT);
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT))
		rotate(game, -1);
	if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT))
		rotate(game, 1);
	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(game->mlx);
}

/*THIS IS THE OLD VERSION.
It handles the rotation and the movement but the movement does not follow players direction,
so even if you are facing a certain way, pressing S moves player towards bottom of the map.*/

// /*This function rotates "the player", right now it's reflected only on minimap*/
// static void	rotate(t_game *game, double speed, double rotation_dir)
// {
// 	double	rotspeed;
// 	double	old_dir_x;
// 	double	old_dir_y;

// 	printf("Before rotation: dir_x=%f, dir_y=%f\n", game->player.dir_x, game->player.dir_y);
// 	rotspeed = speed * rotation_dir;
// 	old_dir_x = game->player.dir_x;
// 	old_dir_y = game->player.dir_y;
// 	game->player.dir_x = old_dir_x * cos(rotspeed) - old_dir_y * sin(rotspeed);
// 	game->player.dir_y = old_dir_x * sin(rotspeed) + old_dir_y * cos(rotspeed);
// 	printf("After rotation: dir_x=%f, dir_y=%f\n", game->player.dir_x, game->player.dir_y);
// 	render_minimap(game);
// }

// /*This function is responsible of moving the player in the maze. However
// the player should probably move according to the direction that they are facing xd*/
// static void	move(t_game *game, double y, double x)
// {
// 	double	new_y;
// 	double	new_x;

// 	new_y = game->player.y + y;
// 	new_x = game->player.x + x;
// 	if (game->map[(int)new_y][(int)new_x] == '1')
// 	{
// 		printf("Are we actually encountering blocker\n");
// 		return ;
// 	}
// 	printf("Old y: %f, old x: %f\n", game->player.y, game->player.x);
// 	printf("Olds y: %d, old x: %d\n", (int)game->player.y, (int)game->player.x);
// 	game->player.y = game->player.y + y;
// 	game->player.x = game->player.x + x;
// 	printf("New y: %f, new x: %f\n", game->player.y, game->player.x);
// 	printf("News y: %d, old x: %d\n", (int)game->player.y, (int)game->player.x);
// 	render_minimap(game);
// }

// //Not sure if this is needed longer but keeping it here just in case
// void	key_hook(mlx_key_data_t keydata, t_game *game)
// {
// }

// /*
// "The WASD keys must allow you to move the point of view through the maze.
// The left and right arrow keys must allow you to look left and right in the maze.
// ESC must lose the window"
// */
// void	loop_hook(void *param)
// {
// 	t_game	*game;

// 	game = param;
// 	if (mlx_is_key_down(game->mlx, MLX_KEY_W))
// 		move(game, -0.1, 0);
// 	if (mlx_is_key_down(game->mlx, MLX_KEY_S))
// 		move(game, 0.1, 0);
// 	if (mlx_is_key_down(game->mlx, MLX_KEY_A))
// 		move(game, 0, -0.1);
// 	if (mlx_is_key_down(game->mlx, MLX_KEY_D))
// 		move(game, 0, 0.1);
// 	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT))
// 		rotate(game, 0.1, -1);
// 	if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT))
// 		rotate(game, 0.1, 1);
// 	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
// 		mlx_close_window(game->mlx);
// }
