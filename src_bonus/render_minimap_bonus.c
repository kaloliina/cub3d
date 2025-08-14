/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_minimap_bonus.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/14 12:02:48 by sojala            #+#    #+#             */
/*   Updated: 2025/08/14 14:21:33 by khiidenh         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D_bonus.h"

/*This function calculates the starting and end points of the portion
of the minimap that we render to be visible at the moment, focusing on
the player.*/
static int	x_edges(double player, bool start, int max, int start_point)
{
	int	point;

	if (start)
	{
		point = player - 10;
		if (point < 0)
			point = 0;
	}
	else
	{
		point = player + 10;
		if (point - start_point < 20)
			point += (20 - (point - start_point));
		if (point >= max)
			point = max;
	}
	return (point);
}

static int	y_edges(double player, int start, int max, int start_point)
{
	int	point;

	if (start)
	{
		point = player - 6;
		if (point < 0)
			point = 0;
	}
	else
	{
		point = player + 6;
		if (point - start_point < 12)
			point += (12 - (point - start_point));
		if (point >= max)
			point = max;
	}
	return (point);
}

static void	draw_minimap_base(t_game *game, int x_start, int y_start, int y_end)
{
	int	y;
	int	y1;
	int	x;
	int	x1;
	int	x_end;

	y = y_start;
	y1 = 0;
	while (y < y_end)
	{
		x = x_start;
		x_end = x_edges(game->player.x, 0, ft_strlen(game->map[y]), x_start);
		x1 = 0;
		while (x < x_end)
		{
			if (game->map[y][x] == '1' || game->map[y][x] == ' ')
				draw_pixels(game, WALL, x1 * TILE, y1 * TILE);
			else if (ft_strchr("0NSEWH", game->map[y][x]))
				draw_pixels(game, BASE, x1 * TILE, y1 * TILE);
			x1++;
			x++;
		}
		y1++;
		y++;
	}
}

void	render_minimap(t_game *game)
{
	int	x_start;
	int	y_start;
	int	y_end;

	x_start = x_edges(game->player.x, 1, game->width, 0);
	y_start = y_edges(game->player.y, 1, game->height, 0);
	y_end = y_edges(game->player.y, 0, game->height, y_start);
	draw_minimap_base(game, x_start, y_start, y_end);
	draw_pixels(game, PLAYER, (game->player.x - 0.5 - x_start) * TILE,
		(game->player.y - 0.5 - y_start) * TILE);
	draw_line(game, (game->player.x - x_start) * TILE,
		(game->player.y - y_start) * TILE);
}
