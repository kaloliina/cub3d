/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errors_and_exits.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 12:01:25 by sojala            #+#    #+#             */
/*   Updated: 2025/08/14 13:04:03 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D.h"

void	cleanup_and_exit(t_game *game, char *str, bool success, bool textures)
{
	int	i;

	i = 0;
	free_array(game->map, 1);
	free_array(game->asset_paths, 0);
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
			free (array[i]);
			i++;
		}
	}
}
