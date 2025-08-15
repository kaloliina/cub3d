# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/24 12:16:09 by khiidenh          #+#    #+#              #
#    Updated: 2025/08/15 11:22:46 by sojala           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= cub3D
NAME_BONUS	= cub3D_bonus
CFLAGS	= -Wall -Wextra -Werror -Ofast
MLX_DIR	= ./MLX42
MLX = $(MLX_DIR)/build/libmlx42.a
LIBFT = libft/libft.a

HEADERS	= -I ./include/cub3D.h -I $(MLX_DIR)/include -I libft/includes
HEADERS_BONUS	= -I ./include/cub3d_bonus.h -I $(MLX_DIR)/include -I libft/includes
LIBS	= $(MLX_DIR)/build/libmlx42.a -ldl -lglfw -pthread -lm
SRCS_DIR	= src/
SRCS_BONUS_DIR	= src_bonus/
SRCS = 	$(SRCS_DIR)cub3d.c\
	$(SRCS_DIR)errors_and_exits.c\
	$(SRCS_DIR)game_mechanics.c\
	$(SRCS_DIR)init_and_validate.c\
	$(SRCS_DIR)parse_file.c\
	$(SRCS_DIR)parse_utils.c\
	$(SRCS_DIR)render_dda.c\
	$(SRCS_DIR)render_dda_utils.c\
	$(SRCS_DIR)render_minimap.c\
	$(SRCS_DIR)render_utils.c\
	$(SRCS_DIR)render.c\
	$(SRCS_DIR)wall_textures.c
SRCS_BONUS = 	$(SRCS_BONUS_DIR)cub3d_bonus.c\
	$(SRCS_BONUS_DIR)errors_and_exits_bonus.c\
	$(SRCS_BONUS_DIR)game_mechanics_bonus.c\
	$(SRCS_BONUS_DIR)init_and_validate_bonus.c\
	$(SRCS_BONUS_DIR)parse_file_bonus.c\
	$(SRCS_BONUS_DIR)parse_utils_bonus.c\
	$(SRCS_BONUS_DIR)render_dda_bonus.c\
	$(SRCS_BONUS_DIR)render_dda_utils_bonus.c\
	$(SRCS_BONUS_DIR)render_minimap_bonus.c\
	$(SRCS_BONUS_DIR)render_sprites_bonus.c\
	$(SRCS_BONUS_DIR)render_sprites_utils_bonus.c\
	$(SRCS_BONUS_DIR)render_utils_bonus.c\
	$(SRCS_BONUS_DIR)render_bonus.c\
	$(SRCS_BONUS_DIR)wall_textures_bonus.c
OBJS	= $(SRCS:.c=.o)
OBJS_BONUS	= $(SRCS_BONUS:.c=.o)

all: $(MLX) $(LIBFT) $(NAME)

bonus: $(MLX) $(LIBFT) $(NAME_BONUS)

$(MLX):
	@if [ ! -d $(MLX_DIR) ]; then \
		git clone https://github.com/codam-coding-college/MLX42.git $(MLX_DIR); \
		fi
	cmake $(MLX_DIR) -B $(MLX_DIR)/build && make -C $(MLX_DIR)/build -j4

$(LIBFT):
	make -C libft

$(SRCS_DIR)%.o: $(SRCS_DIR)%.c
	$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS)

$(SRCS_BONUS_DIR)%.o: $(SRCS_BONUS_DIR)%.c
	$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS_BONUS)

$(NAME): $(OBJS) $(MLX)
	$(CC) $(OBJS) $(LIBS) $(LIBFT) $(HEADERS) -o $(NAME)

$(NAME_BONUS): $(OBJS_BONUS) $(MLX)
	$(CC) $(OBJS_BONUS) $(LIBS) $(LIBFT) $(HEADERS_BONUS) -o $(NAME_BONUS)

clean:
	rm -rf $(OBJS)
	rm -rf $(OBJS_BONUS)
	rm -rf $(MLX_DIR)/build
	make -C libft clean

fclean: clean
	rm -rf $(NAME) $(NAME_BONUS)
	make -C libft fclean
	@if [ -d $(MLX_DIR) ]; then \
		rm -rf $(MLX_DIR); \
		fi

re: clean all

.PHONY: all, clean, fclean, re, libmlx, libft
