CC = cc
CFLAGS = -Wall -Wextra -Werror
NAME = cub3d
NAME_B = cub3d_bonus
HEADER = mandatory/cub.h
HEADER_B = bonus/cub_bonus.h

SRCS = mandatory/check_args.c mandatory/config_parser.c mandatory/ft_split.c mandatory/get_next_line.c \
       mandatory/init.c mandatory/main.c mandatory/map_parser.c mandatory/parse_map.c mandatory/parser.c mandatory/utils_2.c mandatory/utils.c \
	   mandatory/validate_map.c mandatory/mlx.c mandatory/rays.c mandatory/bottoms.c mandatory/ft_malloc.c \
	   mandatory/draw_f_c.c mandatory/rayshelper.c mandatory/parse_helper.c mandatory/ft_putnbr.c

SRCS_B = bonus/check_args_bonus.c bonus/config_parser_bonus.c bonus/ft_split_bonus.c bonus/get_next_line_bonus.c \
       bonus/init_bonus.c bonus/main_bonus.c bonus/map_parser_bonus.c bonus/parse_map_bonus.c bonus/parser_bonus.c bonus/utils_2_bonus.c bonus/utils_bonus.c \
	   bonus/validate_map_bonus.c bonus/mlx_bonus.c bonus/rays_bonus.c bonus/bottoms_bonus.c bonus/ft_malloc_bonus.c \
	   bonus/draw_f_c_bonus.c bonus/rayshelper_bonus.c bonus/parse_helper_bonus.c bonus/ft_putnbr_bonus.c bonus/animated_bonus.c \
	   bonus/minimap_bonus.c bonus/minimap_helper_bonus.c bonus/mouse_move_bonus.c

OBJS = $(SRCS:.c=.o)
OBJS_B = $(SRCS_B:.c=.o)

# Detect operating system
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
	MLX_FLAG = -lmlx -lXext -lX11 -lm -lz
endif
ifeq ($(UNAME_S),Darwin)
	MLX_FLAG = -lmlx -framework OpenGL -framework AppKit
endif

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(MLX_FLAG) -o $(NAME)

bonus: $(NAME_B)

$(NAME_B): $(OBJS_B)
	$(CC) $(CFLAGS) $(OBJS_B) $(MLX_FLAG) -o $(NAME_B)

bonus/%.o: bonus/%.c $(HEADER_B)
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.c $(HEADER) 
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	rm -f $(OBJS) $(OBJS_B)

fclean: clean
	rm -f $(NAME) $(NAME_B)

re: fclean all

.PHONY: clean
