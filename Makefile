# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: gecarval <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/08/21 17:12:20 by gecarval          #+#    #+#              #
#    Updated: 2024/09/10 20:42:01 by gecarval         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = render
CC = cc
CFLAGS = -Wall -Wextra -Werror
LIBFT = libft.a
MINILIBX = mlx_Linux.a
INCS = -I./includes -I./libft -L./libft ./libft/$(LIBFT) -I./minilibx-linux -lmlx_Linux -L./minilibx-linux -lmlx -L/usr/include/../lib -lXext -lX11 -lm
SRCS = ./cubix/math_del.c ./cubix/bresenham.c ./cubix/cubix.c ./fluidsim/part_call.c ./fluidsim/part_emul.c ./fluidsim/part_emul_solid.c ./fluidsim/part_emul_liquid.c ./fluidsim/part_emul_gas.c ./fluidsim/part_emul_logic.c ./fluidsim/fluidsim.c ./calls.c ./reads.c ./tools.c ./main.c
OBJS = $(SRCS:.c=.o)
RM = rm -drf

all: $(MINILIBX) $(LIBFT) $(NAME)

run:
	./$(NAME)

$(NAME): $(OBJS) 
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(INCS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(LIBFT):
	make -C libft

$(MINILIBX):
	make -C minilibx-linux

clean:
	$(RM) $(OBJS)
	make clean -C minilibx-linux
	make clean -C libft

fclean: clean
	$(RM) $(NAME)
	make fclean -C libft

re: fclean all
