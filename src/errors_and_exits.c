#include "cub3D.h"

void	cleanup_and_exit(t_game *game, char *str, bool success)
{
	free_array(game->map, 1);
	free_array(game->asset_paths, 0);
	/*Added freeing plane_x and plane_y, atm they have to be in struct as pointers
	bc they are used in different functions but to be decided if this is the
	smartest way*/
	free(game->plane_x);
	free(game->plane_y);
	if (game->mlx != NULL)
		mlx_terminate(game->mlx);
	if (success == true)
	{
		exit (0);
	}
	if (success == false)
	{
		ft_putstr_fd(str, 2);
		ft_putstr_fd("\n", 2);
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
