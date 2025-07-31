#include "../include/cub3D.h"

static void	fill(int x, int y, t_map_validation *validation, char prev)
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
		&& validation->map[y][x] != '0' && validation->map[y][x] != 'T'))
	{
		validation->is_enclosed = false;
		return ;
	}
	if (validation->map[y][x] == '1')
		validation->map[y][x] = 'w';
	else if (ft_strchr("NSEW0T", validation->map[y][x]))
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
		cleanup_and_exit(game, ERRGEN, 0, 0);
	while (y < game->height)
	{
		validation->map[y] = ft_strdup(game->map[y]);
		if (validation->map[y] == NULL)
		{
			free_array(validation->map, 1);
			cleanup_and_exit(game, ERRGEN, 0, 0);
		}
		y++;
	}
	validation->map[y] = NULL;
	fill(game->player.x, game->player.y, validation, '1');
	free_array(validation->map, 1);
	if (validation->is_enclosed == false)
		cleanup_and_exit(game, ERRENC, 0, 0);
}

static void	validate_map_elements(t_game *game, t_map_validation *validation,
	int x, int y)
{
	if (ft_strchr("NSEW", game->map[y][x]))
	{
		game->player.y = y + 0.5;
		game->player.x = x + 0.5;
		validation->player_count++;
		if (game->map[y][x] == 'N')
			game->player.dir_y = -1;
		if (game->map[y][x] == 'S')
			game->player.dir_y = 1;
		if (game->map[y][x] == 'W')
			game->player.dir_x = -1;
		if (game->map[y][x] == 'E')
			game->player.dir_x = 1;
	}
	if (!ft_strchr("10 NSEWT", game->map[y][x]))
		cleanup_and_exit(game, ERRCHARS, 0, 0);
}

/*If player position is N or S, plane_x is set to 66 degrees and plane_y to 0.
If the position is E or W, vice versa - then plane_x needs to be 0, since it
has to be perpendicular to the ray.*/
static void	init_plane(t_game *game)
{
	game->plane_x = malloc(sizeof(double));
	if (!game->plane_x)
		cleanup_and_exit(game, ERRMEM, 0, 0);
	game->plane_y = malloc(sizeof(double));
	if (!game->plane_y)
		cleanup_and_exit(game, ERRMEM, 0, 0);
	if (game->player.dir_y != 0)
	{
		if (game->player.dir_y == -1)
			*game->plane_x = 0.66;
		else
			*game->plane_x = -0.66;
		*game->plane_y = 0;
	}
	else
	{
		*game->plane_x = 0;
		if (game->player.dir_x == -1)
			*game->plane_y = -0.66;
		else
			*game->plane_y = 0.66;
	}
}

void	initialize_and_validate(t_game *game)
{
	int					x;
	int					y;
	t_map_validation	validation;

	y = 0;
	validation = (t_map_validation){true, 0, NULL};
	game->player = (t_player){0};
	game->mouse_lock = 1;
	while (game->map[y] != NULL)
	{
		x = 0;
		while (game->map[y][x] != '\0')
			validate_map_elements(game, &validation, x++, y);
		if ((int)ft_strlen(game->map[y]) > game->width)
			game->width = (int)ft_strlen(game->map[y]);
		y++;
	}
	init_plane(game);
	if (validation.player_count != 1)
		cleanup_and_exit(game, ERRP, 0, 0);
	game->height = y;
	if (game->width > 200 || game->height > 200)
		cleanup_and_exit(game, ERRSIZE, 0, 0);
	flood_fill(game, &validation);
}
