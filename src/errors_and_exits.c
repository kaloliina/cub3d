#include "cub3D.h"

/*Added freeing plane_x and plane_y, atm they have to be in struct as pointers
bc they are used in different functions but to be decided if this is the
smartest way*/
void	cleanup_and_exit(t_game *game, char *str, bool success, bool textures)
{
	int	i;

	i = 0;
	free_array(game->map, 1);
	free_array(game->asset_paths, 0);
	if (game->plane_x)
		free (game->plane_x);
	if (game->plane_y)
		free (game->plane_y);
	if (game->mlx != NULL)
		mlx_terminate(game->mlx);
	while (textures && i < TEXTURE_COUNT)
		mlx_delete_texture(game->textures[i++]);
	if (success == true)
	{
		exit (0);
	}
	if (success == false)
	{
		ft_putstr_fd(ERRMSG, 2);
		ft_putstr_fd(str, 2);
		exit (1);
	}
}

void	free_array(char **array, int entirety)
{
	int	i;

	i = 0;
	if (entirety == 1)
	{
		if (array != NULL)
		{
			while (array[i] != NULL)
			{
				printf("Freeing %s\n", array[i]);
				free (array[i]);
				i++;
			}
			free (array);
		}
	}
	if (entirety == 0)
	{
		while (array[i] != NULL)
		{
			printf("Freeing %s\n", array[i]);
			free (array[i]);
			i++;
		}
	}
}
