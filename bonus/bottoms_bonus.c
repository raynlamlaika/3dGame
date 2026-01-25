/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   bottoms_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 12:12:38 by rlamlaik          #+#    #+#             */
/*   Updated: 2025/10/23 14:22:29 by abouabba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub_bonus.h"

void	move_player(t_game *game, double dx, double dy)
{
	t_mov	mov;
	int		curr_map_x;
	int		curr_map_y;

	mov.new_x = game->player.x + (dx / TILE_SIZE) * MOVE_SPEED;
	mov.new_y = game->player.y + (dy / TILE_SIZE) * MOVE_SPEED;
	mov.map_x = (int)mov.new_x;
	mov.map_y = (int)mov.new_y;
	curr_map_x = (int)game->player.x;
	curr_map_y = (int)game->player.y;
	if (mov.map_x < 0 || mov.map_y < 0 \
	|| mov.map_x >= game->map_width || mov.map_y >= game->map_height)
		return ;
	if (game->map[mov.map_y][mov.map_x] == '1' ||
		game->map[curr_map_y][mov.map_x] == '1' ||
		game->map[mov.map_y][curr_map_x] == '1')
		return ;
	if (game->map[mov.map_y][mov.map_x] == ' ' ||
		game->map[curr_map_y][mov.map_x] == ' ' ||
		game->map[mov.map_y][curr_map_x] == ' ')
		return ;
	if (game->map[curr_map_y][mov.map_x] != '1' && \
	game->map[mov.map_y][curr_map_x] != '1')
		(1) && (game->player.y = mov.new_y, game->player.x = mov.new_x);
}

void	helper(t_game *game)
{
	if (game->angle < 0)
		game->angle += 2.0 * M_PI;
	if (game->angle >= 2.0 * M_PI)
		game->angle -= 2.0 * M_PI;
}

int	bottoms(int keycode, t_game *game)
{
	double	dx;
	double	dy;

	(1) && (dy = 0.0, dx = 0.0);
	if (keycode == LEFT)
		game->angle -= ROTATION_SPEED;
	else if (keycode == RIGHT)
		game->angle += ROTATION_SPEED;
	helper(game);
	if (keycode == KEY_W)
		(1) && (dx = cos(game->angle), dy = sin(game->angle));
	if (keycode == KEY_S)
		(1) && (dx = -cos(game->angle), dy = -sin(game->angle));
	if (keycode == KEY_D)
		(1) && (dx = -sin(game->angle), dy = cos(game->angle));
	if (keycode == KEY_A)
		(1) && (dx = sin(game->angle), dy = -cos(game->angle));
	if (keycode == 49)
		game->frame = 1;
	if (keycode == 53)
		exit(0);
	move_player(game, dx, dy);
	mlx_clear_window(game->helper->mlx, game->helper->win);
	return (game_gun(game), 0);
}
