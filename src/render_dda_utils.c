/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_dda_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 12:01:47 by sojala            #+#    #+#             */
/*   Updated: 2025/08/14 12:01:48 by sojala           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D.h"

void	init_dda(t_dda *dda, t_game *game)
{
	dda->pos_x = game->player.x;
	dda->pos_y = game->player.y;
	dda->dir_x = game->player.dir_x;
	dda->dir_y = game->player.dir_y;
	dda->raydir_x = 0.0;
	dda->raydir_y = 0.0;
	dda->sidedist_x = 0.0;
	dda->sidedist_y = 0.0;
	dda->squaredist_x = 0.0;
	dda->squaredist_y = 0.0;
	dda->camera_x = 0.0;
	dda->corr_dist = 0.0;
	dda->lineheight = 0.0;
	dda->map_x = dda->pos_x;
	dda->map_y = dda->pos_y;
	dda->step_x = 0;
	dda->step_y = 0;
	dda->hit = 0;
	dda->hor_side = 0;
	dda->y_start = 0;
	dda->y_end = 0;
}
