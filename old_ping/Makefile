# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ndelhota <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/02/10 17:08:53 by ndelhota          #+#    #+#              #
#    Updated: 2026/04/20 19:15:15 by ndelhota         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME := ft_ping 

SRC := main.c \
       srcs/parse_retrieve_args.c \
       srcs/ft_init.c \
       srcs/sig.c \
       srcs/ft_end.c \
       srcs/utils.c \

CC := cc

CFLAGS := -g -Wall -Werror -Wextra 

OBJDIR = obj
OBJ = $(SRC:.c=.o)
OBJ := $(addprefix $(OBJDIR)/, $(OBJ))


all : $(OBJ) $(NAME)

$(NAME) : $(OBJ)
	@$(CC) $(CFLAGS) $(OBJ) $(LIB) -o $(NAME)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c -o $@ $^

clean :
	@rm -rf $(OBJDIR) 

fclean : clean 
	@rm -f $(NAME)

re : fclean all

.PHONY : all clean fclean re
