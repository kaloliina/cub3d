/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_and_exits.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 16:03:18 by khiidenh          #+#    #+#             */
/*   Updated: 2025/06/16 13:38:15 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "so_long.h"

void	early_cleanup_and_exit(char *str)
{
		ft_putstr_fd(str, 2);
		ft_putstr_fd("\n", 2);
		exit (1);
}

void	cleanup_and_exit(t_game *game, char *str, bool success)
{
	int	y;

	y = 0;
	if (game->map != NULL)
	{
		while (game->map[y] != NULL)
		{
			free (game->map[y]);
			y++;
		}
		free (game->map);
	}
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

void	free_floodmap(char **tab)
{
	int	y;

	y = 0;
	if (tab != NULL)
	{
		while (tab[y] != NULL)
		{
			free (tab[y]);
			y++;
		}
		free (tab);
	}
}
