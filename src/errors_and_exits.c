/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_and_exits.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:03:18 by khiidenh          #+#    #+#             */
/*   Updated: 2025/06/17 14:43:45 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	cleanup_and_exit(t_game *game, char *str, bool success)
{
	free_array(game->map, 1);
	free_array(game->asset_paths, 0);
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
