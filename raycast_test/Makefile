NAME = cub3d
CC = cc
CFLAGS = -Wextra -Wall -Werror
MLXDIR = ./MLX42
MLX = $(MLXDIR)/build/libmlx42.a

HEADERS = -I$(MLXDIR)/include
LIBS = $(MLXDIR)/build/libmlx42.a -ldl -lglfw -pthread -lm
SRC = main.c
OBJ = $(SRC:.c=.o)

all: $(NAME)

$(MLX):
	@cmake $(MLXDIR) -B $(MLXDIR)/build
	@make -C $(MLXDIR)/build -j4

$(NAME): $(MLX) $(OBJ)
	$(CC) $(OBJ) $(LIBS) $(HEADERS) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS)

clean:
	@rm -rf $(OBJ)
	@make clean -C $(MLXDIR)/build

fclean: clean
	@rm -rf $(NAME)

re: clean all

.PHONY: all, clean, fclean, re, bonus
