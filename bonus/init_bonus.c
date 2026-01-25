/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/18 15:43:06 by abouabba          #+#    #+#             */
/*   Updated: 2025/10/24 16:09:11 by abouabba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub_bonus.h"

void	init_game_struct(t_game *game)
{
	game->textures.north = NULL;
	game->textures.south = NULL;
	game->textures.west = NULL;
	game->textures.east = NULL;
	game->floor_color = -1;
	game->ceiling_color = -1;
	game->map = NULL;
	game->map_lines = NULL;
	game->map_width = 0;
	game->map_height = 0;
	game->player.x = -1;
	game->player.y = -1;
	game->map_started = 0;
	game->map_ended = 0;
	game->no = 0;
	game->so = 0;
	game->we = 0;
	game->ea = 0;
	game->f = 0;
	game->c = 0;
	game->player_count = 0;
	game->player.direction = '\0';
}
