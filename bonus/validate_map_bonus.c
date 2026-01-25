/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_map_bonus.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/23 16:30:20 by abouabba          #+#    #+#             */
/*   Updated: 2025/10/24 16:07:48 by abouabba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub_bonus.h"

int	is_player_char(char c)
{
	return (c == 'N' || c == 'S' || c == 'E' || c == 'W');
}

void	check_valid_char(char c)
{
	if (!is_valid_char(c))
	{
		print_error("Error: Invalid character\n");
		exit(EXIT_FAILURE);
	}
}

void	check_player(t_game *game, char c, int x, int y)
{
	if (is_player_char(c))
	{
		game->player_count++;
		game->player.x = x + 0.5;
		game->player.y = y + 0.5;
		game->player.direction = c;
	}
}

void	check_map_closed(t_game *game, char c, int x, int y)
{
	if ((c == '0' || is_player_char(c)) && (
			y == 0 || x == 0 || y >= game->map_height - 1
			|| x >= game->map_width - 1 || game->map[y - 1][x] == ' '
		|| game->map[y + 1][x] == ' ' || game->map[y][x - 1] == ' '
		|| game->map[y][x + 1] == ' '))
		print_error("Map is not closed");
}

void	validate_map(t_game *game)
{
	int		y;
	int		x;
	char	c;

	y = -1;
	while (++y < game->map_height)
	{
		x = -1;
		while (++x < game->map_width)
		{
			c = game->map[y][x];
			check_valid_char(c);
			check_player(game, c, x, y);
			check_map_closed(game, c, x, y);
		}
	}
	if (game->player_count != 1)
		print_error("There must be exactly one player");
}
