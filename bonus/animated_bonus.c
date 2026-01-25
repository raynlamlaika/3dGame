/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   animated_bonus.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/10 12:04:16 by abouabba          #+#    #+#             */
/*   Updated: 2025/10/24 16:06:21 by abouabba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub_bonus.h"

void	init_gun_files(char **files)
{
	files[0] = "animat/1.xpm";
	files[1] = "animat/2.xpm";
	files[2] = "animat/3.xpm";
	files[3] = "animat/4.xpm";
	files[4] = "animat/5.xpm";
	files[5] = "animat/6.xpm";
	files[6] = "animat/7.xpm";
	files[7] = "animat/8.xpm";
	files[8] = "animat/9.xpm";
	files[9] = "animat/10.xpm";
	files[10] = "animat/11.xpm";
	files[11] = "animat/12.xpm";
	files[12] = "animat/13.xpm";
	files[13] = "animat/14.xpm";
	files[14] = "animat/15.xpm";
	files[15] = "animat/16.xpm";
	files[16] = "animat/17.xpm";
}

void	load_gun_sprite(t_game *game)
{
	int		i;
	char	*files[17];

	init_gun_files(files);
	i = 0;
	while (i < 17)
	{
		game->gun[i].img = mlx_xpm_file_to_image(game->helper->mlx, files[i],
				&game->gun[i].width, &game->gun[i].height);
		if (!game->gun[i].img)
		{
			printf("Error: failed to load %s\n", files[i]);
			exit(1);
		}
		i++;
	}
	game->frame = 0;
}

void	draw_gun(t_game *game)
{
	t_sprite	*gun;

	gun = &game->gun[game->frame];
	mlx_put_image_to_window(game->helper->mlx, game->helper->win, gun->img,
		(SCREEN_WIDTH / 2) - (gun->width / 2),
		SCREEN_HEIGHT - gun->height);
}

int	animate_gun(t_game *game)
{
	static int	counter;
	static int	idle_frame;

	counter++;
	if (game->frame == 0)
	{
		if (counter > 10)
		{
			idle_frame = (idle_frame + 1) % 1;
			game->frame = 0;
			counter = 0;
		}
	}
	else
	{
		if (counter > 2)
		{
			game->frame++;
			if (game->frame >= 17)
				game->frame = 0;
			counter = 0;
		}
	}
	return (0);
}

int	game_gun(t_game *game)
{
	render_map(game);
	draw_gun(game);
	animate_gun(game);
	return (0);
}
