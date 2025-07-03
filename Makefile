# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: sojala <sojala@student.hive.fi>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/24 12:16:09 by khiidenh          #+#    #+#              #
#    Updated: 2025/07/03 09:34:26 by sojala           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= cub3D
CFLAGS	=
MLX_DIR	= ./MLX42
MLX = $(LIBMLX)/build/libmlx42.a
LIBFT = libft/libft.a

HEADERS	= -I ./include -I $(MLX_DIR)/include -I libft/includes
LIBS	= $(MLX_DIR)/build/libmlx42.a -ldl -lglfw -pthread -lm
SRCS_DIR	= src/
SRCS = 	$(SRCS_DIR)so_long.c\
	$(SRCS_DIR)errors_and_exits.c\
	$(SRCS_DIR)game_mechanics.c\
	$(SRCS_DIR)init_and_validate.c\
	$(SRCS_DIR)render.c\
	$(SRCS_DIR)parse_file.c
OBJS	= $(SRCS:.c=.o)

all: $(MLX) libft $(NAME)

$(MLX): $(MLX_DIR)
	cmake $(MLX_DIR) -B $(MLX_DIR)/build && make -C $(MLX_DIR)/build -j4

libft:
	make -C libft

$(SRCS_DIR)%.o: $(SRCS_DIR)%.c
	$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS)

$(NAME): $(OBJS) $(MLX)
	$(CC) $(OBJS) $(LIBS) $(LIBFT) $(HEADERS) -o $(NAME)

clean:
	rm -rf $(OBJS)
	rm -rf $(LIBMLX)/build
	make -C libft clean

fclean: clean
	rm -rf $(NAME)
	make -C libft fclean

re: clean all

.PHONY: all, clean, fclean, re, libmlx, libft

