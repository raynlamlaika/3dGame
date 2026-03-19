/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouabba <abouabba@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/21 05:15:23 by rlamlaik          #+#    #+#             */
/*   Updated: 2025/10/24 20:57:42 by abouabba         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub.h"

static int	is_texture_space(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\r'
		|| c == '\v' || c == '\f');
}

static void	trim_texture_path(char *file)
{
	size_t	len;
	size_t	start;
	size_t	i;

	if (!file)
		return ;
	start = 0;
	while (file[start] && (file[start] == ' ' || file[start] == '\t'))
		start++;
	if (start > 0)
	{
		i = 0;
		while (file[start + i])
		{
			file[i] = file[start + i];
			i++;
		}
		file[i] = '\0';
	}
	len = ft_strlen(file);
	while (len > 0 && is_texture_space(file[len - 1]))
	{
		file[len - 1] = '\0';
		len--;
	}
}

void	is_valid_texture(t_game *game)
{
	if (game->no != 1 || game->so != 1 || game->we != 1 \
	|| game->ea != 1 || game->f != 1 || game->c != 1)
	{
		perror("Invalid texture");
		exit(1);
	}
}

t_txtu	init_txtu(t_game *game, char *file)
{
	t_txtu	txtu;

	if (file == NULL)
		return (printf("passing in NULL file \n"), exit(1), txtu);
	trim_texture_path(file);
	if (*file == '\0')
		return (printf("Empty texture path\n"), exit(1), txtu);
	if (access(file, F_OK))
	{
		printf("wrong access: |%s|\n", file);
		exit(1);
	}
	txtu.txture_p = mlx_xpm_file_to_image(\
	game->helper->mlx, file, &txtu.width, &txtu.height);
	if (!txtu.txture_p)
	{
		printf("textuer error \n");
		exit(1);
	}
	txtu.data_add = mlx_get_data_addr \
	(txtu.txture_p, &txtu.bbp, &txtu.line_len, &txtu.endian);
	return (txtu);
}

int	closer(t_game *game)
{
	(void)game;
	exit(1);
	return (1);
}

void	hookshandeling(t_game *game)
{
	is_valid_texture(game);
	finalize_map(game);
	validate_map(game);
	init_game(game);
	game->txtu[0] = init_txtu(game, game->textures.east);
	game->txtu[1] = init_txtu(game, game->textures.north);
	game->txtu[2] = init_txtu(game, game->textures.south);
	game->txtu[3] = init_txtu(game, game->textures.west);
	init_player_direction(game, game->player.direction);
	mlx_hook(game->helper->win, 17, 0, closer, game);
	mlx_loop_hook(game->helper->mlx, &render_map, game);
	mlx_hook(game->helper->win, 2, 1, bottoms, game);
	mlx_loop(game->helper->mlx);
}

int	main(int ac, char **av)
{
	t_game	*game;
	int		fd;
	char	*line;

	check_argument(ac, av);
	game = ft_malloc(sizeof(t_game), 1);
	if (!game)
	{
		perror("Error\n<----!Memory allocation failed---->");
		exit(1);
	}
	init_game_struct(game);
	fd = open(av[1], O_RDONLY);
	if (fd == -1)
	{
		perror("Error\n<----!Can't open file---->");
		exit(1);
	}
	line = get_next_line(fd);
	while (line != NULL)
	{
		parse_line(game, line);
		line = get_next_line(fd);
	}
	return (close(fd), hookshandeling(game), 0);
}
