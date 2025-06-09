# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: khiidenh <khiidenh@student.hive.fi>        +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/01/24 12:16:09 by khiidenh          #+#    #+#              #
#    Updated: 2025/02/19 13:10:32 by khiidenh         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	= so_long
CFLAGS	= -Wextra -Wall -Werror
LIBMLX	= ./MLX42
LIBFT = libft/libft.a

HEADERS	= -I ./include -I $(LIBMLX)/include -I libft/includes
LIBS	= $(LIBMLX)/build/libmlx42.a -lglfw
SRCS_DIR	= src/
SRCS = 	$(SRCS_DIR)so_long.c\
	$(SRCS_DIR)errors_and_exits.c\
	$(SRCS_DIR)game_mechanics.c\
	$(SRCS_DIR)init_and_validate.c\
	$(SRCS_DIR)render.c
OBJS	= $(SRCS:.c=.o)

all: libmlx libft $(NAME)

libmlx:
	cmake $(LIBMLX) -B $(LIBMLX)/build && make -C $(LIBMLX)/build -j4

libft:
	make -C libft

$(SRCS_DIR)%.o: $(SRCS_DIR)%.c
	$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS)

$(NAME): $(OBJS)
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

